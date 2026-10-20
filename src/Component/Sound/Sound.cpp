#include "Sound.h"

namespace Component
{
    namespace Sound
    {
        static Sound *sound[NB_SENSORS_SOUND] = {};
        uint8_t       Sound::soundIndex = 0U;

        Sound::Sound(const SoundId &soundId, Gpio::GpioInterface &gpio, Led::LedInterface &led)
            : mStartSoundTime(0UL)
            , mStopSoundTime(0UL)
            , mGpioSound(gpio)
            , mLed(led)
            , mIntervalSoundTimeArray{0U}
            , mOnsetSoundTimeArray{0U}
            , mIntervalSoundTimeArrayIndex(0U)
            , mEventCount(0U)
            , mAverageIntervalSoundTime(0U)
            , mLastLevel(true) {
            sound[soundId] = this;
            soundIndex++;
        }

        Core::Status Sound::Initialize() {
            const Core::Status success = this->mLed.Initialize();
            this->mStartSoundTime = 0U;
            this->mStopSoundTime = 0U;
            this->mLastLevel = true;
            this->mLed.Off();
            this->mGpioSound.SetInterruptPin(Sound::InterruptGpioSoundHit);
            LOG_COMPONENT_DEBUG("Sound", "pin %d Initialized.", this->mGpioSound.GetPin().pin);
            return success;
        }

        void Sound::RecordEvent(const uint64_t onsetUs, const uint64_t durationUs) {
            if (this->mIntervalSoundTimeArrayIndex < NB_MAX_INTERVAL_SOUND_TIME) {
                this->mOnsetSoundTimeArray[this->mIntervalSoundTimeArrayIndex] = onsetUs;
                this->mIntervalSoundTimeArray[this->mIntervalSoundTimeArrayIndex] = durationUs;
                this->mIntervalSoundTimeArrayIndex++;
            }
        }

        void Sound::Hit() {
            this->Hit(Timer::Tick::GetInstance().GetUs());
        }

        void Sound::Hit(const uint64_t timestampUs) {
            const bool level = this->mGpioSound.Get();
            // The interrupt is shared by all sensors: ignore sensors whose level did not change,
            // otherwise their start/stop times would be overwritten by an unrelated edge.
            if (level == this->mLastLevel) {
                return;
            }
            this->mLastLevel = level;

            if (!level) {
                this->mStartSoundTime = timestampUs;
                this->mLed.On();
            } else {
                this->mStopSoundTime = timestampUs;
                this->mLed.Off();
                if (this->mStartSoundTime != 0U && timestampUs >= this->mStartSoundTime) {
                    this->RecordEvent(this->mStartSoundTime, timestampUs - this->mStartSoundTime);
                }
            }
        }

        void Sound::Update(const uint64_t currentTime) {
            (void) currentTime;
            // If the sound is still present, stop the
            // signal
            if (this->mStopSoundTime < this->mStartSoundTime) {
                this->mStopSoundTime = Timer::Tick::GetInstance().GetUs();
                this->RecordEvent(this->mStartSoundTime,
                                  this->mStopSoundTime - this->mStartSoundTime);
            }

            // Make the average
            if (this->mIntervalSoundTimeArrayIndex > 0U) {
                // make the average of all hits interval
                this->mAverageIntervalSoundTime = 0U;
                for (uint8_t indexInterval = 0U; indexInterval < this->mIntervalSoundTimeArrayIndex;
                     indexInterval++) {
                    this->mAverageIntervalSoundTime += this->mIntervalSoundTimeArray[indexInterval];
                }
                this->mAverageIntervalSoundTime /= this->mIntervalSoundTimeArrayIndex;
            }
            this->mEventCount = this->mIntervalSoundTimeArrayIndex;
            this->mIntervalSoundTimeArrayIndex = 0U;
        }

        uint64_t Sound::GetIntervalSoundHit() const {
            return this->mAverageIntervalSoundTime;
        }

