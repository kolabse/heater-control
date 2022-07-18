#ifndef HEATER_CONTROL_CONFIG
#define HEATER_CONTROL_CONFIG

class HeaterControlConfig {
    public:

        // Пин подключения реле нагревателя №1
        void setHeaterKey1(uint8_t heaterKey1);
        // Пин переключателя наличия LCD дисплея
        void setLcdSwitch(uint8_t lcdSwitch);
        // Пин переключателя наличия нагревателя
        void setHeaterSwitch(uint8_t heaterSwitch);
        // Пин подключения реле нагревателя №2
        void setHeaterKey2(uint8_t heaterKey2);
        // Пин переключателя включения режима отправки отладочной информации в COM-порт
        void setDebugSwitch(uint8_t debugSwitch);
        // Пин подключения входа SC MCP
        void setMcpScPin(uint8_t mcpScPin);
        // i2c адрес LCD дисплея
        void setLcdI2CAddr(uint8_t lcdI2CAddr);
        // Время активности старотовой заставки
        void setStartlogoActiveSec(uint8_t startlogoActiveSec);
        // Интервал обновления LCD-дисплея
        void setLcdUpdateIntervalSec(uint8_t lcdUpdateIntervalSec);
        // Время в секундах по истечении которого считается, что потеряна связь с CAN-шиной
        void setCanIsDisconnectTimeout(uint8_t canIsDisconnectTimeout);
        // Время в секундах после того как последний раз были обновлены показания экрана
        void setSecAfterStartWhenLcdLastUpdate(uint32_t secAfterStartWhenLcdLastUpdate);
        // Время в секундах после того как было получено последнее сообщение из шины
        void setSecAfterStartWhenLastCanMessageReceived(uint32_t secAfterStartWhenLastCanMessageReceived);
        // Скорость COM-порта
        void setSerialBaudRate(uint32_t serialBaudRate);
        // Частота запуска проверки необходимости включения/отключения нагреватедя
        void setSystemCheckFrequency(double systemCheckFrequency);
        // Заставка активна
        void setLogoIsActive (bool logoIsActive);
        // LCD-экран подключен
        void setLCDEnabled (bool LCDEnabled);
        // Нагреватель подключен
        void setHeaterEnabled (bool heaterEnabled);
        // Режим отладки
        void setDebugMode  (bool debugMode);

        // Пин подключения реле нагревателя №1
        uint8_t getHeaterKey1();
        // Пин переключателя наличия LCD дисплея
        uint8_t getLcdSwitch();
        // Пин переключателя наличия нагревателя
        uint8_t getHeaterSwitch();
        // Пин подключения реле нагревателя №2
        uint8_t getHeaterKey2();
        // Пин переключателя включения режима отправки отладочной информации в COM-порт
        uint8_t getDebugSwitch();
        // Пин подключения входа SC MCP
        uint8_t getMcpScPin();
        // i2c адрес LCD дисплея
        uint8_t getLcdI2CAddr();
        // Значение по умолчанию для получаемых данных
        uint8_t getStartValue();
        // Время активности старотовой заставки
        uint8_t getStartlogoActiveSec();
        // Интервал обновления LCD-дисплея
        uint8_t getLcdUpdateIntervalSec();
        // Время в секундах по истечении которого считается, что потеряна связь с CAN-шиной
        uint8_t getCanIsDisconnectTimeout();
        // Время в секундах после того как последний раз были обновлены показания экрана
        uint32_t getSecAfterStartWhenLcdLastUpdate();
        // Время в секундах после того как было получено последнее сообщение из шины
        uint32_t getSecAfterStartWhenLastCanMessageReceived();
        // Скорость COM-порта
        uint32_t getSerialBaudRate();
        // Частота запуска проверки необходимости включения/отключения нагреватедя
        double getSystemCheckFrequency();
        // Заставка активна
        bool getLogoIsActive();
        // LCD-экран подключен
        bool getLCDEnabled();
        // Нагреватель подключен
        bool getHeaterEnabled();
        // Режим отладки
        bool getDebugMode();


    private:
        uint8_t _heaterKey1    {4};          // Пин подключения реле нагревателя №1
        uint8_t _lcdSwitch     {5};          // Пин переключателя наличия LCD дисплея
        uint8_t _heaterSwitch  {6};          // Пин переключателя наличия нагревателя
        uint8_t _heaterKey2    {7};          // Пин подключения реле нагревателя №2
        uint8_t _debugSwitch   {9};          // Пин переключателя включения режима отправки отладочной информации в COM-порт
        uint8_t _mcpScPin      {10};
        uint8_t _lcdI2CAddr    {0x3F};

        const int _startValue  {-99};

        uint8_t _startlogoActiveSec {4};
        uint8_t _lcdUpdateIntervalSec {5};
        uint8_t _canIsDisconnectTimeout {5};
        uint32_t _secAfterStartWhenLcdLastUpdate {0};                      // Время в секундах после того как последний раз были обновлены показания экрана
        uint32_t _secAfterStartWhenLastCanMessageReceived {_startValue};    // Время в секундах после того как было получено последнее сообщение из шины
        uint32_t _serialBaudRate {115200};

        double _systemCheckFrequency {0.20};

        bool _logoIsActive   {true};                                       // Заставка активна
        bool _LCDEnabled     {false};
        bool _heaterEnabled  {false};
        bool _debugMode      {false};
};

extern HeaterControlConfig heaterControlConfig;
#endif
