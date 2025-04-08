#include "Software.h"

namespace Component
{
    namespace Software
    {
        Software::Software() :
            mLastLoopTime(0UL),
            mMinLoopTime(0xFFFFFFFFUL),
            mMaxLoopTime(0UL) {
        }

        Core::Status Software::Initialize(void) {
            return (Core::Status::CORE_OK);
        }

        void Software::Update(const uint64_t currentTime) {
            const uint64_t currentLoopTime = currentTime - this->mLastLoopTime;
            if (this->mMaxLoopTime > 1UL) {
                this->mMaxLoopTime--;
            }
            if (this->mMinLoopTime > 0UL) {
                this->mMinLoopTime++;
            }

            if (currentLoopTime < this->mMinLoopTime) {
                this->mMinLoopTime = currentLoopTime;
            } else if (currentLoopTime > this->mMaxLoopTime) {
                this->mMaxLoopTime = currentLoopTime;
            }
            this->mLastLoopTime = currentTime;
        }

        uint64_t Software::GetMinTime(void) const {
            return this->mMinLoopTime;
        }

        uint64_t Software::GetMaxTime(void) const {
            return this->mMaxLoopTime;
        }

        void Software::ResetTime(void) {
            this->mMaxLoopTime = 0UL;
            this->mMinLoopTime = 0xFFFFFFFFUL;
        }

        SoftwareInterface::Version Software::GetVersion(void) {
            constexpr Version version = {.major = 0U, .minor = 1U};
            return (version);
        }
    }
}
