#pragma once

#include "../../Driver/Gpio/GpioInterface.h"
#include "../../Driver/Timer/Tick.h"
#include "../Led/LedInterface.h"
#include "SoundInterface.h"
#include "SoundState.h"

namespace Component
{
    namespace Sound
    {
        using namespace Driver;

        class Sound : public SoundInterface {
        public:
            Sound(const SoundId       &soundId,
                  Gpio::GpioInterface &gpio,
                  Led::LedInterface   &led);

            ~Sound() = default;

            virtual Core::Status Initialize(void) final override;

            virtual void     Update(const uint64_t currentTime) final override;

            virtual uint64_t GetIntervalSoundHit(void) const override;

            void             Hit(void);

            static SoundStruct ComputeAndNotifyMaxSound(void);

            static uint8_t     soundIndex;
            static SoundId     soundNotificationOff;
            static bool        soundNotificationDone;
            uint64_t           mStartSoundTime;
            uint64_t           mStopSoundTime;

        private:
            static void              InterruptGpioSoundHit(void);

            constexpr static uint8_t NB_MAX_INTERVAL_SOUND_TIME = 100U;
            SoundId                  mSoundId;
            Gpio::GpioInterface     &mGpioSound;
            Led::LedInterface       &mLed;
            uint64_t mIntervalSoundTimeArray[NB_MAX_INTERVAL_SOUND_TIME];
            uint8_t  mIntervalSoundTimeArrayIndex;
            uint64_t mAverageIntervalSoundTime;
        };
    } // namespace Sound
} // namespace Component
