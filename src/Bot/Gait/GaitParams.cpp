#include "GaitParams.h"


namespace Bot
{
    namespace Gait
    {
#define AMPLITUDE_MAX       3.0F
#define ELEVATION_MAX       3.0F
#define INITIAL_DURATION   3000U
#define DURATION_MIN       300U

        GaitParams::GaitParams(Driver::Tick::TickInterface &tick, const float directionAngle,
                               const float amplitude, const float elevation, const float rotation):
            mTick(tick)
            , mLerpDirection{directionAngle, directionAngle, 0UL}
            , mLerpAmplitude{amplitude, amplitude, 0UL}
            , mLerpElevation{elevation, elevation, 0UL}
            , mLerpRotation{rotation, rotation, 0UL}
            , mIsRotated(false)
            , mIsRotatedClockWize(false)
            , mStepDuration(INITIAL_DURATION) {
        }

        void GaitParams::Update(const uint64_t currentTime) {
            this->UpdateDirection(currentTime);
            this->UpdateRotation(currentTime);
            this->UpdateAmplitude(currentTime);
            this->UpdateElevation(currentTime);
        }

        bool GaitParams::SetDirection(const float directionAngle) {
            if (directionAngle <= 2U * M_PI) {
                this->mLerpDirection.target = directionAngle;
                this->mLerpDirection.startTime = this->mTick.GetMs();
                return true;
            }
            return false;
        }

        bool GaitParams::SetRotation(const float rotationAngle, const bool clockWize) {
            if (rotationAngle != 0 && rotationAngle <= 2U * M_PI) {
                this->mLerpRotation.target = rotationAngle;
                this->mLerpRotation.startTime = this->mTick.GetMs();
                this->mIsRotated = true;
                this->mIsRotatedClockWize = clockWize;
                return true;
            }
            this->mLerpRotation.target = 0.0F;
            this->mIsRotated = false;
            return false;
        }

        bool GaitParams::SetAmplitude(const float amplitude) {
            if (amplitude <= AMPLITUDE_MAX) {
                this->mLerpAmplitude.target = amplitude;
                this->mLerpAmplitude.startTime = this->mTick.GetMs();
                return true;
            }
            return false;
        }

        bool GaitParams::SetElevation(const float elevation) {
            if (elevation <= ELEVATION_MAX) {
                this->mLerpElevation.target = elevation;
                this->mLerpElevation.startTime = this->mTick.GetMs();
                return true;
            }
            return false;
        }

        bool GaitParams::SetCycleDuration(const uint16_t duration) {
            if (duration >= DURATION_MIN) {
                this->mStepDuration = duration;
                return true;
            }
            return false;
        }

        float GaitParams::GetDirection(void) const {
            return this->mLerpDirection.target;
        }

        float GaitParams::GetRotation(void) const {
            return this->mLerpRotation.target;
        }

        bool GaitParams::IsRotated(void) const {
            return this->mIsRotated;
        }

        float GaitParams::GetAmplitude(void) const {
            return this->mLerpAmplitude.target;
        }

        float GaitParams::GetElevation(void) const {
            return this->mLerpElevation.target;
        }

        uint16_t GaitParams::GetCycleDuration() const {
            return this->mStepDuration;
        }

        float GaitParams::GetCurrentDirection(void) const {
            return this->mLerpDirection.current;
        }

        float GaitParams::GetCurrentRotation(void) const {
            return this->mLerpRotation.current;
        }

        bool GaitParams::GetCurrentRotationClockWize() const {
            return this->mIsRotatedClockWize;
        }

        float GaitParams::GetCurrentAmplitude(void) const {
            return this->mLerpAmplitude.current;
        }

        float GaitParams::GetCurrentElevation(void) const {
            return this->mLerpElevation.current;
        }

        void GaitParams::UpdateDirection(const uint64_t currentTime) {
            if (currentTime <= this->mLerpDirection.startTime + 1000U) {
                const float deltaTime = GetDeltaTime(1000U, currentTime, this->mLerpDirection.startTime);
                this->mLerpDirection.current = Misc::Utils::LerpF(this->mLerpDirection.current,
                                                                  this->mLerpDirection.target, deltaTime);
            } else {
                this->mLerpDirection.current = this->mLerpDirection.target;
            }
        }

        void GaitParams::UpdateRotation(const uint64_t currentTime) {
            if (currentTime <= this->mLerpRotation.startTime + 1000U) {
                const float deltaTime = GetDeltaTime(1000U, currentTime, this->mLerpRotation.startTime);
                this->mLerpRotation.current = Misc::Utils::LerpF(this->mLerpRotation.current,
                                                                 this->mLerpRotation.target, deltaTime);
            } else {
                this->mLerpRotation.current = this->mLerpRotation.target;
            }
        }

        void GaitParams::UpdateAmplitude(const uint64_t currentTime) {
            if (currentTime <= this->mLerpAmplitude.startTime + 1000U) {
                const float deltaTime = GetDeltaTime(1000U, currentTime, this->mLerpAmplitude.startTime);
                this->mLerpAmplitude.current = Misc::Utils::LerpF(this->mLerpAmplitude.current,
                                                                  this->mLerpAmplitude.target, deltaTime);
            } else {
                this->mLerpAmplitude.current = this->mLerpAmplitude.target;
            }
        }

        void GaitParams::UpdateElevation(const uint64_t currentTime) {
            if (currentTime <= this->mLerpElevation.startTime + 1000U) {
                const float deltaTime = GetDeltaTime(1000U, currentTime, this->mLerpElevation.startTime);
                this->mLerpElevation.current = Misc::Utils::LerpF(this->mLerpElevation.current,
                                                                  this->mLerpElevation.target, deltaTime);
            } else {
                this->mLerpElevation.current = this->mLerpElevation.target;
            }
        }

        float GaitParams::GetDeltaTime(const float step, const uint64_t currentTime, const uint64_t startTime) {
            return (step - (step - (currentTime - startTime))) / step;
        }
    } // namespace GaitParams
}