        // The direction is given by the microphone which hears each sound first (time difference
        // of arrival). When the arrival times are too close to be reliable, the loudest
        // (longest) signal wins. A sound heard by only one microphone votes for that side.
        SoundId Sound::ComputeDirection(uint64_t &delay) {
            enum {
                LEFT = 0U,
                RIGHT = 1U
            };
            const Sound *const sensor[NB_SENSORS_SOUND] = {sound[SOUND_LEFT], sound[SOUND_RIGHT]};
            SoundId            result = SOUND_NONE;
            delay = 0U;
            if (sensor[LEFT] == nullptr || sensor[RIGHT] == nullptr) {
                return result;
            }

            const auto nextValid = [](const Sound &s, uint8_t index) {
                while (index < s.mEventCount && s.mIntervalSoundTimeArray[index] < MIN_VALID_SOUND_US) {
                    index++;
                }
                return index;
            };

            uint32_t votes[NB_SENSORS_SOUND] = {0U, 0U};
            uint64_t total[NB_SENSORS_SOUND] = {0U, 0U};
            uint32_t count[NB_SENSORS_SOUND] = {0U, 0U};
            for (uint8_t side = 0U; side < NB_SENSORS_SOUND; side++) {
                for (uint8_t i = nextValid(*sensor[side], 0U); i < sensor[side]->mEventCount;
                     i = nextValid(*sensor[side], i + 1U)) {
                    total[side] += sensor[side]->mIntervalSoundTimeArray[i];
                    count[side]++;
                }
            }

            uint8_t l = nextValid(*sensor[LEFT], 0U);
            uint8_t r = nextValid(*sensor[RIGHT], 0U);
            while (l < sensor[LEFT]->mEventCount || r < sensor[RIGHT]->mEventCount) {
                if (l >= sensor[LEFT]->mEventCount) {
                    votes[RIGHT]++;
                    r = nextValid(*sensor[RIGHT], r + 1U);
                    continue;
                }
                if (r >= sensor[RIGHT]->mEventCount) {
                    votes[LEFT]++;
                    l = nextValid(*sensor[LEFT], l + 1U);
                    continue;
                }

                const uint64_t onsetLeft = sensor[LEFT]->mOnsetSoundTimeArray[l];
                const uint64_t onsetRight = sensor[RIGHT]->mOnsetSoundTimeArray[r];
                const uint64_t diff = onsetLeft > onsetRight ? onsetLeft - onsetRight :
                                                               onsetRight - onsetLeft;
                if (diff <= MAX_ARRIVAL_DELAY_US) {
                    // Same sound seen by both microphones
                    const uint64_t durationLeft = sensor[LEFT]->mIntervalSoundTimeArray[l];
                    const uint64_t durationRight = sensor[RIGHT]->mIntervalSoundTimeArray[r];
                    if (diff >= MIN_ARRIVAL_DELAY_US) {
                        votes[onsetLeft < onsetRight ? LEFT : RIGHT]++;
                    } else if (durationLeft != durationRight) {
                        votes[durationLeft > durationRight ? LEFT : RIGHT]++;
                    }
                    l = nextValid(*sensor[LEFT], l + 1U);
                    r = nextValid(*sensor[RIGHT], r + 1U);
                } else if (onsetLeft < onsetRight) {
                    votes[LEFT]++;
                    l = nextValid(*sensor[LEFT], l + 1U);
                } else {
                    votes[RIGHT]++;
                    r = nextValid(*sensor[RIGHT], r + 1U);
                }
            }

            uint8_t winner = NB_SENSORS_SOUND;
            if (votes[LEFT] != votes[RIGHT]) {
                winner = votes[LEFT] > votes[RIGHT] ? LEFT : RIGHT;
            } else if (total[LEFT] != total[RIGHT]) {
                winner = total[LEFT] > total[RIGHT] ? LEFT : RIGHT;
            }

            if (winner < NB_SENSORS_SOUND && count[winner] > 0U) {
                result = winner == LEFT ? SOUND_LEFT : SOUND_RIGHT;
                delay = total[winner] / count[winner];
            }
            return result;
        }

        SoundStruct Sound::ComputeAndNotifyMaxSound() {
            SoundStruct soundStruct = {.id = SOUND_NONE, .delay = 0U};
            if (sound[SOUND_LEFT] == nullptr || sound[SOUND_RIGHT] == nullptr) {
                return soundStruct;
            }

            soundStruct.id = ComputeDirection(soundStruct.delay);

            for (uint8_t id = 0U; id < NB_SENSORS_SOUND; id++) {
                sound[id]->mStartSoundTime = 0U;
                sound[id]->mAverageIntervalSoundTime = 0U;
                sound[id]->mEventCount = 0U;
                for (uint8_t indexInterval = 0U; indexInterval < NB_MAX_INTERVAL_SOUND_TIME;
                     indexInterval++) {
                    sound[id]->mIntervalSoundTimeArray[indexInterval] = 0U;
                    sound[id]->mOnsetSoundTimeArray[indexInterval] = 0U;
                }
            }

            if (soundStruct.id != SOUND_NONE) {
                sound[soundStruct.id]->Notify(soundStruct);
            }
            return soundStruct;
        }

        void Sound::InterruptGpioSoundHit() {
            // One timestamp for all sensors, so that the arrival time difference is not skewed
            // by the time spent reading each pin.
            const uint64_t timestampUs = Timer::Tick::GetInstance().GetUs();
            for (size_t i = 0U; i < Sound::soundIndex; i++) {
                if (sound[i] != nullptr) {
                    sound[i]->Hit(timestampUs);
                }
            }
        }
    } // namespace Sound
} // namespace Component
