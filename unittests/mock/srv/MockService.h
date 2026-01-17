#pragma once
#include <gmock/gmock.h>
#include "../../../src/Service/ServiceInterface.h"


namespace Service
{
    class MockService : public ServiceInterface {
    public:
        MockService() = default;
    };
} // namespace Service
