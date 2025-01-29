#include "Sound.h"
#include <stdlib.h>

namespace Component
{
    namespace Sound
    {
        static Sound *sound[3U] = {};
        static uint8_t soundIndex = 0U;
        SoundId Sound::soundIdHit = SOUND_NONE;

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
            sound[soundId] = this;
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
                this->mState = LOUD;
            } else {
                this->mStopSoundTime = this->mTick.GetUs();
                this->mLed.Off();
                this->mState = NO_SOUND;
            }

            // check time delay between the 2 sensors hits, or if difference is bigger than 50 us
            if ((sound[SOUND_LEFT]->Get() == LOUD) && (sound[SOUND_RIGHT]->Get() == LOUD)) {
                if (sound[SOUND_LEFT]->GetLastStartTimeHit() < sound[SOUND_RIGHT]->GetLastStartTimeHit()) {
                    Sound::soundIdHit = SOUND_LEFT;
                    this->Notify(SOUND_LEFT, LOUD, sound[SOUND_LEFT]->GetLastStartTimeHit());
                } else {
                    Sound::soundIdHit = SOUND_RIGHT;
                    this->Notify(SOUND_RIGHT, LOUD, sound[SOUND_RIGHT]->GetLastStartTimeHit());
                }
            } else if (abs(static_cast<int64_t>(this->mStartSoundTime - this->mStopSoundTime)) >= 50U) {
                if (sound[SOUND_LEFT]->GetLastStartTimeHit() > sound[SOUND_RIGHT]->GetLastStartTimeHit()) {
                    Sound::soundIdHit = SOUND_LEFT;
                    this->Notify(SOUND_LEFT, LOUD, sound[SOUND_LEFT]->GetLastStartTimeHit());
                } else {
                    Sound::soundIdHit = SOUND_RIGHT;
                    this->Notify(SOUND_RIGHT, LOUD, sound[SOUND_RIGHT]->GetLastStartTimeHit());
                }
            }
        }

        void Sound::Update(const uint64_t currentTime) {
            (void) currentTime;

            // Notify one second after last hit
            if ((this->mState == NO_SOUND) &&
                (this->mStartSoundTime != this->mStopSoundTime) &&
                (this->mTick.GetUs() / 1000U) - (this->mStartSoundTime / 1000U) > 1000U) {
                const uint64_t delayMs = this->mStopSoundTime - this->mStartSoundTime;
                this->Notify(this->mSoundId, this->mState, delayMs);
                this->mStartSoundTime = this->mStopSoundTime;
            }
        }

        SoundState Sound::Get() const {
            return (this->mState);
        }

        uint64_t Sound::GetLastStartTimeHit(void) const {
            return (this->mStartSoundTime);
        }

        Core::Status Sound::Attach(SoundObserverInterface *observer) {
            return (this->mObservable.Attach(observer));
        }

        void Sound::Notify(const SoundId &soundId, const SoundState &soundState, const uint16_t voltage) {
            this->mObservable.Notify(soundId, soundState, voltage);
        }

    }

}
