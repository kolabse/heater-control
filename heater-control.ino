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

HeaterConfig heaterConfig;

// LCD дисплей
LiquidCrystal_I2C lcd(heaterConfig.lcdI2CAddr, 16, 2); // (адрес - определен через i2c_scanner, размеры дисплея)

// CAN модуль
MCP2515 mcp2515(heaterConfig.mcpScPin);

// Электрический радиатор (Цепь силового ключа 1, Цепь силового ключа 2, Цепь силового ключа 3)
Heater heater(heaterConfig.heaterKey1, heaterConfig.heaterKey2);

// Сообщение CAN шины
struct can_frame canMsg;

// Блок инициализации перед запуском основной программы
void setup() {

  // Читаем установки с переключателей
  pinMode(heaterConfig.lcdSwitch, INPUT_PULLUP);
  pinMode(heaterConfig.heaterSwitch, INPUT_PULLUP);
  pinMode(heaterConfig.debugSwitch, INPUT_PULLUP);

  heaterConfig.LCDEnabled = !digitalRead(heaterConfig.lcdSwitch);
  heaterConfig.HeaterEnabled = !digitalRead(heaterConfig.heaterSwitch);
  heaterConfig.DebugMode = !digitalRead(heaterConfig.debugSwitch);

  if(heaterConfig.LCDEnabled){
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
  Timer1.setFrequencyFloat(heaterConfig.systemCheckFrequency);
  // Запускаем таймер 1 аппаратного прерывания на канале "A" (порт D9)
  Timer1.enableISR(CHANNEL_A);

  // Настраиваем фильтрацию сообщений CAN-шины
  car.setCanFilters(mcp2515);

    if(heaterConfig.DebugMode) {
    // Запускаем интерфейс COM-порта
    Serial.begin(heaterConfig.serialBaudRate);

    Serial.print("LCDEnabled: ");
    Serial.println(heaterConfig.LCDEnabled);
    Serial.print("HeaterEnabled: ");
    Serial.println(heaterConfig.HeaterEnabled);

    Serial.println("------- CAN Read ----------");
    Serial.println("ID  DLC   0   1   2   3   4   5   6   7");
  }

}

ISR(TIMER1_A) {

  heater.heaterControl(car);

}

void loop() {

  car.setSecAfterStart(getSecAfterStart());

  if (heaterConfig.logoIsActive and car.getSecAfterStart() > heaterConfig.startlogoActiveSec and heaterConfig.LCDEnabled) {
    heaterConfig.logoIsActive = false;
    lcd.clear();
  }

  if (mcp2515.readMessage(&canMsg) == MCP2515::ERROR_OK) {

    heaterConfig.secAfterStartWhenLastCanMessageReceived = car.getSecAfterStart();
    if (!heaterConfig.logoIsActive and heaterConfig.LCDEnabled) {
      lcd.setCursor(10, 1);
      lcd.print("(V)");
    }

    if (heaterConfig.DebugMode) {
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

    if (heaterConfig.DebugMode) {
      // Вывод отладочной информации в Serial порт платы
      Serial.print(car.getSecAfterStart());
      Serial.print("s ");

      if (car.getBatteryVoltage() != heaterConfig.startValue) {
        Serial.print(car.getBatteryVoltage());
        Serial.print("v ");
      }

      if (car.getCoolantTemp() != heaterConfig.startValue) {
        Serial.print(car.getCoolantTemp());
        Serial.print("°C ");
      }

      if (car.getOutdoorTemp() != heaterConfig.startValue) {
        Serial.print(car.getOutdoorTemp());
        Serial.print("°C ");
      }

      if (car.getInstFuelCons() != heaterConfig.startValue) {
        Serial.print(car.getInstFuelCons());
        Serial.print("L/100km ");
      }

      if (car.getClimateFanSpeed() != heaterConfig.startValue) {
        Serial.print(car.getClimateFanSpeed());
        Serial.print("sp. ");
      }

      if (car.getClimateLeftTemp() != heaterConfig.startValue) {
        Serial.print(car.getClimateLeftTemp());
        Serial.print("°C ");
      }

      if (car.getClimateRightTemp() != heaterConfig.startValue) {
        Serial.print(car.getClimateRightTemp());
        Serial.print("°C ");
      }

      if (car.getRecyclingAir() != heaterConfig.startValue) {
        Serial.print("is_rc: ");
        Serial.print(car.getRecyclingAir());
      }

      if (car.getBlowingWindshield() != heaterConfig.startValue) {
        Serial.print(" is_bw: ");
        Serial.print(car.getBlowingWindshield());
      }

      Serial.print(" Need_to_heat: ");
      Serial.print(heaterConfig.needToHeat);

      Serial.print(" HeatingIntensity: ");
      Serial.print(heater.getHeatingIntensity());

      Serial.println();
    }
  }

  if (car.getSecAfterStart() - heaterConfig.secAfterStartWhenLcdLastUpdate > heaterConfig.lcdUpdateIntervalSec and heaterConfig.LCDEnabled) {

    heaterConfig.secAfterStartWhenLcdLastUpdate = car.getSecAfterStart();

    if (car.getBatteryVoltage() != heaterConfig.startValue) {
      lcd.setCursor(0, 0);
      lcd.print(String(car.getBatteryVoltage()));
      lcd.print("v");
    }

    if (car.getCoolantTemp() != heaterConfig.startValue) {
      lcd.setCursor(7, 0);
      lcd.print(valueWithOffset(car.getCoolantTemp(), true));
      lcd.print("C");
    }

    if (car.getOutdoorTemp() != heaterConfig.startValue) {
      lcd.setCursor(12, 0);
      lcd.print(valueWithOffset(car.getOutdoorTemp(), true));
      lcd.print("C");
    }

    if (car.getInstFuelCons() != heaterConfig.startValue) {
      lcd.setCursor(0, 1);
      lcd.print(valueWithOffset(car.getInstFuelCons(), false));
      lcd.print("L");
    }

    if (
      car.getCoolantTemp() != heaterConfig.startValue
      or car.getOutdoorTemp() != heaterConfig.startValue
      or car.getBatteryVoltage() != heaterConfig.startValue
      or car.getInstFuelCons() != heaterConfig.startValue
    ) {
      lcd.setCursor(15, 1);
      lcd.print(String(heater.getHeatingIntensity()));
    }
  }

  if (car.getSecAfterStart() - heaterConfig.secAfterStartWhenLastCanMessageReceived > heaterConfig.canIsDisconnectTimeout) {
    if(!heaterConfig.logoIsActive and heaterConfig.LCDEnabled) {
      lcd.setCursor(10, 1);
      lcd.print("(X)");
    }
    heater.emergencyStop();
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

uint32_t getSecAfterStart() {
  return millis() / 1000ul;
}
