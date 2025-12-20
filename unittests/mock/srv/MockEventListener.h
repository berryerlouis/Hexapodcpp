#pragma once
#include <gmock/gmock.h>

#include "../../../src/Service/Message/MessageInterface.h"

namespace Service
{
    namespace Event
    {
        class MockMessageListener : public MessageInterface {
        public:
            MOCK_CONST_METHOD1(SendMessage, void (const Cluster::Frame &));
        };
    }
}