#include "Software.h"

namespace Component
{
    namespace Software
    {
        Software::Software() : mMinTime(0UL)
                               , mMaxTime(0UL) {
        }

        Core::CoreStatus Software::Initialize(void) {
            return (Core::CoreStatus::CORE_OK);
        }

        void Software::Update(const uint64_t currentTime) {
            (void) currentTime;
        }

        SoftwareInterface::Version Software::GetVersion(void) {
            SoftwareInterface::Version version;
            version.major = 0U;
            version.minor = 1U;
            return (version);
        }

        uint64_t Software::GetMinTime(void) {
            return (this->mMinTime);
        }

        uint64_t Software::GetMaxTime(void) {
            return (this->mMaxTime);
        }

        void Software::ResetTiming(void) {
            this->mMinTime = 0U;
            this->mMaxTime = 0U;
        }

        void Software::SetMinTime(const uint64_t time) {
            this->mMinTime = time;
        }

        void Software::SetMaxTime(const uint64_t time) {
            this->mMaxTime = time;
        }
    }
}
