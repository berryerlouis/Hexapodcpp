#pragma once
#include <gmock/gmock.h>
#include "../../../src/Component/Button/ButtonInterface.h"

namespace Component
{
    namespace Button
    {
        class MockButton : public ButtonInterface {
        public:
            MOCK_METHOD0(Initialize, Core::Status(void));

            MOCK_METHOD1(Update, void(const uint64_t));

            MOCK_CONST_METHOD0(Get, ButtonState(void));
        };
    } // namespace Button
} // namespace Component
