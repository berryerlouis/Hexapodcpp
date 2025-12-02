#pragma once
#include <gmock/gmock.h>
#include "MockEventListener.h"
#include "../../../src/Service/ServiceInterface.h"


namespace Service
{
    class MockService : public ServiceInterface {
    public:
        MockService(Event::MessageInterface &messageListener)
            :
            mMessageListener(messageListener) {
        }

        MOCK_METHOD0(Initialize, Core::Status (void));

        MOCK_METHOD1(Update, void (const uint64_t));

    private:
        Event::MessageInterface &mMessageListener;
    };
}