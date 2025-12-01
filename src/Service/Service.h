#pragma once

#include "Constants.h"
#include "Event/MessageInterface.h"
#include "ServiceInterface.h"
#include "../Cluster/General/ClusterGeneral.h"
#include "../Driver/Tick/Tick.h"

namespace Service
{
    class Service : public ServiceInterface {
    public:
        Service(const EServices serviceId, const uint64_t updateTime, Event::MessageInterface &messageListener) :
            mUpdateTime(updateTime)
            , mDeltaTime(0U)
            , mInitialized(false)
            , mServiceId(serviceId)
            , mPreviousTime(0UL)
            , mMinDeltaTime(10000UL)
            , mMaxDeltaTime(0UL)
            , mMessageListener(messageListener) {
            LOG_SERVICE_DEBUG("%s(%d) each %dms.",
                              EServicesStruct::ServiceIdToString(serviceId).c_str(), serviceId, updateTime);
        }

        ~Service() = default;

        void
        UpdateService(const uint64_t currentTime) {
            if (this->NeedUpdate(currentTime) == Core::Status::CORE_OK) {
                this->Update(currentTime);
                this->SetNewUpdateTime(Driver::Tick::Tick::GetInstance().GetMs());
            }
        }

        Core::Status
        NeedUpdate(const uint64_t currentTime) const {
            return (((currentTime - this->mPreviousTime) >= this->mUpdateTime && this->mInitialized)
                        ? Core::Status::CORE_OK
                        : Core::Status::CORE_ERROR);
        }

        void
        SetNewUpdateTime(const uint64_t currentTime) {
            this->mDeltaTime = abs(static_cast<int64_t>(currentTime) -
                                   static_cast<int64_t>(this->mPreviousTime) -
                                   static_cast<int64_t>(this->mUpdateTime));

            if (this->mPreviousTime == 0U) {
                this->mDeltaTime = 0U;
                this->mPreviousTime = currentTime;
                return;
            }
            this->mPreviousTime = currentTime;
            if (this->mDeltaTime < this->mMinDeltaTime) {
                this->SetMinTime(this->mDeltaTime);
                Frame response;
                Cluster::General::ClusterGeneral::BuildFrameGetMinTime(this->mServiceId, this->mDeltaTime, response);
                this->SendMessage(response);
            } else if (this->mDeltaTime > this->mMaxDeltaTime) {
                this->SetMaxTime(this->mDeltaTime);
                Frame response;
                Cluster::General::ClusterGeneral::BuildFrameGetMaxTime(this->mServiceId, this->mDeltaTime, response);
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
        SetMinTime(const uint64_t time) {
            this->mMinDeltaTime = time;
        }

        void
        SetMaxTime(const uint64_t time) {
            this->mMaxDeltaTime = time;
        }

        EServices
        GetServiceId(void) const {
            return this->mServiceId;
        }

    protected:
        void
        SendMessage(const Frame &message) const {
            this->mMessageListener.SendMessage(message);
        }

        volatile uint64_t mUpdateTime;
        volatile uint64_t mDeltaTime;
        bool mInitialized;
        EServices mServiceId;

    private:
        volatile uint64_t mPreviousTime;
        volatile uint64_t mMinDeltaTime;
        volatile uint64_t mMaxDeltaTime;
        Event::MessageInterface &mMessageListener;
    };
} // namespace Service
