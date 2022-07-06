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

void Heater::needToHeat(AllCars &car, bool needToHeat) {
    if (_needToHeat != needToHeat) {
        if (_needToHeat) {
            _secAfterStartWhenNeedToHeatIsTrue = car.getSecAfterStart();
        } else {
            _secAfterStartWhenNeedToHeatIsFalse = car.getSecAfterStart();
        }
        _needToHeat = needToHeat;
    }
    if (_needToHeat) {
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
