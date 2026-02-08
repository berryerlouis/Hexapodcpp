#include "Sound.h"

namespace Component
{
    namespace Sound
    {
        static Sound *sound[NB_SENSORS_SOUND] = {};
        uint8_t       Sound::soundIndex = 0U;

        Sound::Sound(const SoundId       &soundId,
                     Gpio::GpioInterface &gpio,
                     Led::LedInterface   &led)
            : mStartSoundTime(0UL)
            , mStopSoundTime(0UL)
            , mGpioSound(gpio)
            , mLed(led)
            , mIntervalSoundTimeArray{0U}
            , mIntervalSoundTimeArrayIndex(0U)
            , mAverageIntervalSoundTime(0U) {
            sound[soundId] = this;
            soundIndex++;
        }

        Core::Status Sound::Initialize() {
            const Core::Status success = this->mLed.Initialize();
            this->mStartSoundTime = 0U;
            this->mStopSoundTime = 0U;
            this->mLed.Off();
            this->mGpioSound.SetInterruptPin(Sound::InterruptGpioSoundHit);
            LOG_COMPONENT_DEBUG("Sound",
                                "pin %d Initialized.",
                                this->mGpioSound.GetPin().pin);
            return success;
        }

        void Sound::Hit() {
            if (!this->mGpioSound.Get()) {
                this->mStartSoundTime = Timer::Tick::GetInstance().GetUs();
                this->mLed.On();
            } else {
                this->mStopSoundTime = Timer::Tick::GetInstance().GetUs();
                this->mLed.Off();
                if (this->mStartSoundTime != 0U &&
                    this->mIntervalSoundTimeArrayIndex <
                            NB_MAX_INTERVAL_SOUND_TIME) {
                    this->mIntervalSoundTimeArray
                            [this->mIntervalSoundTimeArrayIndex] =
                            this->mStopSoundTime - this->mStartSoundTime;
                    this->mIntervalSoundTimeArrayIndex++;
                }
            }
        }

        void Sound::Update(const uint64_t currentTime) {
            (void) currentTime;
            // If the sound is still present, stop the
            // signal
            if (this->mStopSoundTime < this->mStartSoundTime) {
                this->mStopSoundTime = Timer::Tick::GetInstance().GetUs();
                if (this->mIntervalSoundTimeArrayIndex <
                    NB_MAX_INTERVAL_SOUND_TIME) {
                    this->mIntervalSoundTimeArray
                            [this->mIntervalSoundTimeArrayIndex] =
                            this->mStopSoundTime - this->mStartSoundTime;
                    this->mIntervalSoundTimeArrayIndex++;
                }
            }

            // Make the average
            if (this->mIntervalSoundTimeArrayIndex > 0U) {
                // make the average of all hits interval
                this->mAverageIntervalSoundTime = 0U;
                for (uint8_t indexInterval = 0U;
                     indexInterval < this->mIntervalSoundTimeArrayIndex;
                     indexInterval++) {
                    this->mAverageIntervalSoundTime +=
                            this->mIntervalSoundTimeArray[indexInterval];
                }
                this->mAverageIntervalSoundTime /=
                        this->mIntervalSoundTimeArrayIndex;
            }
            this->mIntervalSoundTimeArrayIndex = 0U;
        }

        uint64_t Sound::GetIntervalSoundHit() const {
            return this->mAverageIntervalSoundTime;
        }

        SoundStruct Sound::ComputeAndNotifyMaxSound() {
            SoundStruct    soundStruct = {.id = SOUND_NONE, .delay = 0U};
            const uint64_t soundLeft = sound[SOUND_LEFT]->GetIntervalSoundHit();
            const uint64_t soundRight =
                    sound[SOUND_RIGHT]->GetIntervalSoundHit();
            sound[SOUND_LEFT]->mStartSoundTime = 0U;
            sound[SOUND_RIGHT]->mStartSoundTime = 0U;
            sound[SOUND_LEFT]->mAverageIntervalSoundTime = 0U;
            sound[SOUND_RIGHT]->mAverageIntervalSoundTime = 0U;

            for (uint8_t indexInterval = 0U; indexInterval < 100U;
                 indexInterval++) {
                sound[SOUND_LEFT]->mIntervalSoundTimeArray[indexInterval] = 0U;
                sound[SOUND_RIGHT]->mIntervalSoundTimeArray[indexInterval] = 0U;
            }

            if (soundLeft > 1000U || soundRight > 1000U) {
                if (soundLeft > soundRight) {
                    soundStruct.id = SOUND_LEFT;
                    soundStruct.delay = soundLeft;
                } else {
                    soundStruct.id = SOUND_RIGHT;
                    soundStruct.delay = soundRight;
                }
                sound[soundStruct.id]->Notify(soundStruct);
            }
            return soundStruct;
        }

        void Sound::InterruptGpioSoundHit() {
            for (size_t i = 0U; i < Sound::soundIndex; i++) {
                if (sound[i] != nullptr) {
                    sound[i]->Hit();
                }
            }
        }
    } // namespace Sound
} // namespace Component
