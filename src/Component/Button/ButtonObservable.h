#pragma once
#include "../../Core/CoreInterface.h"
#include "ButtonState.h"
#include "ButtonObserverInterface.h"
#include "ButtonObservableInterface.h"

namespace Component
{
    namespace Button
    {
        class ButtonObservable : public ButtonObservableInterface {
        public:
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
            Notify(const ButtonState &state, const uint16_t period) final override {
                for (size_t i = 0; i < this->mIndexList; i++) {
                    if (this->mListObserver[i] != nullptr) {
                        this->mListObserver[i]->UpdatedButtonState(state, period);
                    }
                }
            }

        private:
            static constexpr uint8_t MAX_OBSERVERS = 2U;
            uint8_t mIndexList;
            ButtonObserverInterface *mListObserver[MAX_OBSERVERS];
        };
    }
}
