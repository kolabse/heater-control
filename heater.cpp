#include <Arduino.h>
#include "heater.h"

Heater::Heater(uint8_t heater_key_1, uint8_t heater_key_2) {
    _heater_key_1 = heater_key_1;
    _heater_key_2 = heater_key_2;
    _heatingIntensity = 0;
    _needToHeat = false;
    // Устанавливаем контакты включения нагревателя в режим выхода
    pinMode(_heater_key_1, OUTPUT);
    pinMode(_heater_key_2, OUTPUT);

    // Устанавливаем низкий уровень на выходах включения нагрева
    digitalWrite(_heater_key_1, LOW);
    digitalWrite(_heater_key_2, LOW);
}

void Heater::heaterControl(AllCars &car) {
    // Проверяем достаточно ли прошло времени с момента запуска двигателя
    // Проверяем полученны ли все необходимые данные
    if (car.getSecAfterStart() > heaterConfig.getSecAfterStartBeforeHeaterEnabled() and isAllNecessaryDataReceived(car)) {
        if (isCarReady(car)) {
            // Если подогрев выключен, проверяем не находимся ли мы в условиях подходящих для включения
            if (_heater_state == _heater_state_idle && isHeaterMustBeOn(car)) {
                heaterConfig.setNeedToHeat(true);
            // Если подогрев включен, проверяем не находимся ли мы в условиях подходящих для отключения
            } else if (_heater_state == _heater_state_heat  && isHeaterMustBeOff(car)) {
                heaterConfig.setNeedToHeat(false);
            }
        } else if (heaterConfig.getNeedToHeat() == false && isClimateInDefrostMode(car)) {
            heaterConfig.setNeedToHeat(true);
        } else {
            heaterConfig.setNeedToHeat(false);
        }

        if (_needToHeat != heaterConfig.getNeedToHeat()) {
            if (_needToHeat) {
                _secAfterStartWhenNeedToHeatIsTrue = car.getSecAfterStart();
            } else {
                _secAfterStartWhenNeedToHeatIsFalse = car.getSecAfterStart();
            }
            _needToHeat = heaterConfig.getNeedToHeat();
        }
        if (_needToHeat) {
            _heater_state = _heater_state_heat;
            _secAfterNeedToHeatIsTrue = car.getSecAfterStart() - _secAfterStartWhenNeedToHeatIsTrue;
            if (_secAfterNeedToHeatIsTrue <= 10) {
                setHeatingIntensity(_300W);
            } else if (_secAfterNeedToHeatIsTrue > 10 and _secAfterNeedToHeatIsTrue <= 20) {
                setHeatingIntensity(_600W);
            } else {
                setHeatingIntensity(_900W);
            }
        } else {
            _secAfterNeedToHeatIsFalse = car.getSecAfterStart() - _secAfterStartWhenNeedToHeatIsFalse;
            if (_secAfterNeedToHeatIsFalse <= 10) {
                setHeatingIntensity(_600W);
            } else if (_secAfterNeedToHeatIsFalse > 10 and _secAfterNeedToHeatIsFalse <= 20) {
                setHeatingIntensity(_300W);
            } else {
                setHeatingIntensity(_000W);
                _heater_state = _heater_state_idle;
            }
        }
    }
}

uint8_t Heater::getHeatingIntensity() { return _heatingIntensity; }

void Heater::setHeatingIntensity(uint8_t intensity) {

    switch (intensity)
    {
    case 1:
        digitalWrite(_heater_key_1, HIGH);
        digitalWrite(_heater_key_2, LOW);
        _heatingIntensity = 1;
        break;
    case 2:
        digitalWrite(_heater_key_1, LOW);
        digitalWrite(_heater_key_2, HIGH);
        _heatingIntensity = 2;
        break;
    case 3:
        digitalWrite(_heater_key_1, HIGH);
        digitalWrite(_heater_key_2, HIGH);
        _heatingIntensity = 3;
        break;

    default:
        digitalWrite(_heater_key_1, LOW);
        digitalWrite(_heater_key_2, LOW);
        _heatingIntensity = 0;
        break;
    }
}

void Heater::emergencyStop(){
    digitalWrite(_heater_key_1, LOW);
    digitalWrite(_heater_key_2, LOW);
    _heatingIntensity = 0;
}

bool Heater::isAllNecessaryDataReceived(AllCars &car) {
  return car.getCoolantTemp()      != heaterConfig.getStartValue()
      && car.getOutdoorTemp()      != heaterConfig.getStartValue()
      && car.getBatteryVoltage()   != heaterConfig.getStartValue()
      && car.getClimateFanSpeed()  != heaterConfig.getStartValue()
      && car.getClimateLeftTemp()  != heaterConfig.getStartValue()
      && car.getClimateRightTemp() != heaterConfig.getStartValue();
}

bool Heater::isCarReady(AllCars &car) {
  return car.getClimateFanSpeed()  >= heaterConfig.getMinClimateFanSpeed()
      && car.getBatteryVoltage()   >= heaterConfig.getMinBatteryVoltage()
      && car.getClimateLeftTemp()  >= heaterConfig.getMinClimateLeftTemp()
      && car.getClimateRightTemp() >= heaterConfig.getMinClimateRightTemp();
}

bool Heater::isHeaterMustBeOn(AllCars &car) {
  return car.getOutdoorTemp() <= heaterConfig.getOutdoorTempValues()[0] && car.getCoolantTemp() < heaterConfig.getCoolantTempValuesToOn()[0]
      || car.getOutdoorTemp() <= heaterConfig.getOutdoorTempValues()[1] && car.getCoolantTemp() < heaterConfig.getCoolantTempValuesToOn()[1]
      || car.getOutdoorTemp() <= heaterConfig.getOutdoorTempValues()[2] && car.getCoolantTemp() < heaterConfig.getCoolantTempValuesToOn()[2]
      || car.getOutdoorTemp() <= heaterConfig.getOutdoorTempValues()[3] && car.getCoolantTemp() < heaterConfig.getCoolantTempValuesToOn()[3];
}

bool Heater::isHeaterMustBeOff(AllCars &car) {
  return car.getOutdoorTemp() <= heaterConfig.getOutdoorTempValues()[0] && car.getCoolantTemp() >= heaterConfig.getCoolantTempValuesToOff()[0]
      || car.getOutdoorTemp() <= heaterConfig.getOutdoorTempValues()[1] && car.getCoolantTemp() >= heaterConfig.getCoolantTempValuesToOff()[1]
      || car.getOutdoorTemp() <= heaterConfig.getOutdoorTempValues()[2] && car.getCoolantTemp() >= heaterConfig.getCoolantTempValuesToOff()[2]
      || car.getOutdoorTemp() <= heaterConfig.getOutdoorTempValues()[3] && car.getCoolantTemp() >= heaterConfig.getCoolantTempValuesToOff()[3];
}

bool Heater::isClimateInDefrostMode(AllCars &car) {
  return car.getBlowingWindshield() == true
      && car.getClimateLeftTemp()   > heaterConfig.getMinClimateLeftTempWhenDefrost()
      && car.getClimateRightTemp()  > heaterConfig.getMinClimateRightTempWhenDefrost()
      && car.getClimateFanSpeed()   >= heaterConfig.getMinClimateFanSpeed()
      && car.getRecyclingAir()      == false;
}
