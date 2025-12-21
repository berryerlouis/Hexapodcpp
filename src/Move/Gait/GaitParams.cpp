#include "../../Misc/Maths/Utils.h"
#include "GaitCycle.h"

namespace Move
{
    namespace Gait
    {
        using namespace Misc::Maths;

        static constexpr float AMPLITUDE_MAX = 3.0F;
        static constexpr float ELEVATION_MAX = 3.0F;
        static constexpr float DURATION_MIN = 1000.0F; // in milliseconds

        GaitParams::GaitParams(void) :
            mCurrentGait(GaitType::TRIPOD),
            mIsRunning(false),
            mDirection(0.0F),
            mRotation(0.0F),
            mIsRotated(false),
            mClockWize(false),
            mAmplitude(AMPLITUDE_MAX),
            mElevation(ELEVATION_MAX),
            mCycleDuration(DURATION_MIN),
            mUpdatedTimeStamp(0UL) {
        }

        bool GaitParams::SetGaitType(const GaitType gaitType) {
            this->mCurrentGait = gaitType;
            return true;
        }

        GaitType GaitParams::GetGaitType() const {
            return this->mCurrentGait;
        }

        bool GaitParams::SetDirection(const float directionAngle) {
            if (directionAngle <= 2.0F * M_PI) {
                this->mDirection = directionAngle;
                this->mUpdatedTimeStamp = Driver::Timer::Tick::GetInstance().GetMs();
                return true;
            }
            return false;
        }

        bool GaitParams::SetRotation(const float rotationAngle,
                                     const bool  clockWize) {
            if (rotationAngle != 0.0F && rotationAngle <= 2.0F * M_PI) {
                this->mRotation = rotationAngle;
                this->mIsRotated = true;
                this->mClockWize = clockWize;
                this->mUpdatedTimeStamp = Driver::Timer::Tick::GetInstance().GetMs();
                return true;
            } else if (rotationAngle == 0.0F) {
                this->mRotation = 0.0F;
                this->mIsRotated = false;
                this->mClockWize = clockWize;
                this->mUpdatedTimeStamp = Driver::Timer::Tick::GetInstance().GetMs();
                return true;
            }
            return false;
        }

        bool GaitParams::SetAmplitude(const float amplitude) {
            if (amplitude <= AMPLITUDE_MAX) {
                this->mAmplitude = amplitude;
                this->mUpdatedTimeStamp = Driver::Timer::Tick::GetInstance().GetMs();
                return true;
            }
            return false;
        }

        bool GaitParams::SetElevation(const float elevation) {
            if (elevation <= ELEVATION_MAX) {
                this->mElevation = elevation;
                this->mUpdatedTimeStamp = Driver::Timer::Tick::GetInstance().GetMs();
                return true;
            }
            return false;
        }

        bool GaitParams::SetCycleDuration(const uint16_t duration) {
            if (duration >= DURATION_MIN) {
                this->mCycleDuration = duration;
                return true;
            }
            return false;
        }

        float GaitParams::GetDirection() const {
            return this->mDirection;
        }

        float GaitParams::GetRotation() const {
            return this->mRotation;
        }

        bool GaitParams::IsRotated() const {
            return this->mIsRotated;
        }

        bool GaitParams::GetRotationClockWize() const {
            return this->mClockWize;
        }

        float GaitParams::GetAmplitude() const {
            return this->mAmplitude;
        }

        float GaitParams::GetElevation() const {
            return this->mElevation;
        }

        uint16_t GaitParams::GetCycleDuration() const {
            return this->mCycleDuration;
        }

        bool GaitParams::IsRunning() const {
            return this->mIsRunning;
        }

        bool GaitParams::SetRunning(const bool running) {
            this->mIsRunning = running;
            return true;
        }

        uint64_t GaitParams::GetUpdatedTimeStamp() const {
            return this->mUpdatedTimeStamp;
        }
    } // namespace Gait
} // namespace Move
