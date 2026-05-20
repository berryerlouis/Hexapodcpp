#include "Software.h"
#include "../../Core/Version.h"

namespace Component
{
    namespace Software
    {
        Software::Software()
            : mLastLoopTime(0UL)
            , mMinLoopTime(0xFFFFFFFFUL)
            , mMaxLoopTime(0UL)
            , mPrevTime(0UL) {
        }

        Core::Status Software::Initialize() {
            LOG_COMPONENT_DEBUG("Software", "Initialized.");
            return (Core::Status::CORE_OK);
        }

        void Software::Update(const uint64_t currentTime) {
            const uint64_t currentLoopTime = currentTime - this->mLastLoopTime;

            if (currentLoopTime < this->mMinLoopTime) {
                this->mMinLoopTime = currentLoopTime;
            } else if (currentLoopTime > this->mMaxLoopTime) {
                this->mMaxLoopTime = currentLoopTime;
            }
            this->mLastLoopTime = currentTime;

            this->AdjustLoopTime(currentTime);
        }

        uint64_t Software::GetMinTime() const {
            return this->mMinLoopTime;
        }

        uint64_t Software::GetMaxTime() const {
            return this->mMaxLoopTime;
        }

        void Software::ResetTime() {
            this->mMaxLoopTime = 0UL;
            this->mMinLoopTime = 0xFFFFFFFFUL;
        }

        SoftwareInterface::Version Software::GetVersion() {
            constexpr Version version = {.major = VERSION_MAJOR, .minor = VERSION_MINOR};
            return (version);
        }

        void Software::AdjustLoopTime(const uint64_t currentTime) {
            if (currentTime > this->mPrevTime + 10UL) {
                if (this->mMaxLoopTime > 1UL) {
                    this->mMaxLoopTime--;
                }
                if (this->mMinLoopTime > 0UL) {
                    this->mMinLoopTime++;
                }
                this->mPrevTime = currentTime;
            }
        }
    } // namespace Software
} // namespace Component
