#pragma once
#include "../../Core/CoreInterface.h"
#include "ButtonObservableInterface.h"

namespace Component
{
    namespace Proximity
    {
        class ButtonObservable : public ButtonObservableInterface {
        public:
#define MAX_OBSERVERS    5U

            ButtonObservable() :
                mIndexList(0U)
                , mListObserver{nullptr} {
            }

            ~ButtonObservable() = default;

            virtual Core::Status
            Attach(ButtonObserverInterface *observer) final override {
                Core::Status success = Core::Status::CORE_ERROR;
                if (this->mIndexList != MAX_OBSERVERS) {
                    this->mListObserver[this->mIndexList] = observer;
                    this->mIndexList++;
                    success = Core::Status::CORE_OK;
                }
                return (success);
            }

            virtual void
            Notify(const uint16_t distance) final override {
                for (size_t i = 0; i < this->mIndexList; i++) {
                    if (this->mListObserver[i] != nullptr) {
                        this->mListObserver[i]->Event(distance);
                    }
                }
            }

        private:
            uint8_t mIndexList;
            ButtonObserverInterface *mListObserver[MAX_OBSERVERS];
        };
    }
}
