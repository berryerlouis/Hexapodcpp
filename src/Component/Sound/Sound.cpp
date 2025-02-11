#include "Sound.h"
#include <stdlib.h>

namespace Component
{
    namespace Sound
    {
        static Sound *sound[NB_SENSORS_SOUND] = {};
        uint8_t Sound::soundIndex = 0U;

        void InterruptGpioSoundHit(void) {
            for (size_t i = 0U; i < Sound::soundIndex; i++) {
                if (sound[i] != nullptr) {
                    sound[i]->Hit();
                }
            }
        }

        Sound::Sound(const SoundId &soundId, Gpio::GpioInterface &gpio, Led::LedInterface &led,
                     Tick::TickInterface &tick) :
            mSoundId(soundId)
            , mGpioSound(gpio)
            , mLed(led)
            , mTick(tick)
            , mStartSoundTime(0U)
            , mStopSoundTime(0U)
            , mIntervalSoundTimeArray{0U}
            , mIntervalSoundTimeArrayIndex(0U)
            , mAverageIntervalSoundTime(0U)
            , mState(NO_SOUND) {
            sound[soundId] = this;
            soundIndex++;
        }

        Core::Status Sound::Initialize(void) {
            const Core::Status success = this->mLed.Initialize();
            this->mLed.Off();
            this->mGpioSound.SetInterruptPin(&InterruptGpioSoundHit);
            return (success);
        }

        void Sound::Hit(void) {
            if (this->mGpioSound.Get() == false) {
                this->mStartSoundTime = this->mTick.GetUs();
                this->mLed.On();
            } else {
                this->mStopSoundTime = this->mTick.GetUs();
                this->mLed.Off();
                if (this->mStartSoundTime != 0U
                    && this->mIntervalSoundTimeArrayIndex < NB_MAX_INTERVAL_SOUND_TIME) {
                    this->mIntervalSoundTimeArray[this->mIntervalSoundTimeArrayIndex] =
                            this->mStopSoundTime - this->mStartSoundTime;
                    this->mIntervalSoundTimeArrayIndex++;
                }
            }
        }

        void Sound::Update(const uint64_t currentTime) {

            // 100ms after the first hit compare the average interval of hits
            if (((sound[SOUND_LEFT]->mStartSoundTime != 0U) ||
                 (sound[SOUND_RIGHT]->mStartSoundTime != 0U)) &&
                (currentTime - (sound[SOUND_LEFT]->mStartSoundTime / 1000U) >= 100U) &&
                (currentTime - (sound[SOUND_RIGHT]->mStartSoundTime / 1000U) >= 100U)
            ) {
                // If the sound is still present count up to now
                if (this->mStopSoundTime < this->mStartSoundTime) {
                    this->mStopSoundTime = this->mTick.GetUs();
                    if (this->mIntervalSoundTimeArrayIndex < NB_MAX_INTERVAL_SOUND_TIME) {
                        this->mIntervalSoundTimeArray[this->mIntervalSoundTimeArrayIndex] =
                                this->mStopSoundTime - this->mStartSoundTime;
                        this->mIntervalSoundTimeArrayIndex++;
                    }
                }

                // Make the average
                for (size_t i = 0U; i < soundIndex; i++) {
                    if (sound[i]->mIntervalSoundTimeArrayIndex > 0U) {
                        //make the average of all hits interval
                        sound[i]->mAverageIntervalSoundTime = 0U;
                        for (uint8_t indexInterval = 0U;
                             indexInterval < sound[i]->mIntervalSoundTimeArrayIndex;
                             indexInterval++) {
                            sound[i]->mAverageIntervalSoundTime += sound[i]->mIntervalSoundTimeArray[indexInterval];
                        }
                        sound[i]->mAverageIntervalSoundTime /= sound[i]->mIntervalSoundTimeArrayIndex;
                    }
                }

                // Notify
                if (sound[SOUND_LEFT]->mAverageIntervalSoundTime > sound[SOUND_RIGHT]->mAverageIntervalSoundTime) {
                    sound[SOUND_LEFT]->mState = LOUD;
                    sound[SOUND_RIGHT]->mState = NO_SOUND;
                } else if (sound[SOUND_LEFT]->mAverageIntervalSoundTime < sound[SOUND_RIGHT]->
                           mAverageIntervalSoundTime) {
                    sound[SOUND_LEFT]->mState = NO_SOUND;
                    sound[SOUND_RIGHT]->mState = LOUD;
                } else {
                    sound[SOUND_LEFT]->mState = LOUD;
                    sound[SOUND_RIGHT]->mState = LOUD;
                }
                this->Notify({sound[SOUND_LEFT]->mSoundId,
                              sound[SOUND_LEFT]->mState,
                              sound[SOUND_LEFT]->mAverageIntervalSoundTime});
                this->Notify({sound[SOUND_RIGHT]->mSoundId,
                              sound[SOUND_RIGHT]->mState,
                              sound[SOUND_RIGHT]->mAverageIntervalSoundTime});
                this->mStartSoundTime = 0U;
                this->mIntervalSoundTimeArrayIndex = 0U;
            } else {
                // Do it once
                if (this->mAverageIntervalSoundTime != 0U) {
                    this->mState = NO_SOUND;
                    this->Notify({this->mSoundId,
                                  this->mState,
                                  this->mAverageIntervalSoundTime});
                }
                this->mAverageIntervalSoundTime = 0U;
            }
        }

        SoundState Sound::GetStatus() const {
            return (this->mState);
        }

        uint64_t Sound::GetLastStartTimeHit(void) const {
            return (this->mStartSoundTime);
        }

        uint64_t Sound::GetIntervalSoundHit(void) const {
            return (this->mAverageIntervalSoundTime);
        }

    }

}
