#ifndef HEATER_CONFIG
#define HEATER_CONFIG

class HeaterConfig {

    public:
        // Необходимость в дополнительном нагреве
        void setNeedToHeat(bool needToHeat);
        // Количество секунд до разрешения включения нагревателя после запуска
        void setSecAfterStartBeforeHeaterEnabled (int8_t seconds);
        // Минимальное напряжение АКБ для разрешения включения нагревателя
        void setMinBatteryVoltage(double minBatteryVoltage);
        // Минимальная скорость вентилятора климатической установки для разрешения включения нагревателя
        void setMinClimateFanSpeed(uint8_t minClimateFanSpeed);
        // Минимальная температура левой зоны климатической установки для разрешения включения нагревателя
        void setMinClimateLeftTemp(uint8_t minClimateLeftTemp);
        // Минимальная температура правой зоны климатической установки для разрешения включения нагревателя
        void setMinClimateRightTemp(uint8_t minClimateRightTemp);
        // Минимальная температура левой зоны климатической установки в режиме обдува ветрового стекла для разрешения включения нагревателя
        void setMinClimateLeftTempWhenDefrost(uint8_t minClimateLeftTempWhenDefrost);
        // Минимальная температура правой зоны климатической установки в режиме обдува ветрового стекла для разрешения включения нагревателя
        void setMinClimateRightTempWhenDefrost(uint8_t minClimateRightTempWhenDefrost);
        // Таблица значений температуры охлаждающей жидкости для отключения нагрева
        void setCoolantTempValuesToOff(uint8_t coolantTempValuesToOff[4]);
        // Таблица значений температуры охлаждающей жидкости для включения нагрева
        void setCoolantTempValuesToOn(uint8_t coolantTempValuesToOn[4]);
        // Таблица значений температуры наружного воздуха для отключения нагрева
        void setOutdoorTempValues(int8_t outdoorTempValues[4]);

        // Необходимость в дополнительном нагреве
        bool getNeedToHeat();
        // Количество секунд до разрешения включения нагревателя после запуска
        int8_t getSecAfterStartBeforeHeaterEnabled();
        // Значение по умолчанию для измеряемых параметров
        int8_t getStartValue();
        // Минимальное напряжение АКБ для разрешения включения нагревателя
        double getMinBatteryVoltage();
        // Минимальная скорость вентилятора климатической установки для разрешения включения нагревателя
        uint8_t getMinClimateFanSpeed();
        // Минимальная температура левой зоны климатической установки для разрешения включения нагревателя
        uint8_t getMinClimateLeftTemp();
        // Минимальная температура правой зоны климатической установки для разрешения включения нагревателя
        uint8_t getMinClimateRightTemp();
        // Минимальная температура левой зоны климатической установки в режиме обдува ветрового стекла для разрешения включения нагревателя
        uint8_t getMinClimateLeftTempWhenDefrost();
        // Минимальная температура правой зоны климатической установки в режиме обдува ветрового стекла для разрешения включения нагревателя
        uint8_t getMinClimateRightTempWhenDefrost();
        // Таблица значений температуры охлаждающей жидкости для отключения нагрева
        uint8_t* getCoolantTempValuesToOff();
        // Таблица значений температуры охлаждающей жидкости для включения нагрева
        uint8_t* getCoolantTempValuesToOn();
        // Таблица значений температуры наружного воздуха для отключения нагрева
        int8_t* getOutdoorTempValues();

    private:
        uint8_t _secAfterStartBeforeHeaterEnabled {30};
        bool _needToHeat {false};
        const int _startValue {-99};
        double _minBatteryVoltage {13.5};
        uint8_t _minClimateFanSpeed {3};
        uint8_t _minClimateLeftTemp {5};
        uint8_t _minClimateRightTemp {5};
        uint8_t _minClimateLeftTempWhenDefrost {16};
        uint8_t _minClimateRightTempWhenDefrost {16};
        uint8_t _coolantTempValuesToOff[4] {80, 75, 70, 65};
        uint8_t _coolantTempValuesToOn[4] {65, 60, 55, 50};
        int8_t _outdoorTempValues[4] {-20, -10, 0, 10};

};
extern HeaterConfig heaterConfig;
#endif
