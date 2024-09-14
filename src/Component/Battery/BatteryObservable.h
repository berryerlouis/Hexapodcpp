#pragma once
#include "../../Core/CoreInterface.h"
#include "BatteryState.h"
#include "BatteryObserverInterface.h"
#include "BatteryObservableInterface.h"

namespace Component
{
    namespace Battery
    {
        class BatteryObservable : public BatteryObservableInterface {
        public:
#define MAX_OBSERVERS    5U

            BatteryObservable() : mIndexList(0U)
                                  , mListObserver{nullptr} {
            }

            ~BatteryObservable() = default;

            virtual Core::CoreStatus Attach(BatteryObserverInterface *observer) final override {
                Core::CoreStatus success = Core::CoreStatus::CORE_ERROR;
                if (this->mIndexList != MAX_OBSERVERS) {
                    this->mListObserver[this->mIndexList] = observer;
                    this->mIndexList++;
                    success = Core::CoreStatus::CORE_OK;
                }
                return (success);
            }

            virtual void Notify(const BatteryState &state, const uint16_t voltage) final override {
                for (size_t i = 0; i < this->mIndexList; i++) {
                    if (this->mListObserver[i] != nullptr) {
                        this->mListObserver[i]->UpdatedBatteryState(state, voltage);
                    }
                }
            }

        private:
            uint8_t mIndexList;
            BatteryObserverInterface *mListObserver[MAX_OBSERVERS];
        };
    }
}
