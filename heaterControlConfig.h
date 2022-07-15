#ifndef HEATER_CONTROL_CONFIG
#define HEATER_CONTROL_CONFIG

class HeaterControlConfig {
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

        uint8_t startlogoActiveSec {4};
        uint8_t lcdUpdateIntervalSec {5};
        uint8_t canIsDisconnectTimeout {5};
        uint32_t secAfterStartWhenLcdLastUpdate {0};                      // Время в секундах после того как последний раз были обновлены показания экрана
        uint32_t secAfterStartWhenLastCanMessageReceived {startValue};    // Время в секундах после того как было получено последнее сообщение из шины
        uint32_t serialBaudRate {115200};

        double systemCheckFrequency {0.20};

        bool logoIsActive   {true};                                       // Заставка активна
        bool LCDEnabled     {false};
        bool HeaterEnabled  {false};
        bool DebugMode      {false};

};

extern HeaterControlConfig heaterControlConfig;
#endif