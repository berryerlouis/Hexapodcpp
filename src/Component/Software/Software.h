#pragma once

#include "SoftwareInterface.h"

namespace Component
{
    namespace Software
    {
        class Software : public SoftwareInterface {
        public:
            Software();

            ~Software() = default;

            virtual Core::Status Initialize(void) final override;

            virtual void         Update(const uint64_t currentTime) final override;

            virtual Version      GetVersion(void) final override;

            virtual uint64_t     GetMinTime(void) const override;

            virtual uint64_t     GetMaxTime(void) const override;

            virtual void         ResetTime(void) override;

        private:
            uint64_t mLastLoopTime;
            uint64_t mMinLoopTime;
            uint64_t mMaxLoopTime;
            uint64_t mPrevTime;

            void     AdjustLoopTime(const uint64_t currentTime);
        };
    } // namespace Software
} // namespace Component
