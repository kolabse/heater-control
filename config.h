#ifndef HEATER_CONFIG
#define HEATER_CONFIG

class HeaterConfig {
    public:

        // Назначение пинов и адресов
        const int heaterKey1    {4};          // Пин подключения реле нагревателя №1
        const int lcdSwitch     {5};          // Пин переключателя наличия LCD дисплея
        const int heaterSwitch  {6};          // Пин переключателя наличия нагревателя
        const int heaterKey2    {7};          // Пин подключения реле нагревателя №2
        const int debugSwitch   {9};          // Пин переключателя включения режима отправки отладочной информации в COM-порт
        const int mcpScPin      {10};
        const int lcdI2CAddr    {0x3F};

        // Установка значений по умолчанию
        const int startValue    {-99};

        // Объявление переменных и значений по умолчанию для них
        uint8_t secAfterStartBeforeHeaterEnabled {30};
        uint8_t startlogoActiveSec {4};
        uint8_t lcdUpdateIntervalSec {5};
        uint8_t canIsDisconnectTimeout {5};

        uint32_t secAfterStartWhenLcdLastUpdate {0};                      // Время в секундах после того как последний раз были обновлены показания экрана
        uint32_t secAfterStartWhenLastCanMessageReceived {startValue};    // Время в секундах после того как было получено последнее сообщение из шины
        uint32_t serialBaudRate {115200};

        double systemCheckFrequency {0.20};

        bool logoIsActive   {true};                                         // Заставка активна
        bool needToHeat     {false};                                        // Необходимость в дополнительном нагреве
        bool LCDEnabled     {false};
        bool HeaterEnabled  {false};
        bool DebugMode      {false};

        // climate system values ranges for heater - диапозоны параметров климатической установки для работы отопителя

        double minBatteryVoltage {13.5};

        uint8_t minClimateFanSpeed {3};
        uint8_t minClimateLeftTemp {5};
        uint8_t minClimateRightTemp {5};
        uint8_t minClimateLeftTempWhenDefrost {16};
        uint8_t minClimateRightTempWhenDefrost {16};
        int8_t outdoorTempValues[4] {-20, -10, 0, 10};
        uint8_t coolantTempValuesToOff[4] {80, 75, 70, 65};
        uint8_t coolantTempValuesToOn[4] {65, 60, 55, 50};

};
extern HeaterConfig heaterConfig;
#endif
