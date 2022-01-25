/**
 * CAN-info reader with heater controller
 * Система получения информации из шины CAN-info
 * с функцией управления дополнительным подогревателем
 *
 * authors:
 * Михаил Федоров [https://github.com/kolabse]
 * Илья Мартемьянов [https://github.com/Sonc-man]
 *
 * Список CAR_TYPE:
 *  0 - without can decode, only store data;
 *  1 - PSACars - see PSACars.h
*/

#define CAR_TYPE 1

// Подключение необходимых библиотек
#include <GyverTimers.h>
#include <SPI.h>
#include <mcp2515.h>
#include <Wire.h>
#include <LiquidCrystal_I2C.h>
#include <KolabseCarsCan.h>                   // https://github.com/kolabse/KolabseCarsCan
#include "heater.h"

// Назначение пинов и адресов
const int heaterKey1    {4};          // Пин подключения реле нагревателя №1
const int lcdSwitch     {5};          // Пин переключателя наличия LCD дисплея
const int heaterSwitch  {6};          // Пин переключателя наличия нагревателя
const int heaterKey2    {7};          // Пин подключения реле нагревателя №2
const int heaterKey3    {8};          // Пин подключения реле нагревателя №3
const int debugSwitch   {9};          // Пин переключателя включения режима отправки отладочной информации в COM-порт
const int mcpScPin      {10};
const int lcdI2CAddr    {0x3F};

// Установка значений по умолчанию
const int startValue    {-99};

// Объявление переменных и значений по умолчанию для них
uint32_t secAfterStartWhenLcdLastUpdate {0};                      // Время в секундах после того как последний раз были обновлены показания экрана
uint32_t SecAfterStartWhenLastCanMessageReceived {startValue};    // Время в секундах после того как было получено последнее сообщение из шины

bool logoIsActive   {true};                                         // Заставка активна
bool needToHeat     {false};                                          // Необходимость в дополнительном нагреве
bool LCDEnabled     {false};
bool HeaterEnabled  {false};
bool DebugMode      {false};

// LCD дисплей
LiquidCrystal_I2C lcd(lcdI2CAddr, 16, 2); // (адрес - определен через i2c_scanner, размеры дисплея)

// CAN модуль
MCP2515 mcp2515(mcpScPin);

// Электрический радиатор (Цепь силового ключа 1, Цепь силового ключа 2, Цепь силового ключа 3)
Heater heater(heaterKey1, heaterKey2, heaterKey3);

// Сообщение CAN шины
struct can_frame canMsg;

// Блок инициализации перед запуском основной программы
void setup() {

  // Читаем установки с переключателей
  pinMode(lcdSwitch, INPUT_PULLUP);
  pinMode(heaterSwitch, INPUT_PULLUP);
  pinMode(debugSwitch, INPUT_PULLUP);

  LCDEnabled = !digitalRead(lcdSwitch);
  HeaterEnabled = !digitalRead(heaterSwitch);
  DebugMode = !digitalRead(debugSwitch);

  // Инициализация дисплея
  lcd.begin();

  // включение подсвеки
  lcd.backlight();

  // вывод экрана приветствия
  lcd.setCursor(1, 0);
  lcd.print("Heat controller");
  lcd.setCursor(0, 1);
  lcd.print("v0.04b loading...");

  // Устанавливаем частоту аппаратного прерывания с частотой 0.20 Hz (1 раз в 5 секунд) для таймера 1
  Timer1.setFrequencyFloat(0.20);
  // Запускаем таймер 1 аппаратного прерывания на канале "A" (порт D9)
  Timer1.enableISR(CHANNEL_A);

  // Настраиваем фильтрацию сообщений CAN-шины
  car.setCanFilters(mcp2515);

    if(DebugMode) {
    // Запускаем интерфейс COM-порта
    Serial.begin(115200);

    Serial.print("LCDEnabled: ");
    Serial.println(LCDEnabled);
    Serial.print("HeaterEnabled: ");
    Serial.println(HeaterEnabled);

    Serial.println("------- CAN Read ----------");
    Serial.println("ID  DLC   0   1   2   3   4   5   6   7");
  }

}

ISR(TIMER1_A) {

  // Проверяем достаточно ли прошло времени с момента запуска двигателя
  if (car.getSecAfterStart() > 20) {
    // Запускаем функции обработки значений полученных из шины
    // Проверяем полученны ли все необходимые данные
    if (
      car.getCoolantTemp() != startValue
      && car.getOutdoorTemp() != startValue
      && car.getBatteryVoltage() != startValue
      && car.getClimateFanSpeed() != startValue
      && car.getClimateLeftTemp() != startValue
      && car.getClimateRightTemp() != startValue
    ) {
      // Проверяем обязательные требования
      if (
        car.getClimateFanSpeed() >= 3
        && car.getBatteryVoltage() >= 13.5
        && car.getClimateLeftTemp() >= 5
        && car.getClimateRightTemp() >= 5
      ) {
        if (
          // Если подогрев выключен, проверяем не находимся ли мы в условиях подходящих для включения
          needToHeat == false
          &&
          (
            car.getOutdoorTemp() <= -20 && car.getCoolantTemp() < 65
            || car.getOutdoorTemp() <= -10 && car.getCoolantTemp() < 60
            || car.getOutdoorTemp() <=   0 && car.getCoolantTemp() < 55
            || car.getOutdoorTemp() <=  10 && car.getCoolantTemp() < 50
          )
        ) {
          needToHeat = true;
        } else if (
          // Если подогрев включен, проверяем не находимся ли мы в условиях подходящих для отключения
          needToHeat
          &&
          (
            car.getOutdoorTemp() <= -20 && car.getCoolantTemp() >= 80
            || car.getOutdoorTemp() <= -10 && car.getCoolantTemp() >= 75
            || car.getOutdoorTemp() <=   0 && car.getCoolantTemp() >= 70
            || car.getOutdoorTemp() <=  10 && car.getCoolantTemp() >= 65
          )
        ) {
          needToHeat = false;
        }
      } else if (
        needToHeat == false
        &&
        (
          car.getBlowingWindshield() == true
          && car.getClimateLeftTemp() > 16
          && car.getClimateRightTemp() > 16
          && car.getClimateFanSpeed() >= 3
          && car.getRecyclingAir() == false
        )
      ) {
        needToHeat = true;
      } else {
        needToHeat = false;
      }
      // Запускаем функции для определения необходимости управления нагревателем

      heater.needToHeat(car, needToHeat);
    }
  }
}

