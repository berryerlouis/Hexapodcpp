#pragma once
#include <gmock/gmock.h>
#include "../../../src/Component/Software/SoftwareInterface.h"

namespace Component
{
    namespace Software
    {
        class MockSoftware : public SoftwareInterface {
        public:
            MOCK_METHOD0(Initialize, Core::Status(void));

            MOCK_METHOD1(Update, void(const uint64_t));

            MOCK_METHOD0(GetVersion, Version(void));

            MOCK_CONST_METHOD0(GetMinTime, uint64_t(void));

            MOCK_CONST_METHOD0(GetMaxTime, uint64_t(void));

            MOCK_METHOD0(ResetTime, void(void));
        };
    } // namespace Software
} // namespace Component
