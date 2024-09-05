#pragma once

#include "ServiceInterface.h"
#include "../Core/Event/EventMediatorInterface.h"
#include "../Misc/Logger/Logger.h"

namespace Service
{
    class Service : public ServiceInterface, public Core::EventManager {
    public:
        Service(const uint64_t updateTime)
            : mUpdateTime(updateTime)
              , mDeltaTime(0U)
              , mPreviousTime(0UL) {
        }

        ~Service() = default;

        Core::CoreStatus NeedUpdate(const uint64_t currentTime) const {
            return (((currentTime - this->mPreviousTime) >= this->mUpdateTime)
                        ? Core::CoreStatus::CORE_OK
                        : Core::CoreStatus::CORE_ERROR);
        }

        void SetNewUpdateTime(const uint64_t currentTime) {
            this->mDeltaTime = currentTime - this->mPreviousTime;
            this->mPreviousTime = currentTime;
        }

        uint64_t GetPreviousTime(void) const {
            return (this->mPreviousTime);
        }

        uint64_t GetDeltaTime(void) const {
            return (this->mDeltaTime);
        }

    protected:
        uint64_t mUpdateTime;
        uint64_t mDeltaTime;

    private:
        uint64_t mPreviousTime;
    };
}
