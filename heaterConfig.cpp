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

void HeaterConfig::setCoolantTempValuesToOff(uint8_t CoolantTempValuesToOff[4]) {
    _coolantTempValuesToOff[0] = CoolantTempValuesToOff[0];
    _coolantTempValuesToOff[1] = CoolantTempValuesToOff[1];
    _coolantTempValuesToOff[2] = CoolantTempValuesToOff[2];
    _coolantTempValuesToOff[3] = CoolantTempValuesToOff[3];
}

void HeaterConfig::setCoolantTempValuesToOn(uint8_t CoolantTempValuesToOn[4]) {
    _coolantTempValuesToOn[0] = CoolantTempValuesToOn[0];
    _coolantTempValuesToOn[1] = CoolantTempValuesToOn[1];
    _coolantTempValuesToOn[2] = CoolantTempValuesToOn[2];
    _coolantTempValuesToOn[3] = CoolantTempValuesToOn[3];
}

void HeaterConfig::setOutdoorTempValues(int8_t outdoorTempValues[4]) {
    _outdoorTempValues[0] = outdoorTempValues[0];
    _outdoorTempValues[1] = outdoorTempValues[1];
    _outdoorTempValues[2] = outdoorTempValues[2];
    _outdoorTempValues[3] = outdoorTempValues[3];
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
