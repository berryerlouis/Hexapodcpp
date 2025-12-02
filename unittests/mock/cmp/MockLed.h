#pragma once
#include <gmock/gmock.h>
#include "../../../src/Component/Led/LedInterface.h"

namespace Component
{
    namespace Led
    {
        class MockLed : public LedInterface {
        public:
            MOCK_METHOD0(Initialize, Core::Status (void));

            MOCK_METHOD1(Update, void (const uint64_t));

            MOCK_METHOD0(On, Core::Status (void));

            MOCK_METHOD0(Off, Core::Status (void));

            MOCK_METHOD0(Toggle, Core::Status (void));

            MOCK_CONST_METHOD0(Get, LedState (void));
        };
    }
}