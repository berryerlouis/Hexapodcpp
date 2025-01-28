#pragma once
#include "../../Core/CoreInterface.h"
#include "SoundState.h"
#include "SoundObserverInterface.h"
#include "SoundObservableInterface.h"

namespace Component
{
    namespace Sound
    {
        class SoundObservable : public SoundObservableInterface {
        public:
            SoundObservable() :
                mIndexList(0U)
                , mListObserver{nullptr} {
            }

            ~SoundObservable() = default;

            virtual Core::Status
            Attach(SoundObserverInterface *observer) final override {
                Core::Status success = Core::Status::CORE_ERROR;
                if (this->mIndexList != MAX_OBSERVERS) {
                    this->mListObserver[this->mIndexList] = observer;
                    this->mIndexList++;
                    success = Core::Status::CORE_OK;
                }
                return (success);
            }

            virtual void
            Notify(const SoundId &soundId, const SoundState &soundState, const uint16_t period) final override {
                for (size_t i = 0; i < this->mIndexList; i++) {
                    if (this->mListObserver[i] != nullptr) {
                        this->mListObserver[i]->UpdatedSoundState(soundId, soundState, period);
                    }
                }
            }

        private:
            static constexpr uint8_t MAX_OBSERVERS = 2U;
            uint8_t mIndexList;
            SoundObserverInterface *mListObserver[MAX_OBSERVERS];
        };
    }
}
