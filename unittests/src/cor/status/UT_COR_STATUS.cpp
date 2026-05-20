#include <gmock/gmock.h>
#include <gtest/gtest.h>


#include "../../../../src/Core/Status.h"

using ::testing::_;
using ::testing::Return;
using ::testing::StrictMock;

namespace Core
{
    class UT_COR_STATUS : public ::testing::Test {
    protected:
        UT_COR_STATUS() {
        }

        virtual void SetUp() {
        }

        virtual void TearDown() {
        }

        virtual ~UT_COR_STATUS() = default;

        /* Mocks */

        /* Test class */
    };

    TEST_F(UT_COR_STATUS, CORE_OK_Ok) {
        EXPECT_STREQ("CORE_OK", Core::StatusToString(Core::Status::CORE_OK).c_str());
    }

    TEST_F(UT_COR_STATUS, CORE_ERROR_Ok) {
        EXPECT_STREQ("CORE_ERROR", Core::StatusToString(Core::Status::CORE_ERROR).c_str());
    }

    TEST_F(UT_COR_STATUS, CORE_ERROR_ARGUMENT_Ok) {
        EXPECT_STREQ("CORE_ERROR_ARGUMENT",
                     Core::StatusToString(Core::Status::CORE_ERROR_ARGUMENT).c_str());
    }

    TEST_F(UT_COR_STATUS, CORE_ERROR_NULLPTR_Ok) {
        EXPECT_STREQ("CORE_ERROR_NULLPTR",
                     Core::StatusToString(Core::Status::CORE_ERROR_NULLPTR).c_str());
    }

    TEST_F(UT_COR_STATUS, CORE_ERROR_OVERLOAD_Ok) {
        EXPECT_STREQ("CORE_ERROR_OVERLOAD",
                     Core::StatusToString(Core::Status::CORE_ERROR_OVERLOAD).c_str());
    }

    TEST_F(UT_COR_STATUS, CORE_ERROR_SIZE_Ok) {
        EXPECT_STREQ("CORE_ERROR_SIZE",
                     Core::StatusToString(Core::Status::CORE_ERROR_SIZE).c_str());
    }

    TEST_F(UT_COR_STATUS, CORE_ERROR_MIN_Ok) {
        EXPECT_STREQ("CORE_ERROR_MIN", Core::StatusToString(Core::Status::CORE_ERROR_MIN).c_str());
    }

    TEST_F(UT_COR_STATUS, CORE_ERROR_MAX_Ok) {
        EXPECT_STREQ("CORE_ERROR_MAX", Core::StatusToString(Core::Status::CORE_ERROR_MAX).c_str());
    }

    TEST_F(UT_COR_STATUS, CORE_ERROR_DISABLE_Ok) {
        EXPECT_STREQ("CORE_ERROR_DISABLE",
                     Core::StatusToString(Core::Status::CORE_ERROR_DISABLE).c_str());
    }

    TEST_F(UT_COR_STATUS, CORE_ERROR_UNKNOWN_CLUSTER_Ok) {
        EXPECT_STREQ("CORE_ERROR_UNKNOWN_CLUSTER",
                     Core::StatusToString(Core::Status::CORE_ERROR_UNKNOWN_CLUSTER).c_str());
    }

    TEST_F(UT_COR_STATUS, CORE_ERROR_DEFAULT_Ok) {
        EXPECT_STREQ("UNKNOWN_STATUS", Core::StatusToString(Core::Status(-50)).c_str());
    }

    TEST_F(UT_COR_STATUS, OperatorOr_Ok) {
        Core::Status status = Core::Status::CORE_OK | CORE_ERROR_DISABLE;
        status |= Core::Status::CORE_ERROR;
        EXPECT_EQ(status, Core::Status::CORE_ERROR | Core::Status::CORE_OK | CORE_ERROR_DISABLE);
    }
} // namespace Core
