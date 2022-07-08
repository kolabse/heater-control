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
#include "config.h"

// LCD дисплей
LiquidCrystal_I2C lcd(lcdI2CAddr, 16, 2); // (адрес - определен через i2c_scanner, размеры дисплея)

// CAN модуль
MCP2515 mcp2515(mcpScPin);

// Электрический радиатор (Цепь силового ключа 1, Цепь силового ключа 2, Цепь силового ключа 3)
Heater heater(heaterKey1, heaterKey2);

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

  if(LCDEnabled){
    // Инициализация дисплея
    lcd.begin();

    // включение подсвеки
    lcd.backlight();

    // вывод экрана приветствия
    lcd.setCursor(1, 0);
    lcd.print("Heat controller");
    lcd.setCursor(0, 1);
    lcd.print("v0.04b loading...");
  }
  // Устанавливаем частоту аппаратного прерывания с частотой 0.20 Hz (1 раз в 5 секунд) для таймера 1
  Timer1.setFrequencyFloat(systemCheckFrequency);
  // Запускаем таймер 1 аппаратного прерывания на канале "A" (порт D9)
  Timer1.enableISR(CHANNEL_A);

  // Настраиваем фильтрацию сообщений CAN-шины
  car.setCanFilters(mcp2515);

    if(DebugMode) {
    // Запускаем интерфейс COM-порта
    Serial.begin(serialBaudRate);

    Serial.print("LCDEnabled: ");
    Serial.println(LCDEnabled);
    Serial.print("HeaterEnabled: ");
    Serial.println(HeaterEnabled);

    Serial.println("------- CAN Read ----------");
    Serial.println("ID  DLC   0   1   2   3   4   5   6   7");
  }

}

ISR(TIMER1_A) {

  // TODO: move all heater logic and methods to heater lib

  // Проверяем достаточно ли прошло времени с момента запуска двигателя
  if (car.getSecAfterStart() > secAfterStartBeforeHeaterEnabled) {
    // Запускаем функции обработки значений полученных из шины
    // Проверяем полученны ли все необходимые данные
    if (isAllNecessaryDataReceived()) {
      // Проверяем обязательные требования
      if (isCarReady()) {
        if (
          // Если подогрев выключен, проверяем не находимся ли мы в условиях подходящих для включения
          needToHeat == false
          &&
          (isHeaterMustBeOn())
        ) {
          needToHeat = true;
        } else if (
          // Если подогрев включен, проверяем не находимся ли мы в условиях подходящих для отключения
          needToHeat
          &&
          (isHeaterMustBeOff())
        ) {
          needToHeat = false;
        }
      } else if (
        needToHeat == false
        &&
        (isClimateInDefrostMode())
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

  if (logoIsActive and car.getSecAfterStart() > startlogoActiveSec and LCDEnabled) {
    logoIsActive = false;
    lcd.clear();
  }

  if (mcp2515.readMessage(&canMsg) == MCP2515::ERROR_OK) {

    secAfterStartWhenLastCanMessageReceived = car.getSecAfterStart();
    if (!logoIsActive and LCDEnabled) {
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

  if (car.getSecAfterStart() - secAfterStartWhenLcdLastUpdate > lcdUpdateIntervalSec and LCDEnabled) {

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

  if (car.getSecAfterStart() - secAfterStartWhenLastCanMessageReceived > canIsDisconnectTimeout) {
    if(!logoIsActive and LCDEnabled) {
      lcd.setCursor(10, 1);
      lcd.print("(X)");
    }
    heater.emergencyStop();
  }
}

bool isAllNecessaryDataReceived() {
  return car.getCoolantTemp()      != startValue
      && car.getOutdoorTemp()      != startValue
      && car.getBatteryVoltage()   != startValue
      && car.getClimateFanSpeed()  != startValue
      && car.getClimateLeftTemp()  != startValue
      && car.getClimateRightTemp() != startValue;
}

bool isCarReady() {
  return car.getClimateFanSpeed()  >= minClimateFanSpeed
      && car.getBatteryVoltage()   >= minBatteryVoltage
      && car.getClimateLeftTemp()  >= minClimateLeftTemp
      && car.getClimateRightTemp() >= minClimateRightTemp;
}

bool isHeaterMustBeOn() {
  return car.getOutdoorTemp() <= outdoorTempValues[0] && car.getCoolantTemp() < coolantTempValuesToOn[0]
      || car.getOutdoorTemp() <= outdoorTempValues[1] && car.getCoolantTemp() < coolantTempValuesToOn[1]
      || car.getOutdoorTemp() <= outdoorTempValues[2] && car.getCoolantTemp() < coolantTempValuesToOn[2]
      || car.getOutdoorTemp() <= outdoorTempValues[3] && car.getCoolantTemp() < coolantTempValuesToOn[3];
}

bool isHeaterMustBeOff() {
  return car.getOutdoorTemp() <= outdoorTempValues[0] && car.getCoolantTemp() >= coolantTempValuesToOff[0]
      || car.getOutdoorTemp() <= outdoorTempValues[1] && car.getCoolantTemp() >= coolantTempValuesToOff[1]
      || car.getOutdoorTemp() <= outdoorTempValues[2] && car.getCoolantTemp() >= coolantTempValuesToOff[2]
      || car.getOutdoorTemp() <= outdoorTempValues[3] && car.getCoolantTemp() >= coolantTempValuesToOff[3];
}

bool isClimateInDefrostMode() {
  return car.getBlowingWindshield() == true
      && car.getClimateLeftTemp()   > minClimateLeftTempWhenDefrost
      && car.getClimateRightTemp()  > minClimateRightTempWhenDefrost
      && car.getClimateFanSpeed()   >= minClimateFanSpeed
      && car.getRecyclingAir()      == false;
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

uint32_t getSecAfterStart() {
  return millis() / 1000ul;
}
