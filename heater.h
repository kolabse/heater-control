#ifndef HEATER_CONTROL
#define HEATER_CONTROL

#include <Arduino.h>
#include <AllCars.h>
#include "heaterConfig.h"

class Heater {
    public:
        Heater(uint8_t heater_key_1, uint8_t heater_key_2);
        void heaterControl(AllCars &car);
        void emergencyStop();
        uint8_t getHeatingIntensity();

    private:
        uint8_t _heater_key_1;
        uint8_t _heater_key_2;
        uint8_t _heatingIntensity;                        // Ступень работы дополнительного нагревателя
        bool _needToHeat;                                 // Необходимость в дополнительном нагреве
        uint32_t _secAfterStartWhenNeedToHeatIsTrue {0};  // Время в секундах после того как появилась необходимость включения дополнительного нагрева
        uint32_t _secAfterStartWhenNeedToHeatIsFalse {0}; // Время в секундах после того как появилась необходимость выключения дополнительного нагрева
        uint32_t _secAfterNeedToHeatIsTrue {0};           // Время в секундах после того как дополнительный нагрев был включен
        uint32_t _secAfterNeedToHeatIsFalse {0};          // Время в секундах после того как дополнительный нагрев был выключен

        uint8_t _000W {0};
        uint8_t _300W {1};
        uint8_t _600W {2};
        uint8_t _900W {3};

        uint8_t _heater_state_idle {0};
        uint8_t _heater_state_heat {1};

        uint8_t _heater_state {_heater_state_idle};

        void setHeatingIntensity(uint8_t intensity);
        bool isAllNecessaryDataReceived(AllCars &car);
        bool isCarReady(AllCars &car);
        bool isHeaterMustBeOn(AllCars &car);
        bool isHeaterMustBeOff(AllCars &car);
        bool isClimateInDefrostMode(AllCars &car);
};

#endif
