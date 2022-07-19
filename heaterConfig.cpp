#include <Arduino.h>
#include "heaterConfig.h"

void HeaterConfig::setNeedToHeat(bool needToHeat) { _needToHeat = needToHeat; }
void HeaterConfig::setSecAfterStartBeforeHeaterEnabled (int8_t seconds) { _secAfterStartBeforeHeaterEnabled = seconds; }
void HeaterConfig::setMinBatteryVoltage(double minBatteryVoltage) { _minBatteryVoltage = minBatteryVoltage; }
void HeaterConfig::setMinClimateFanSpeed(uint8_t minClimateFanSpeed) { _minClimateFanSpeed = minClimateFanSpeed; }
void HeaterConfig::setMinClimateLeftTemp(uint8_t minClimateLeftTemp) { _minClimateLeftTemp = minClimateLeftTemp; }
void HeaterConfig::setMinClimateRightTemp(uint8_t minClimateRightTemp) { _minClimateRightTemp = minClimateRightTemp; }
void HeaterConfig::setMinClimateLeftTempWhenDefrost(uint8_t minClimateLeftTempWhenDefrost) { _minClimateLeftTempWhenDefrost = minClimateLeftTempWhenDefrost; }
void HeaterConfig::setMinClimateRightTempWhenDefrost(uint8_t minClimateRightTempWhenDefrost) { _minClimateRightTempWhenDefrost = minClimateRightTempWhenDefrost; }

void HeaterConfig::setCoolantTempValuesToOff(uint8_t coolantTempValuesToOff[4]) {
    for (int i = 0; i < 4; ++i) {
        _coolantTempValuesToOff[i] = coolantTempValuesToOff[i];
    }
}

void HeaterConfig::setCoolantTempValuesToOn(uint8_t coolantTempValuesToOn[4]) {
    for (int i = 0; i < 4; ++i) {
        _coolantTempValuesToOn[i] = coolantTempValuesToOn[i];
    }
}

void HeaterConfig::setOutdoorTempValues(int8_t outdoorTempValues[4]) {
    for (int i = 0; i < 4; ++i) {
        _outdoorTempValues[i] = outdoorTempValues[i];
    }
}

bool HeaterConfig::getNeedToHeat() { return _needToHeat; }
int8_t HeaterConfig::getSecAfterStartBeforeHeaterEnabled() { return _secAfterStartBeforeHeaterEnabled; }
int8_t HeaterConfig::getStartValue() { return _startValue; }
double HeaterConfig::getMinBatteryVoltage() { return _minBatteryVoltage; }
uint8_t HeaterConfig::getMinClimateFanSpeed() { return _minClimateFanSpeed; }
uint8_t HeaterConfig::getMinClimateLeftTemp() { return _minClimateLeftTemp; }
uint8_t HeaterConfig::getMinClimateRightTemp() {return _minClimateRightTemp; }
uint8_t HeaterConfig::getMinClimateLeftTempWhenDefrost() { return _minClimateLeftTempWhenDefrost; }
uint8_t HeaterConfig::getMinClimateRightTempWhenDefrost() { return _minClimateRightTempWhenDefrost; }
uint8_t* HeaterConfig::getCoolantTempValuesToOff() { return _coolantTempValuesToOff; }
uint8_t* HeaterConfig::getCoolantTempValuesToOn() { return _coolantTempValuesToOn; }
int8_t* HeaterConfig::getOutdoorTempValues() { return _outdoorTempValues; }

HeaterConfig heaterConfig = HeaterConfig();
