#pragma once
#include <gmock/gmock.h>
#include "../../../src/Driver/Timer/TickInterface.h"

namespace Driver
{
    namespace Timer
    {
        class MockTick : public TickInterface {
        public:
            MOCK_METHOD0(GetUs, uint64_t(void));

            MOCK_METHOD0(GetMs, uint64_t(void));

            MOCK_METHOD1(DelayMs, void(uint64_t));

            MOCK_METHOD1(DelayUs, void(uint64_t));
        };
    } // namespace Timer
} // namespace Driver
