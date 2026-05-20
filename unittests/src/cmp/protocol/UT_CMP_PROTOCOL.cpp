#include <gmock/gmock.h>
#include <gtest/gtest.h>


#include "../../../../src/Component/Communication/Decoding/Protocol.h"

using ::testing::_;
using ::testing::Return;

namespace Component
{
    namespace Decoding
    {
        class UT_CMP_PROTOCOL : public ::testing::Test {
        protected:
            UT_CMP_PROTOCOL() {
            }

            virtual void SetUp() {
            }

            virtual void TearDown() {
            }

            virtual ~UT_CMP_PROTOCOL() = default;

            /* Mocks */

            /* Test class */
        };

        TEST_F(UT_CMP_PROTOCOL, Decode_NullPtrBufferData) {
            Frame              request;

            const Core::Status parsedStatus = Protocol::Decode(nullptr, request);

            EXPECT_EQ(parsedStatus, Core::Status::CORE_ERROR_NULLPTR);
            EXPECT_EQ(request.GetClusterId(), 0U);
            EXPECT_EQ(request.GetCommandId(), 0U);
            EXPECT_EQ(request.GetNbParams(), 0U);
            for (size_t i = 0U; i < FRAME_MAX_PARAMS; i++) {
                EXPECT_EQ(request.Get1ByteParam(i), 0U);
            }
        }

        TEST_F(UT_CMP_PROTOCOL, Decode_NoBufferData) {
            Frame              request;
            constexpr char     bufferRx[] = "";
            const Core::Status parsedStatus = Protocol::Decode(bufferRx, request);

            EXPECT_NE(parsedStatus, Core::Status::CORE_OK);
            EXPECT_EQ(request.GetClusterId(), 0U);
            EXPECT_EQ(request.GetCommandId(), 0U);
            EXPECT_EQ(request.GetNbParams(), 0U);
            for (size_t i = 0U; i < FRAME_MAX_PARAMS; i++) {
                EXPECT_EQ(request.Get1ByteParam(i), 0U);
            }
        }

        TEST_F(UT_CMP_PROTOCOL, Decode_BufferDataWhitoutParam) {
            Frame              request;
            constexpr char     bufferRx[] = "010400";
            const Core::Status parsedStatus = Protocol::Decode(bufferRx, request);

            EXPECT_EQ(parsedStatus, Core::Status::CORE_OK);
            EXPECT_EQ(request.GetClusterId(), 1U);
            EXPECT_EQ(request.GetCommandId(), 4U);
            EXPECT_EQ(request.GetNbParams(), 0U);
            for (size_t i = 0U; i < FRAME_MAX_PARAMS; i++) {
                EXPECT_EQ(request.Get1ByteParam(i), 0U);
            }
        }

        TEST_F(UT_CMP_PROTOCOL, Decode_BufferDataWhitParam) {
            Frame              request;
            constexpr char     bufferRx[] = "0608020102";
            const Core::Status parsedStatus = Protocol::Decode(bufferRx, request);

            EXPECT_EQ(parsedStatus, Core::Status::CORE_OK);
            EXPECT_EQ(request.GetClusterId(), 6U);
            EXPECT_EQ(request.GetCommandId(), 8U);
            EXPECT_EQ(request.GetNbParams(), 2U);
            EXPECT_EQ(request.Get1ByteParam(0U), 1U);
            EXPECT_EQ(request.Get1ByteParam(1U), 2U);

            for (size_t i = request.GetNbParams(); i < FRAME_MAX_PARAMS; i++) {
                EXPECT_EQ(request.Get1ByteParam(i), 0U);
            }
        }

        TEST_F(UT_CMP_PROTOCOL, Decode_BufferDataTooSmall) {
            Frame              request;
            constexpr char     bufferRx[] = "00000";
            const Core::Status parsedStatus = Protocol::Decode(bufferRx, request);

            EXPECT_EQ(parsedStatus, Core::Status::CORE_ERROR_SIZE);
            EXPECT_EQ(request.GetClusterId(), 0U);
            EXPECT_EQ(request.GetCommandId(), 0U);
            EXPECT_EQ(request.GetNbParams(), 0U);
            for (size_t i = 0U; i < FRAME_MAX_PARAMS; i++) {
                EXPECT_EQ(request.Get1ByteParam(i), 0U);
            }
        }

        TEST_F(UT_CMP_PROTOCOL, Decode_BufferDataTooBig) {
            Frame              request;
            constexpr char     bufferRx[] = "0000000";
            const Core::Status parsedStatus = Protocol::Decode(bufferRx, request);

            EXPECT_EQ(parsedStatus, Core::Status::CORE_ERROR_SIZE);
            EXPECT_EQ(request.GetClusterId(), 0U);
            EXPECT_EQ(request.GetCommandId(), 0U);
            EXPECT_EQ(request.GetNbParams(), 0U);
            for (size_t i = 0U; i < FRAME_MAX_PARAMS; i++) {
                EXPECT_EQ(request.Get1ByteParam(i), 0U);
            }
        }

        TEST_F(UT_CMP_PROTOCOL, Decode_BufferDataSizeError) {
            Frame              request;
            constexpr char     bufferRx[] = "000001";
            const Core::Status parsedStatus = Protocol::Decode(bufferRx, request);

            EXPECT_EQ(parsedStatus, Core::Status::CORE_ERROR_OVERLOAD);
            EXPECT_EQ(request.GetClusterId(), 0U);
            EXPECT_EQ(request.GetCommandId(), 0U);
            EXPECT_EQ(request.GetNbParams(), 0U);
            for (size_t i = 0U; i < FRAME_MAX_PARAMS; i++) {
                EXPECT_EQ(request.Get1ByteParam(i), 0U);
            }
        }

        TEST_F(UT_CMP_PROTOCOL, Encode_Frame) {
            Frame             response;
            constexpr uint8_t params[] = {1U, 2U};
            response.Build(4U, 1U, params, 2U);
            char         bufferTx[100U];
            const size_t size = Protocol::Encode(response, bufferTx);

            EXPECT_EQ(size, 12U);

            EXPECT_TRUE(0U == strcmp(bufferTx, "<0401020102>"));
        }

        TEST_F(UT_CMP_PROTOCOL, Encode_NullBuffer) {
            Frame             response;
            constexpr uint8_t params[] = {1U, 2U};
            response.Build(4U, 1U, params, 2U);
            const size_t size = Protocol::Encode(response, nullptr);
            EXPECT_EQ(size, 0U);
        }
    } // namespace Decoding
} // namespace Component
