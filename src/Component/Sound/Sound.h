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
            Sound(const SoundId &soundId, Gpio::GpioInterface &gpio, Led::LedInterface &led);

            ~Sound() = default;

            virtual Core::Status Initialize(void) final override;

            virtual void         Update(const uint64_t currentTime) final override;

            virtual uint64_t     GetIntervalSoundHit(void) const override;

            void                 Hit(void);

            void                 Hit(const uint64_t timestampUs);

            static SoundStruct   ComputeAndNotifyMaxSound(void);

            static uint8_t       soundIndex;
            static SoundId       soundNotificationOff;
            static bool          soundNotificationDone;
            uint64_t             mStartSoundTime;
            uint64_t             mStopSoundTime;

        private:
            static void              InterruptGpioSoundHit(void);

            constexpr static uint8_t NB_MAX_INTERVAL_SOUND_TIME = 100U;
            // Minimal duration of a sound event, shorter ones are considered as noise
            constexpr static uint64_t MIN_VALID_SOUND_US = 1000U;
            // Maximal time between the onsets of one sound seen by both microphones
            constexpr static uint64_t MAX_ARRIVAL_DELAY_US = 1500U;
            // Below this onset difference, the timing is not reliable (interrupt jitter)
            constexpr static uint64_t MIN_ARRIVAL_DELAY_US = 50U;

            void                     RecordEvent(const uint64_t onsetUs, const uint64_t durationUs);
            static SoundId           ComputeDirection(uint64_t &delay);

            Gpio::GpioInterface     &mGpioSound;
            Led::LedInterface       &mLed;
            uint64_t                 mIntervalSoundTimeArray[NB_MAX_INTERVAL_SOUND_TIME];
            uint64_t                 mOnsetSoundTimeArray[NB_MAX_INTERVAL_SOUND_TIME];
            uint8_t                  mIntervalSoundTimeArrayIndex;
            uint8_t                  mEventCount;
            uint64_t                 mAverageIntervalSoundTime;
            bool                     mLastLevel;
        };
    } // namespace Sound
} // namespace Component