void loop() {

  car.setSecAfterStart(getSecAfterStart());

  if (logoIsActive and car.getSecAfterStart() > 4) {
    logoIsActive = false;
    lcd.clear();
  }

  if (mcp2515.readMessage(&canMsg) == MCP2515::ERROR_OK) {

    SecAfterStartWhenLastCanMessageReceived = car.getSecAfterStart();
    if (!logoIsActive) {
      lcd.setCursor(10, 1);
      lcd.print("(V)");
    }

    if (DebugMode) {
      Serial.print(canMsg.can_id, HEX); // print ID
      Serial.print(" ");

      Serial.print(canMsg.can_dlc, HEX); // print DLC
      Serial.print(" ");

      for (int i = 0; i < canMsg.can_dlc; i++)  { // print the data
        Serial.print(canMsg.data[i], HEX);
        Serial.print(" ");
      }
    }

    car.decodeCanMessage(canMsg);

    if (DebugMode) {
      // Вывод отладочной информации в Serial порт платы
      Serial.print(car.getSecAfterStart());
      Serial.print("s ");

      if (car.getBatteryVoltage() != startValue) {
        Serial.print(car.getBatteryVoltage());
        Serial.print("v ");
      }

      if (car.getCoolantTemp() != startValue) {
        Serial.print(car.getCoolantTemp());
        Serial.print("°C ");
      }

      if (car.getOutdoorTemp() != startValue) {
        Serial.print(car.getOutdoorTemp());
        Serial.print("°C ");
      }

      if (car.getInstFuelCons() != startValue) {
        Serial.print(car.getInstFuelCons());
        Serial.print("L/100km ");
      }

      if (car.getClimateFanSpeed() != startValue) {
        Serial.print(car.getClimateFanSpeed());
        Serial.print("sp. ");
      }

      if (car.getClimateLeftTemp() != startValue) {
        Serial.print(car.getClimateLeftTemp());
        Serial.print("°C ");
      }

      if (car.getClimateRightTemp() != startValue) {
        Serial.print(car.getClimateRightTemp());
        Serial.print("°C ");
      }

      if (car.getRecyclingAir() != startValue) {
        Serial.print("is_rc: ");
        Serial.print(car.getRecyclingAir());
      }

      if (car.getBlowingWindshield() != startValue) {
        Serial.print(" is_bw: ");
        Serial.print(car.getBlowingWindshield());
      }

      Serial.print(" Need_to_heat: ");
      Serial.print(needToHeat);

      Serial.print(" HeatingIntensity: ");
      Serial.print(heater.getHeatingIntensity());

      Serial.println();
    }
  }

  if (car.getSecAfterStart() - secAfterStartWhenLcdLastUpdate > 5) {

    secAfterStartWhenLcdLastUpdate = car.getSecAfterStart();

    if (car.getBatteryVoltage() != startValue) {
      lcd.setCursor(0, 0);
      lcd.print(String(car.getBatteryVoltage()));
      lcd.print("v");
    }

    if (car.getCoolantTemp() != startValue) {
      lcd.setCursor(7, 0);
      lcd.print(valueWithOffset(car.getCoolantTemp(), true));
      lcd.print("C");
    }

    if (car.getOutdoorTemp() != startValue) {
      lcd.setCursor(12, 0);
      lcd.print(valueWithOffset(car.getOutdoorTemp(), true));
      lcd.print("C");
    }

    if (car.getInstFuelCons() != startValue) {
      lcd.setCursor(0, 1);
      lcd.print(valueWithOffset(car.getInstFuelCons(), false));
      lcd.print("L");
    }

    if (
      car.getCoolantTemp() != startValue
      or car.getOutdoorTemp() != startValue
      or car.getBatteryVoltage() != startValue
      or car.getInstFuelCons() != startValue
    ) {
      lcd.setCursor(15, 1);
      lcd.print(String(heater.getHeatingIntensity()));
    }
  }

  if (!logoIsActive and car.getSecAfterStart() - SecAfterStartWhenLastCanMessageReceived > 5) {
    lcd.setCursor(10, 1);
    lcd.print("(X)");
    digitalWrite(heaterKey1, LOW);
    digitalWrite(heaterKey2, LOW);
    digitalWrite(heaterKey3, LOW);
  }
}

String valueWithOffset(float value, bool isInt) {
  String offsetForValue = "";

  if ((value > -10 and value < 0) or (value >= 10 and value < 100)) {
    offsetForValue = " ";
  } else if (value >= 0 and value < 10 ) {
    offsetForValue = "  ";
  }

  if (isInt) {
    return offsetForValue + String(int(value));
  }

  return offsetForValue + String(value);
}

uint32_t getSecAfterStart() { return millis() / 1000ul; }
