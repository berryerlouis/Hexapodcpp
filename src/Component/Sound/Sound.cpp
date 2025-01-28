#include "Sound.h"

namespace Component
{
    namespace Sound
    {
        static Sound *sound[3U] = {};
        static uint8_t soundIndex = 0U;

        void InterruptGpioBp(void) {
            for (size_t i = 0U; i < soundIndex; i++) {
                sound[i]->Hit();
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
            , mState(NO_SOUND) {
            sound[soundIndex] = this;
            soundIndex++;
        }

        Core::Status Sound::Initialize(void) {
            const Core::Status success = this->mLed.Initialize();
            this->mLed.Off();
            this->mGpioSound.SetInterruptPin(&InterruptGpioBp);
            return (success);
        }

        void Sound::Hit(void) {
            if (this->mGpioSound.Get() == true) {
                this->mStartSoundTime = this->mTick.GetUs();
                this->mLed.On();
                if (this->mState == NO_SOUND) {
                    this->Notify(this->mSoundId, LOUD, 0U);
                }
                this->mState = LOUD;
            } else {
                this->mStopSoundTime = this->mTick.GetUs();
            }
        }

        void Sound::Update(const uint64_t currentTime) {
            (void) currentTime;

            if (this->mState == LOUD && ((this->mTick.GetUs() / 1000U) - (this->mStartSoundTime / 1000U)) > 1000U) {
                this->mState = NO_SOUND;
                this->mLed.Off();
                const uint64_t delayMs = this->mStopSoundTime - this->mStartSoundTime;
                this->Notify(this->mSoundId, this->mState, delayMs);
                this->mStartSoundTime = 0U;
            }
        }

        SoundState Sound::Get() const {
            return (this->mState);
        }

        Core::Status Sound::Attach(SoundObserverInterface *observer) {
            return (this->mObservable.Attach(observer));
        }

        void Sound::Notify(const SoundId &soundId, const SoundState &soundState, const uint16_t voltage) {
            this->mObservable.Notify(soundId, soundState, voltage);
        }

    }

}
