#pragma once

#include <any>
#include <typeinfo>
#include "../../Core/Logger.h"
#include "../Constants.h"

namespace Service
{
#define LOG_EVENT_DEBUG(...) LOG_GENERIC(DEBUG, EVENT, __VA_ARGS__)
#define LOG_EVENT_INFO(...) LOG_GENERIC(INFO, EVENT, __VA_ARGS__)
#define LOG_EVENT_WARNING(...) LOG_GENERIC(WARNING, EVENT, __VA_ARGS__)
#define LOG_EVENT_ERROR(...) LOG_GENERIC(ERROR, EVENT, __VA_ARGS__)

    enum EventType {
        EVENT_INIT_UPDATE,
        EVENT_BUTTON_UPDATE,
        EVENT_COM_UPDATE,
        EVENT_BATTERY_UPDATE,
        EVENT_SENSOR_UPDATE,
        EVENT_SOUND_UPDATE
    };

    namespace Event
    {
        class Event {
        public:
            EventType eventType;
            EServices serviceId;
            std::any  eventArg;
            Event(const EServices serviceId,
                  const EventType eventType,
                  const std::any &eventArg) {
                this->serviceId = serviceId;
                this->eventType = eventType;
                this->eventArg = eventArg;
            }
            static std::string EventTypeToString(const EventType eventType) {
                switch (eventType) {
                    case EVENT_INIT_UPDATE: return "EVENT_INIT_UPDATE";
                    case EVENT_BUTTON_UPDATE: return "EVENT_BUTTON_UPDATE";
                    case EVENT_COM_UPDATE: return "EVENT_COM_UPDATE";
                    case EVENT_BATTERY_UPDATE: return "EVENT_BATTERY_UPDATE";
                    case EVENT_SENSOR_UPDATE: return "EVENT_SENSOR_UPDATE";
                    case EVENT_SOUND_UPDATE: return "EVENT_SOUND_UPDATE";
                    default: return "UNKNOWN";
                }
            }
            bool operator==(const Event &other) const {
                return (this->serviceId == other.serviceId && this->eventType == other.eventType &&
                        this->eventArg.type() == other.eventArg.type());
            }
        };
    } // namespace Event
} // namespace Service
