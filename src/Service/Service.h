#pragma once

#include "../Cluster/Constants.h"
#include "Constants.h"
#include "Event/MessageInterface.h"
#include "ServiceInterface.h"
#include "../Cluster/General/ClusterGeneral.h"

namespace Service
{
    class Service : public ServiceInterface {
    public:
        Service(const uint64_t updateTime, Event::MessageInterface &messageListener) :
            mUpdateTime(updateTime), mDeltaTime(0U), mInitialized(false), mPreviousTime(0UL), mMinDeltaTime(10000UL),
            mMaxDeltaTime(0UL), mMessageListener(messageListener) {
        }

        ~Service() = default;

        Core::Status
        NeedUpdate(const uint64_t currentTime) const {
            return (((currentTime - this->mPreviousTime) >= this->mUpdateTime && this->mInitialized)
                        ? Core::Status::CORE_OK
                        : Core::Status::CORE_ERROR);
        }

        void
        SetNewUpdateTime(const uint64_t currentTime, const EServices serviceId) {
            this->mDeltaTime = abs(static_cast<uint16_t>(static_cast<int64_t>(currentTime) -
                                                         static_cast<int64_t>(this->mPreviousTime) -
                                                         static_cast<int64_t>(this->mUpdateTime)));

            if (this->mPreviousTime == 0U) {
                this->mDeltaTime = 0U;
                this->mPreviousTime = currentTime;
                return;
            }
            this->mPreviousTime = currentTime;
            if (this->mDeltaTime < this->mMinDeltaTime) {
                this->SetMinTime(this->mDeltaTime);
                Frame response;
                General::ClusterGeneral::BuildFrameGetMinTime(serviceId, this->mDeltaTime, response);
                this->SendMessage(response);
            } else if (this->mDeltaTime > this->mMaxDeltaTime) {
                this->SetMaxTime(this->mDeltaTime);
                Frame response;
                General::ClusterGeneral::BuildFrameGetMaxTime(serviceId, this->mDeltaTime, response);
                this->SendMessage(response);
            }
        }

        uint64_t
        GetPreviousTime(void) const {
            return (this->mPreviousTime);
        }

        uint16_t
        GetDeltaTime(void) const {
            return (this->mDeltaTime);
        }

        uint16_t
        GetMinTime(void) const {
            return (this->mMinDeltaTime);
        }

        uint16_t
        GetMaxTime(void) const {
            return (this->mMaxDeltaTime);
        }

        void
        ResetTiming(void) {
            this->mMinDeltaTime = 10000UL;
            this->mMaxDeltaTime = 0U;
        }

        void
        SetMinTime(const uint16_t time) {
            this->mMinDeltaTime = time;
        }

        void
        SetMaxTime(const uint16_t time) {
            this->mMaxDeltaTime = time;
        }

    protected:
        void
        SendMessage(const Frame &message) const {
            this->mMessageListener.SendMessage(message);
        }

        volatile uint64_t mUpdateTime;
        volatile uint64_t mDeltaTime;
        bool mInitialized;

    private:
        volatile uint64_t mPreviousTime;
        volatile uint64_t mMinDeltaTime;
        volatile uint64_t mMaxDeltaTime;
        Event::MessageInterface &mMessageListener;
    };
} // namespace Service
