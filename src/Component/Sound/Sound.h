#pragma once

#include "../../Driver/Gpio/GpioInterface.h"
#include "../../Driver/Tick/TickInterface.h"
#include "../Led/LedInterface.h"
#include "SoundState.h"
#include "SoundInterface.h"

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

            virtual uint64_t GetIntervalSoundHit(void) const;

            void Hit(void);

            static SoundStruct ComputeAndNotifyMaxSound(void);

            static uint8_t soundIndex;
            static SoundId soundNotificationOff;
            static bool soundNotificationDone;

        private:
            static void InterruptGpioSoundHit(void);

            constexpr static uint8_t NB_MAX_INTERVAL_SOUND_TIME = 100U;
            SoundId mSoundId;
            Gpio::GpioInterface &mGpioSound;
            Led::LedInterface &mLed;
            Tick::TickInterface &mTick;
            uint64_t mStartSoundTime;
            uint64_t mStopSoundTime;
            uint64_t mIntervalSoundTimeArray[NB_MAX_INTERVAL_SOUND_TIME];
            uint8_t mIntervalSoundTimeArrayIndex;
            uint64_t mAverageIntervalSoundTime;
        };
    }
}
