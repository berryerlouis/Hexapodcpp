#pragma once

#include "ServiceInterface.h"
#include "../Cluster/Constants.h"
#include "../Misc/Logger/Logger.h"
#include "Event/EventDispatcherInterface.h"
#include "Event/EventListenerInterface.h"
#include "Constants.h"
#include "string.h"

namespace Service
{
    class Service : public ServiceInterface, public Event::EventDispatcherInterface {
    public:
        Service(const uint64_t updateTime, Event::EventListenerInterface &eventListener)
            : mUpdateTime(updateTime)
              , mDeltaTime(0U)
              , mPreviousTime(0UL)
              , mMinDeltaTime(0UL)
              , mMaxDeltaTime(0UL)
              , mEventListener(eventListener) {
        }

        ~Service() = default;

        Core::CoreStatus NeedUpdate(const uint64_t currentTime) const {
            return (((currentTime - this->mPreviousTime) >= this->mUpdateTime)
                        ? Core::CoreStatus::CORE_OK
                        : Core::CoreStatus::CORE_ERROR);
        }

        void SetNewUpdateTime(const uint64_t currentTime, const EServices serviceId) {
            this->mDeltaTime = currentTime - this->mPreviousTime;
            this->mPreviousTime = currentTime;
            if (this->mDeltaTime < this->mMinDeltaTime) {
                this->SetMinTime(this->mDeltaTime);
                const uint8_t arg[3U] = {
                    static_cast<uint8_t>(serviceId),
                    static_cast<uint8_t>(this->mDeltaTime >> 8),
                    static_cast<uint8_t>(this->mDeltaTime & 0xFF)
                };
                const SEvent ev(EServices::GENERAL, MIN_EXECUTION_TIME, arg, 3U);
                this->AddEvent(ev);
            } else if (this->mDeltaTime > this->mMaxDeltaTime) {
                this->SetMaxTime(this->mDeltaTime);
                const uint8_t arg[3U] = {
                    static_cast<uint8_t>(serviceId),
                    static_cast<uint8_t>(this->mDeltaTime >> 8),
                    static_cast<uint8_t>(this->mDeltaTime & 0xFF)
                };
                const SEvent ev(EServices::GENERAL, MAX_EXECUTION_TIME, arg, 3U);
                this->AddEvent(ev);
            }
        }

        uint64_t GetPreviousTime(void) const {
            return (this->mPreviousTime);
        }

        uint16_t GetDeltaTime(void) const {
            return (this->mDeltaTime);
        }

        uint16_t GetMinTime(void) const {
            return (this->mMinDeltaTime);
        }

        uint16_t GetMaxTime(void) const {
            return (this->mMaxDeltaTime);
        }

        void ResetTiming(void) {
            this->mMinDeltaTime = 0U;
            this->mMaxDeltaTime = 0U;
        }

        void SetMinTime(const uint16_t time) {
            this->mMinDeltaTime = time;
        }

        void SetMaxTime(const uint16_t time) {
            this->mMaxDeltaTime = time;
        }

        void AddEvent(const SEvent &event) const {
            this->mEventListener.AddEvent(event);
        }

        virtual void DispatchEvent(const SEvent &event) = 0;

    protected:
        uint64_t mUpdateTime;
        uint16_t mDeltaTime;

    private:
        uint64_t mPreviousTime;
        uint16_t mMinDeltaTime;
        uint16_t mMaxDeltaTime;
        Event::EventListenerInterface &mEventListener;
    };
}
