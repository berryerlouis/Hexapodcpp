#include <gmock/gmock.h>
#include <gtest/gtest.h>


#include "../../../../src/Core/Logger.h"

using ::testing::_;
using ::testing::Return;
using ::testing::StrictMock;

namespace Core
{
    class UT_COR_LOGGER : public ::testing::Test {
    protected:
        UT_COR_LOGGER() {
        }

        virtual void SetUp() {
        }

        virtual void TearDown() {
        }

        virtual ~UT_COR_LOGGER() = default;

        /* Mocks */

        /* Test class */
    };

    TEST_F(UT_COR_LOGGER, SetLogLevel_Ok) {
        Logger::SetLogLevel(Core::LogLevel::LOG_DEBUG);
        EXPECT_EQ(Core::LogLevel::LOG_DEBUG, Logger::GetLogLevel());
    }

    TEST_F(UT_COR_LOGGER, LOG_DEBUG_Ok) {
        EXPECT_EQ(LogLevel::LOG_DEBUG, Logger::StringToLevel("DEBUG"));
    }

    TEST_F(UT_COR_LOGGER, LOG_INFO_Ok) {
        EXPECT_EQ(LogLevel::LOG_INFO, Logger::StringToLevel("INFO"));
    }

    TEST_F(UT_COR_LOGGER, LOG_WARNING_Ok) {
        EXPECT_EQ(LogLevel::LOG_WARNING, Logger::StringToLevel("WARNING"));
    }

    TEST_F(UT_COR_LOGGER, LOG_ERROR_Ok) {
        EXPECT_EQ(LogLevel::LOG_ERROR, Logger::StringToLevel("ERROR"));
    }

    TEST_F(UT_COR_LOGGER, LOG_NONE_Ok) {
        EXPECT_EQ(LogLevel::LOG_NONE, Logger::StringToLevel("NONE"));
    }

    TEST_F(UT_COR_LOGGER, LOG_UNKNOWN_Ok) {
        EXPECT_EQ(LogLevel::LOG_UNKNOWN, Logger::StringToLevel("LOG_UNKNOWN"));
    }


    TEST_F(UT_COR_LOGGER, LOG_Ok) {
        Logger::Log(LogLevel::LOG_DEBUG, "LOG_DEBUG");
        Logger::Log(LogLevel::LOG_INFO, "LOG_INFO");
        Logger::Log(LogLevel::LOG_WARNING, "LOG_WARNING");
        Logger::Log(LogLevel::LOG_ERROR, "LOG_ERROR");
        Logger::Log(LogLevel::LOG_NONE, "LOG_NONE");
        Logger::Log(LogLevel::LOG_UNKNOWN, "LOG_UNKNOWN");
    }

} // namespace Core
