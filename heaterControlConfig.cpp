#include <Arduino.h>
#include "heaterControlConfig.h"

void HeaterControlConfig::setHeaterKey1(uint8_t heaterKey1) {
    _heaterKey1 = heaterKey1;
}
void HeaterControlConfig::setLcdSwitch(uint8_t lcdSwitch) {
    _lcdSwitch = lcdSwitch;
}
void HeaterControlConfig::setHeaterSwitch(uint8_t heaterSwitch) {
    _heaterSwitch = heaterSwitch;
}
void HeaterControlConfig::setHeaterKey2(uint8_t heaterKey2) {
    _heaterKey2 = heaterKey2;
}
void HeaterControlConfig::setDebugSwitch(uint8_t debugSwitch) {
    _debugSwitch = debugSwitch;
}

void HeaterControlConfig::setMcpScPin(uint8_t mcpScPin) {
    _mcpScPin = mcpScPin;
}
void HeaterControlConfig::setLcdI2CAddr(uint8_t lcdI2CAddr) {
    _lcdI2CAddr = lcdI2CAddr;
}
void HeaterControlConfig::setStartlogoActiveSec(uint8_t startlogoActiveSec) {
    _startlogoActiveSec = startlogoActiveSec;
}
void HeaterControlConfig::setLcdUpdateIntervalSec(uint8_t lcdUpdateIntervalSec) {
    _lcdUpdateIntervalSec = lcdUpdateIntervalSec;
}
void HeaterControlConfig::setCanIsDisconnectTimeout(uint8_t canIsDisconnectTimeout) {
    _canIsDisconnectTimeout = canIsDisconnectTimeout;
}
void HeaterControlConfig::setSecAfterStartWhenLcdLastUpdate(uint32_t secAfterStartWhenLcdLastUpdate) {
    _secAfterStartWhenLcdLastUpdate = secAfterStartWhenLcdLastUpdate;
}
void HeaterControlConfig::setSecAfterStartWhenLastCanMessageReceived(uint32_t secAfterStartWhenLastCanMessageReceived) {
    _secAfterStartWhenLastCanMessageReceived = secAfterStartWhenLastCanMessageReceived;
}
void HeaterControlConfig::setSerialBaudRate(uint32_t serialBaudRate) {
    _serialBaudRate = serialBaudRate;
}
void HeaterControlConfig::setSystemCheckFrequency(double systemCheckFrequency) {
    _systemCheckFrequency = systemCheckFrequency;
}
void HeaterControlConfig::setLogoIsActive (bool logoIsActive) {
    _logoIsActive = logoIsActive;
}
void HeaterControlConfig::setLCDEnabled (bool LCDEnabled) {
    _LCDEnabled = LCDEnabled;
}
void HeaterControlConfig::setHeaterEnabled (bool heaterEnabled) {
    _heaterEnabled = heaterEnabled;
}
void HeaterControlConfig::setDebugMode  (bool debugMode) {
    _debugMode = debugMode;
}

uint8_t HeaterControlConfig::getHeaterKey1() {return _heaterKey1; }
uint8_t HeaterControlConfig::getLcdSwitch() {return _lcdSwitch; }
uint8_t HeaterControlConfig::getHeaterSwitch() {return _heaterSwitch; }
uint8_t HeaterControlConfig::getHeaterKey2() {return _heaterKey2; }
uint8_t HeaterControlConfig::getDebugSwitch() {return _debugSwitch; }
uint8_t HeaterControlConfig::getMcpScPin() {return _mcpScPin; }
uint8_t HeaterControlConfig::getLcdI2CAddr() {return _lcdI2CAddr; }
uint8_t HeaterControlConfig::getStartValue() {return _startValue; }
uint8_t HeaterControlConfig::getStartlogoActiveSec() {return _startlogoActiveSec; }
uint8_t HeaterControlConfig::getLcdUpdateIntervalSec() {return _lcdUpdateIntervalSec; }
uint8_t HeaterControlConfig::getCanIsDisconnectTimeout() {return _canIsDisconnectTimeout; }
uint32_t HeaterControlConfig::getSecAfterStartWhenLcdLastUpdate() {return _secAfterStartWhenLcdLastUpdate; }
uint32_t HeaterControlConfig::getSecAfterStartWhenLastCanMessageReceived() {return _secAfterStartWhenLastCanMessageReceived; }
uint32_t HeaterControlConfig::getSerialBaudRate() {return _serialBaudRate; }
double HeaterControlConfig::getSystemCheckFrequency() {return _systemCheckFrequency; }
bool HeaterControlConfig::getLogoIsActive() {return _logoIsActive; }
bool HeaterControlConfig::getLCDEnabled() {return _LCDEnabled; }
bool HeaterControlConfig::getHeaterEnabled() {return _heaterEnabled; }
bool HeaterControlConfig::getDebugMode() {return _debugMode; }

HeaterControlConfig heaterControlConfig = HeaterControlConfig();
