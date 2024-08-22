#include <gmock/gmock.h>
#include <gtest/gtest.h>


#include "../../../../src/Cluster/Decoding/Protocol.h"

using ::testing::_;
using ::testing::Return;

namespace Cluster {
namespace Decoding {
class UT_CLU_PROTOCOL : public ::testing::Test {
protected:
	UT_CLU_PROTOCOL()
	{
	}

	virtual void SetUp ()
	{
	}

	virtual void TearDown ()
	{
	}

	virtual ~UT_CLU_PROTOCOL() = default;

	/* Mocks */

	/* Test class */
};

TEST_F( UT_CLU_PROTOCOL, Decode_NullPtrBufferData )
{
	Frame request;

	Protocol::ProtocolStatus parsedStatus = Protocol::Decode( nullptr, request );

	EXPECT_EQ( parsedStatus, Protocol::ProtocolStatus::ERROR_NULL_BUFFER );
	EXPECT_EQ( request.clusterId, 0U );
	EXPECT_EQ( request.commandId, 0U );
	EXPECT_EQ( request.nbParams, 0U );
	for ( size_t i = 0U; i < FRAME_MAX_PARAMS; i++ )
	{
		EXPECT_EQ( request.params[i], 0U );
	}
}

TEST_F( UT_CLU_PROTOCOL, Decode_NoBufferData )
{
	Frame request;
	char  bufferRx[] = "";
	Protocol::ProtocolStatus parsedStatus = Protocol::Decode( bufferRx, request );

	EXPECT_NE( parsedStatus, Protocol::ProtocolStatus::NO_ERROR );
	EXPECT_EQ( request.clusterId, 0U );
	EXPECT_EQ( request.commandId, 0U );
	EXPECT_EQ( request.nbParams, 0U );
	for ( size_t i = 0U; i < FRAME_MAX_PARAMS; i++ )
	{
		EXPECT_EQ( request.params[i], 0U );
	}
}

TEST_F( UT_CLU_PROTOCOL, Decode_BufferDataWhitoutParam )
{
	Frame request;
	char  bufferRx[] = "010400";
	Protocol::ProtocolStatus parsedStatus = Protocol::Decode( bufferRx, request );

	EXPECT_EQ( parsedStatus, Protocol::ProtocolStatus::NO_ERROR );
	EXPECT_EQ( request.clusterId, 1U );
	EXPECT_EQ( request.commandId, 4U );
	EXPECT_EQ( request.nbParams, 0U );
	for ( size_t i = 0U; i < FRAME_MAX_PARAMS; i++ )
	{
		EXPECT_EQ( request.params[i], 0U );
	}
}

TEST_F( UT_CLU_PROTOCOL, Decode_BufferDataWhitParam )
{
	Frame request;
	char  bufferRx[] = "0608020102";
	Protocol::ProtocolStatus parsedStatus = Protocol::Decode( bufferRx, request );

	EXPECT_EQ( parsedStatus, Protocol::ProtocolStatus::NO_ERROR );
	EXPECT_EQ( request.clusterId, 6U );
	EXPECT_EQ( request.commandId, 8U );
	EXPECT_EQ( request.nbParams, 2U );
	EXPECT_EQ( request.params[0U], 1U );
	EXPECT_EQ( request.params[1U], 2U );

	for ( size_t i = request.nbParams; i < FRAME_MAX_PARAMS; i++ )
	{
		EXPECT_EQ( request.params[i], 0U );
	}
}

TEST_F( UT_CLU_PROTOCOL, Decode_BufferDataTooSmall )
{
	Frame request;
	char  bufferRx[] = "00000";
	Protocol::ProtocolStatus parsedStatus = Protocol::Decode( bufferRx, request );

	EXPECT_EQ( parsedStatus, Protocol::ProtocolStatus::ERROR_LENGHT );
	EXPECT_EQ( request.clusterId, 0U );
	EXPECT_EQ( request.commandId, 0U );
	EXPECT_EQ( request.nbParams, 0U );
	for ( size_t i = 0U; i < FRAME_MAX_PARAMS; i++ )
	{
		EXPECT_EQ( request.params[i], 0U );
	}
}

TEST_F( UT_CLU_PROTOCOL, Decode_BufferDataTooBig )
{
	Frame request;
	char  bufferRx[] = "0000000";
	Protocol::ProtocolStatus parsedStatus = Protocol::Decode( bufferRx, request );

	EXPECT_EQ( parsedStatus, Protocol::ProtocolStatus::ERROR_LENGHT );
	EXPECT_EQ( request.clusterId, 0U );
	EXPECT_EQ( request.commandId, 0U );
	EXPECT_EQ( request.nbParams, 0U );
	for ( size_t i = 0U; i < FRAME_MAX_PARAMS; i++ )
	{
		EXPECT_EQ( request.params[i], 0U );
	}
}

TEST_F( UT_CLU_PROTOCOL, Decode_BufferDataInvalidCharacter )
{
	Frame request;
	char  bufferRx[] = "000g00";
	Protocol::ProtocolStatus parsedStatus = Protocol::Decode( bufferRx, request );

	EXPECT_EQ( parsedStatus, Protocol::ProtocolStatus::ERROR_CHAR_INVALID );
	EXPECT_EQ( request.clusterId, 0U );
	EXPECT_EQ( request.commandId, 0U );
	EXPECT_EQ( request.nbParams, 0U );
	for ( size_t i = 0U; i < FRAME_MAX_PARAMS; i++ )
	{
		EXPECT_EQ( request.params[i], 0U );
	}
}

TEST_F( UT_CLU_PROTOCOL, Decode_BufferDataSizeError )
{
	Frame request;
	char  bufferRx[] = "000001";
	Protocol::ProtocolStatus parsedStatus = Protocol::Decode( bufferRx, request );

	EXPECT_EQ( parsedStatus, Protocol::ProtocolStatus::ERROR_SIZE_PARAMS );
	EXPECT_EQ( request.clusterId, 0U );
	EXPECT_EQ( request.commandId, 0U );
	EXPECT_EQ( request.nbParams, 1U );
	for ( size_t i = 0U; i < FRAME_MAX_PARAMS; i++ )
	{
		EXPECT_EQ( request.params[i], 0U );
	}
}

TEST_F( UT_CLU_PROTOCOL, Encode_Frame )
{
	Frame   response;
	uint8_t params[] = { 1U, 2U };
	response.Build(
		4U,
		1U,
		params,
		2U );
	char         bufferTx[100U];
	const size_t size = Protocol::Encode( response, (uint8_t *) bufferTx );

	EXPECT_EQ( size, 12U );

	EXPECT_TRUE( 0U == strcmp( bufferTx, "<0401020102>" ) );
}
}
}
