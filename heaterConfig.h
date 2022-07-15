#ifndef HEATER_CONFIG
#define HEATER_CONFIG

class HeaterConfig {
    public:

        // Установка значений по умолчанию
        const int startValue    {-99};

        // Объявление переменных и значений по умолчанию для них
        uint8_t secAfterStartBeforeHeaterEnabled {30};

        bool needToHeat     {false};                                        // Необходимость в дополнительном нагреве

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
