#pragma once

#include "../../Driver/Gpio/GpioInterface.h"
#include "../../Driver/Tick/TickInterface.h"
#include "../Led/LedInterface.h"
#include "SoundsId.h"
#include "SoundInterface.h"
#include "SoundObservable.h"

namespace Component
{
    namespace Sound
    {
        using namespace Driver;

        class Sound : public SoundInterface {
        public:
            Sound(const SoundId &soundId, Gpio::GpioInterface &gpio, Led::LedInterface &led, Tick::TickInterface &tick);

            ~Sound() = default;

            virtual Core::Status Initialize(void) final override;

            virtual void Update(const uint64_t currentTime) final override;

            virtual SoundState Get(void) const final override;

            virtual Core::Status Attach(SoundObserverInterface *observer) final override;

            virtual void
            Notify(const SoundId &soundId, const SoundState &soundState, const uint16_t voltage) final override;

            void Hit(void);

        private:
            SoundId mSoundId;
            Gpio::GpioInterface &mGpioSound;
            Led::LedInterface &mLed;
            Tick::TickInterface &mTick;
            SoundObservable mObservable;
            uint64_t mStartSoundTime;
            uint64_t mStopSoundTime;
            SoundState mState;
        };
    }
}
