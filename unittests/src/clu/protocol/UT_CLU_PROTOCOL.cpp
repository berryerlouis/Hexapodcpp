#include <gmock/gmock.h>
#include <gtest/gtest.h>


#include "../../../../src/Cluster/Decoding/Protocol.h"

using ::testing::_;
using ::testing::Return;

namespace Cluster
{
	namespace Decoding
	{
		class UT_CLU_PROTOCOL : public ::testing::Test {
		protected:
			UT_CLU_PROTOCOL() {
			}

			virtual void SetUp() {
			}

			virtual void TearDown() {
			}

			virtual ~UT_CLU_PROTOCOL() = default;

			/* Mocks */

			/* Test class */
		};

		TEST_F(UT_CLU_PROTOCOL, Decode_NullPtrBufferData) {
			Frame request;

			const Core::CoreStatus parsedStatus = Protocol::Decode(nullptr, request);

			EXPECT_EQ(parsedStatus, Core::CoreStatus::CORE_ERROR_NULLPTR);
			EXPECT_EQ(request.clusterId, 0U);
			EXPECT_EQ(request.commandId, 0U);
			EXPECT_EQ(request.nbParams, 0U);
			for (size_t i = 0U; i < FRAME_MAX_PARAMS; i++) {
				EXPECT_EQ(request.params[i], 0U);
			}
		}

		TEST_F(UT_CLU_PROTOCOL, Decode_NoBufferData) {
			Frame request;
			constexpr char bufferRx[] = "";
			const Core::CoreStatus parsedStatus = Protocol::Decode(bufferRx, request);

			EXPECT_NE(parsedStatus, Core::CoreStatus::CORE_OK);
			EXPECT_EQ(request.clusterId, 0U);
			EXPECT_EQ(request.commandId, 0U);
			EXPECT_EQ(request.nbParams, 0U);
			for (size_t i = 0U; i < FRAME_MAX_PARAMS; i++) {
				EXPECT_EQ(request.params[i], 0U);
			}
		}

		TEST_F(UT_CLU_PROTOCOL, Decode_BufferDataWhitoutParam) {
			Frame request;
			constexpr char bufferRx[] = "010400";
			const Core::CoreStatus parsedStatus = Protocol::Decode(bufferRx, request);

			EXPECT_EQ(parsedStatus, Core::CoreStatus::CORE_OK);
			EXPECT_EQ(request.clusterId, 1U);
			EXPECT_EQ(request.commandId, 4U);
			EXPECT_EQ(request.nbParams, 0U);
			for (size_t i = 0U; i < FRAME_MAX_PARAMS; i++) {
				EXPECT_EQ(request.params[i], 0U);
			}
		}

		TEST_F(UT_CLU_PROTOCOL, Decode_BufferDataWhitParam) {
			Frame request;
			constexpr char bufferRx[] = "0608020102";
			const Core::CoreStatus parsedStatus = Protocol::Decode(bufferRx, request);

			EXPECT_EQ(parsedStatus, Core::CoreStatus::CORE_OK);
			EXPECT_EQ(request.clusterId, 6U);
			EXPECT_EQ(request.commandId, 8U);
			EXPECT_EQ(request.nbParams, 2U);
			EXPECT_EQ(request.params[0U], 1U);
			EXPECT_EQ(request.params[1U], 2U);

			for (size_t i = request.nbParams; i < FRAME_MAX_PARAMS; i++) {
				EXPECT_EQ(request.params[i], 0U);
			}
		}

		TEST_F(UT_CLU_PROTOCOL, Decode_BufferDataTooSmall) {
			Frame request;
			constexpr char bufferRx[] = "00000";
			const Core::CoreStatus parsedStatus = Protocol::Decode(bufferRx, request);

			EXPECT_EQ(parsedStatus, Core::CoreStatus::CORE_ERROR_SIZE);
			EXPECT_EQ(request.clusterId, 0U);
			EXPECT_EQ(request.commandId, 0U);
			EXPECT_EQ(request.nbParams, 0U);
			for (size_t i = 0U; i < FRAME_MAX_PARAMS; i++) {
				EXPECT_EQ(request.params[i], 0U);
			}
		}

		TEST_F(UT_CLU_PROTOCOL, Decode_BufferDataTooBig) {
			Frame request;
			constexpr char bufferRx[] = "0000000";
			const Core::CoreStatus parsedStatus = Protocol::Decode(bufferRx, request);

			EXPECT_EQ(parsedStatus, Core::CoreStatus::CORE_ERROR_SIZE);
			EXPECT_EQ(request.clusterId, 0U);
			EXPECT_EQ(request.commandId, 0U);
			EXPECT_EQ(request.nbParams, 0U);
			for (size_t i = 0U; i < FRAME_MAX_PARAMS; i++) {
				EXPECT_EQ(request.params[i], 0U);
			}
		}

		TEST_F(UT_CLU_PROTOCOL, Decode_BufferDataSizeError) {
			Frame request;
			constexpr char bufferRx[] = "000001";
			const Core::CoreStatus parsedStatus = Protocol::Decode(bufferRx, request);

			EXPECT_EQ(parsedStatus, Core::CoreStatus::CORE_ERROR_OVERLOAD);
			EXPECT_EQ(request.clusterId, 0U);
			EXPECT_EQ(request.commandId, 0U);
			EXPECT_EQ(request.nbParams, 1U);
			for (size_t i = 0U; i < FRAME_MAX_PARAMS; i++) {
				EXPECT_EQ(request.params[i], 0U);
			}
		}

		TEST_F(UT_CLU_PROTOCOL, Encode_Frame) {
			Frame response;
			constexpr uint8_t params[] = {1U, 2U};
			response.Build(
				4U,
				1U,
				params,
				2U);
			char bufferTx[100U];
			const size_t size = Protocol::Encode(response, bufferTx);

			EXPECT_EQ(size, 12U);

			EXPECT_TRUE(0U == strcmp( bufferTx, "<0401020102>" ));
		}
	}
}
