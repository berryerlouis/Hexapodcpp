#pragma once

#include "../Cluster/General/ClusterGeneral.h"
#include "../Driver/Timer/Tick.h"
#include "Constants.h"
#include "Event/EventListener.h"
#include "Message/MessageInterface.h"
#include "ServiceInterface.h"

namespace Service
{
    class Service : public ServiceInterface {
    public:
        Service(const EServices                serviceId,
                const uint64_t                 updateTime,
                Message::MessageInterface     &messageListener,
                Event::EventListenerInterface &eventListener) :
            mUpdateTime(updateTime),
            mDeltaTime(0U),
            mInitialized(false),
            mServiceId(serviceId),
            mPreviousTime(0UL),
            mMinDeltaTime(10000UL),
            mMaxDeltaTime(0UL),
            mMessageListener(messageListener),
            mEventListener(eventListener) {
            LOG_SERVICE_DEBUG("%s(%d) each %dms.",
                              EServicesStruct::ServiceIdToString(serviceId).c_str(),
                              serviceId,
                              updateTime);
        }

        ~Service() = default;

        void SetEvent(const EServices               serviceId,
                      const Event::Event::EventType eventType) const {
            this->mEventListener.SetEvent(serviceId, eventType);
        }

        void SetEvent(const Event::Event::EventType eventType) const {
            this->mEventListener.SetEvent(this->GetServiceId(), eventType);
        }

        virtual void DispatchEvent(const Event::Event &event) const = 0;

        void         UpdateService(const uint64_t currentTime) {
            if (this->NeedUpdate(currentTime) == Core::Status::CORE_OK) {
                this->Update(currentTime);
                this->SetNewUpdateTime(Driver::Timer::Tick::GetInstance().GetMs());
            }
        }

        Core::Status NeedUpdate(const uint64_t currentTime) const {
            return ((currentTime - this->mPreviousTime) >= this->mUpdateTime && this->mInitialized)
                           ? Core::Status::CORE_OK
                           : Core::Status::CORE_ERROR;
        }

        void SetNewUpdateTime(const uint64_t currentTime) {
            this->mDeltaTime = abs(static_cast<int64_t>(currentTime) - static_cast<int64_t>(this->mPreviousTime) -
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

        uint64_t GetPreviousTime(void) const {
            return this->mPreviousTime;
        }

        uint16_t GetDeltaTime(void) const {
            return this->mDeltaTime;
        }

        uint16_t GetMinTime(void) const {
            return this->mMinDeltaTime;
        }

        uint16_t GetMaxTime(void) const {
            return this->mMaxDeltaTime;
        }

        void ResetTiming(void) {
            this->mMinDeltaTime = 10000UL;
            this->mMaxDeltaTime = 0U;
        }

        void SetMinTime(const uint64_t time) {
            this->mMinDeltaTime = time;
        }

        void SetMaxTime(const uint64_t time) {
            this->mMaxDeltaTime = time;
        }

        EServices GetServiceId(void) const {
            return this->mServiceId;
        }

    protected:
        void SendMessage(const Frame &message) const {
            this->mMessageListener.SendMessage(message);
        }

        volatile uint64_t mUpdateTime;
        volatile uint64_t mDeltaTime;
        bool              mInitialized;
        EServices         mServiceId;

    private:
        volatile uint64_t              mPreviousTime;
        volatile uint64_t              mMinDeltaTime;
        volatile uint64_t              mMaxDeltaTime;
        Message::MessageInterface     &mMessageListener;
        Event::EventListenerInterface &mEventListener;
    };
} // namespace Service
