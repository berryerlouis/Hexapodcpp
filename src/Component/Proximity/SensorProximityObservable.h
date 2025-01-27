#pragma once
#include "../../Core/CoreInterface.h"
#include "SensorsId.h"
#include "SensorProximityObserverInterface.h"
#include "SensorProximityObservableInterface.h"

namespace Component
{
    namespace Proximity
    {
        class SensorProximityObservable : public SensorProximityObservableInterface {
        public:
            SensorProximityObservable() :
                mIndexList(0U)
                , mListObserver{nullptr} {
            }

            ~SensorProximityObservable() = default;

            virtual Core::Status
            Attach(SensorProximityObserverInterface *observer) final override {
                Core::Status success = Core::Status::CORE_ERROR;
                if (this->mIndexList != MAX_OBSERVERS) {
                    this->mListObserver[this->mIndexList] = observer;
                    this->mIndexList++;
                    success = Core::Status::CORE_OK;
                }
                return (success);
            }

            virtual void
            Notify(const SensorsId &sensorId, const uint16_t distance) final override {
                for (size_t i = 0; i < this->mIndexList; i++) {
                    if (this->mListObserver[i] != nullptr) {
                        this->mListObserver[i]->Detect(sensorId, distance);
                    }
                }
            }

        private:
            static constexpr uint8_t MAX_OBSERVERS = 5U;
            uint8_t mIndexList;
            SensorProximityObserverInterface *mListObserver[MAX_OBSERVERS];
        };
    }
}
