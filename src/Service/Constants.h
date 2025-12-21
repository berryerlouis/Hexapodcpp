#pragma once

#include <stdint.h>
#include <string>

namespace Service
{
    enum EServices {
        BATTERY = 0U,
        CONTROL = 1U,
        COMMUNICATION = 2U,
        DISPLAY = 3U,
        GENERAL = 4U,
        ORIENTATION = 5U,
        PROXIMITY = 6U,
        BODY = 7U,
        BUTTON = 8U,
        SOUND = 9U,
        NONE = 10U
    };

    static constexpr uint8_t NB_SERVICES = 10U;

    class EServicesStruct {
    public:
        static std::string ServiceIdToString(const EServices serviceId) {
            switch (serviceId) {
                case BATTERY: return "BATTERY";
                case CONTROL: return "CONTROL";
                case COMMUNICATION: return "COMMUNICATION";
                case DISPLAY: return "DISPLAY";
                case GENERAL: return "GENERAL";
                case ORIENTATION: return "ORIENTATION";
                case PROXIMITY: return "PROXIMITY";
                case BODY: return "BODY";
                case BUTTON: return "BUTTON";
                case SOUND: return "SOUND";
                default: return "UNKNOWN";
            }
        }
    };
} // namespace Service
