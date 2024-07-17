#include <gmock/gmock.h>
#include <gtest/gtest.h>

#include "../../../../src/clu/Constants.h"
#include "../../../../src/clu/Frame.h"

using ::testing::_;
using ::testing::Return;
using ::testing::StrictMock;

using namespace Clusters;

class UT_CLU_FRAME : public ::testing::Test  {
protected:
	UT_CLU_FRAME() :
		mFrame()
	{
	}

	virtual void SetUp ()
	{
	}

	virtual void TearDown ()
	{
	}

	virtual ~UT_CLU_FRAME() = default;

	/* Mocks */

	/* Test class */
	Clusters::Frame mFrame;
};

TEST_F( UT_CLU_FRAME, BuildNoArg_Ok )
{
	Core::CoreStatus success = Core::CoreStatus::CORE_ERROR;

	success = mFrame.Build(
		EClusters::BATTERY,
		EBatteryCommands::GET_VOLTAGE );

	EXPECT_EQ( mFrame.clusterId, Clusters::EClusters::BATTERY );
	EXPECT_EQ( mFrame.commandId, Clusters::EBatteryCommands::GET_VOLTAGE );
	EXPECT_EQ( mFrame.nbParams, 0U );
	EXPECT_EQ( mFrame.params[0U], 0U );
	EXPECT_TRUE( success );
}

TEST_F( UT_CLU_FRAME, BuildNoArgNullptr_Ok )
{
	Core::CoreStatus success = Core::CoreStatus::CORE_ERROR;

	success = mFrame.Build(
		EClusters::BATTERY,
		EBatteryCommands::GET_VOLTAGE,
		nullptr,
		0U );

	EXPECT_EQ( mFrame.clusterId, Clusters::EClusters::BATTERY );
	EXPECT_EQ( mFrame.commandId, Clusters::EBatteryCommands::GET_VOLTAGE );
	EXPECT_EQ( mFrame.nbParams, 0U );
	EXPECT_EQ( mFrame.params[0U], 0U );
	EXPECT_TRUE( success );
}

TEST_F( UT_CLU_FRAME, Build1ByteArg_Ok )
{
	Core::CoreStatus success = Core::CoreStatus::CORE_ERROR;

	uint8_t params = 42;
	success = mFrame.Build(
		EClusters::BATTERY,
		EBatteryCommands::GET_VOLTAGE,
		&params,
		1U );

	EXPECT_EQ( mFrame.clusterId, Clusters::EClusters::BATTERY );
	EXPECT_EQ( mFrame.commandId, Clusters::EBatteryCommands::GET_VOLTAGE );
	EXPECT_EQ( mFrame.nbParams, 1U );
	EXPECT_EQ( mFrame.params[0U], 42U );
	EXPECT_EQ( mFrame.Get1ByteParam( 0U ), 42U );
	EXPECT_TRUE( success );
}

TEST_F( UT_CLU_FRAME, BuildAndAdd1ByteArg_Ok )
{
	Core::CoreStatus success = Core::CoreStatus::CORE_ERROR;

	uint8_t params = 42;
	success = mFrame.Build(
		EClusters::BATTERY,
		EBatteryCommands::GET_VOLTAGE );
	mFrame.Set1ByteParam( 42U );

	EXPECT_EQ( mFrame.clusterId, Clusters::EClusters::BATTERY );
	EXPECT_EQ( mFrame.commandId, Clusters::EBatteryCommands::GET_VOLTAGE );
	EXPECT_EQ( mFrame.nbParams, 1U );
	EXPECT_EQ( mFrame.params[0U], 42U );
	EXPECT_EQ( mFrame.Get1ByteParam( 0U ), 42U );
	EXPECT_TRUE( success );
}

TEST_F( UT_CLU_FRAME, BuildAndAddMultiple1ByteArg_Ok )
{
	Core::CoreStatus success = Core::CoreStatus::CORE_ERROR;

	uint8_t params = 42;
	success = mFrame.Build(
		EClusters::BATTERY,
		EBatteryCommands::GET_VOLTAGE );
	mFrame.Set1ByteParam( 42U );
	mFrame.Set1ByteParam( 24U );
	mFrame.Set1ByteParam( 55U );
	mFrame.Set1ByteParam( 64U );
	mFrame.Set1ByteParam( 15U );

	EXPECT_EQ( mFrame.clusterId, Clusters::EClusters::BATTERY );
	EXPECT_EQ( mFrame.commandId, Clusters::EBatteryCommands::GET_VOLTAGE );
	EXPECT_EQ( mFrame.nbParams, 5U );
	EXPECT_EQ( mFrame.params[0U], 42U );
	EXPECT_EQ( mFrame.Get1ByteParam( 0U ), 42U );
	EXPECT_EQ( mFrame.params[1U], 24U );
	EXPECT_EQ( mFrame.Get1ByteParam( 1U ), 24U );
	EXPECT_EQ( mFrame.params[2U], 55U );
	EXPECT_EQ( mFrame.Get1ByteParam( 2U ), 55U );
	EXPECT_EQ( mFrame.params[3U], 64U );
	EXPECT_EQ( mFrame.Get1ByteParam( 3U ), 64U );
	EXPECT_EQ( mFrame.params[4U], 15U );
	EXPECT_EQ( mFrame.Get1ByteParam( 4U ), 15U );
	EXPECT_TRUE( success );
}

TEST_F( UT_CLU_FRAME, Build2BytesArg_Ok )
{
	Core::CoreStatus success = Core::CoreStatus::CORE_ERROR;

	uint16_t params = 42;
	success = mFrame.Build(
		EClusters::BATTERY,
		EBatteryCommands::GET_VOLTAGE,
		(uint8_t *) &params,
		2U );

	EXPECT_EQ( mFrame.clusterId, Clusters::EClusters::BATTERY );
	EXPECT_EQ( mFrame.commandId, Clusters::EBatteryCommands::GET_VOLTAGE );
	EXPECT_EQ( mFrame.nbParams, 2U );
	EXPECT_EQ( mFrame.params[0U], 42U );
	EXPECT_EQ( mFrame.params[1U], 0U );
	EXPECT_EQ( mFrame.Get2BytesParam( 0U ), 42U );
	EXPECT_TRUE( success );
}

TEST_F( UT_CLU_FRAME, BuildAndAdd2BytesArg_Ok )
{
	Core::CoreStatus success = Core::CoreStatus::CORE_ERROR;

	uint16_t params = 42;
	success = mFrame.Build(
		EClusters::BATTERY,
		EBatteryCommands::GET_VOLTAGE );
	mFrame.Set2BytesParam( 42U );

	EXPECT_EQ( mFrame.clusterId, Clusters::EClusters::BATTERY );
	EXPECT_EQ( mFrame.commandId, Clusters::EBatteryCommands::GET_VOLTAGE );
	EXPECT_EQ( mFrame.nbParams, 2U );
	EXPECT_EQ( mFrame.params[0U], 42U );
	EXPECT_EQ( mFrame.params[1U], 0U );
	EXPECT_EQ( mFrame.Get2BytesParam( 0U ), 42U );
	EXPECT_TRUE( success );
}

TEST_F( UT_CLU_FRAME, BuildAndAddMultiple2BytesArg_Ok )
{
	Core::CoreStatus success = Core::CoreStatus::CORE_ERROR;

	success = mFrame.Build(
		EClusters::BATTERY,
		EBatteryCommands::GET_VOLTAGE );
	mFrame.Set2BytesParam( 42U );
	mFrame.Set2BytesParam( 24U );
	mFrame.Set2BytesParam( 55U );
	mFrame.Set2BytesParam( 64U );
	mFrame.Set2BytesParam( 0x1500U );

	EXPECT_EQ( mFrame.clusterId, Clusters::EClusters::BATTERY );
	EXPECT_EQ( mFrame.commandId, Clusters::EBatteryCommands::GET_VOLTAGE );
	EXPECT_EQ( mFrame.nbParams, 10U );
	EXPECT_EQ( mFrame.params[0U], 42U );
	EXPECT_EQ( mFrame.params[1U], 0U );
	EXPECT_EQ( mFrame.Get2BytesParam( 0U ), 42U );
	EXPECT_EQ( mFrame.params[2U], 24U );
	EXPECT_EQ( mFrame.params[3U], 0U );
	EXPECT_EQ( mFrame.Get2BytesParam( 2U ), 24U );
	EXPECT_EQ( mFrame.params[4U], 55U );
	EXPECT_EQ( mFrame.params[5U], 0U );
	EXPECT_EQ( mFrame.Get2BytesParam( 4U ), 55U );
	EXPECT_EQ( mFrame.params[6U], 64U );
	EXPECT_EQ( mFrame.params[7U], 0U );
	EXPECT_EQ( mFrame.Get2BytesParam( 6U ), 64U );
	EXPECT_EQ( mFrame.params[8U], 0U );
	EXPECT_EQ( mFrame.params[9U], 0x15U );
	EXPECT_EQ( mFrame.Get2BytesParam( 8U ), 0x1500U );
	EXPECT_TRUE( success );
}

TEST_F( UT_CLU_FRAME, BuildAndAdd3BytesArg_Ok )
{
	Core::CoreStatus success = Core::CoreStatus::CORE_ERROR;

	uint32_t params = 0x424140;
	success = mFrame.Build(
		EClusters::BATTERY,
		EBatteryCommands::GET_VOLTAGE );
	mFrame.Set3BytesParam( params );

	EXPECT_EQ( mFrame.clusterId, Clusters::EClusters::BATTERY );
	EXPECT_EQ( mFrame.commandId, Clusters::EBatteryCommands::GET_VOLTAGE );
	EXPECT_EQ( mFrame.nbParams, 3U );
	EXPECT_EQ( mFrame.params[0U], 0x40U );
	EXPECT_EQ( mFrame.params[1U], 0x41U );
	EXPECT_EQ( mFrame.params[2U], 0x42U );
	EXPECT_EQ( mFrame.Get3BytesParam( 0U ), 0x424140U );
	EXPECT_TRUE( success );
}

TEST_F( UT_CLU_FRAME, BuildAndAddMultiple3BytesArg_Ok )
{
	Core::CoreStatus success = Core::CoreStatus::CORE_ERROR;

	uint32_t params24 = 0x424140U;
	uint16_t params16 = 0x5533U;
	uint8_t  params8  = 0xAAU;
	success = mFrame.Build(
		EClusters::BATTERY,
		EBatteryCommands::GET_VOLTAGE );
	mFrame.Set3BytesParam( params24 );
	mFrame.Set2BytesParam( params16 );
	mFrame.Set1ByteParam( params8 );

	EXPECT_EQ( mFrame.clusterId, Clusters::EClusters::BATTERY );
	EXPECT_EQ( mFrame.commandId, Clusters::EBatteryCommands::GET_VOLTAGE );
	EXPECT_EQ( mFrame.nbParams, 6U );
	EXPECT_EQ( mFrame.params[0U], 0x40U );
	EXPECT_EQ( mFrame.params[1U], 0x41U );
	EXPECT_EQ( mFrame.params[2U], 0x42U );
	EXPECT_EQ( mFrame.params[3U], 0x33U );
	EXPECT_EQ( mFrame.params[4U], 0x55U );
	EXPECT_EQ( mFrame.params[5U], 0xAAU );
	EXPECT_EQ( mFrame.Get3BytesParam( 0U ), 0x424140U );
	EXPECT_EQ( mFrame.Get2BytesParam( 3U ), 0x5533U );
	EXPECT_EQ( mFrame.Get1ByteParam( 5U ), 0xAAU );
	EXPECT_TRUE( success );
}

TEST_F( UT_CLU_FRAME, BuildAndAdd4BytesArg_Ok )
{
	Core::CoreStatus success = Core::CoreStatus::CORE_ERROR;

	uint32_t params = 0x42414039;
	success = mFrame.Build(
		EClusters::BATTERY,
		EBatteryCommands::GET_VOLTAGE );
	mFrame.Set4BytesParam( params );

	EXPECT_EQ( mFrame.clusterId, Clusters::EClusters::BATTERY );
	EXPECT_EQ( mFrame.commandId, Clusters::EBatteryCommands::GET_VOLTAGE );
	EXPECT_EQ( mFrame.nbParams, 4U );
	EXPECT_EQ( mFrame.params[0U], 0x39U );
	EXPECT_EQ( mFrame.params[1U], 0x40U );
	EXPECT_EQ( mFrame.params[2U], 0x41U );
	EXPECT_EQ( mFrame.params[3U], 0x42U );
	EXPECT_EQ( mFrame.Get4BytesParam( 0U ), 0x42414039U );
	EXPECT_TRUE( success );
}

TEST_F( UT_CLU_FRAME, BuildAndAddMultiple4BytesArg_Ok )
{
	Core::CoreStatus success = Core::CoreStatus::CORE_ERROR;

	uint32_t params32 = 0x42414039U;
	uint32_t params24 = 0x424140U;
	uint16_t params16 = 0x5533U;
	uint8_t  params8  = 0xAAU;
	success = mFrame.Build(
		EClusters::BATTERY,
		EBatteryCommands::GET_VOLTAGE );
	mFrame.Set3BytesParam( params24 );
	mFrame.Set2BytesParam( params16 );
	mFrame.Set1ByteParam( params8 );
	mFrame.Set4BytesParam( params32 );

	EXPECT_EQ( mFrame.clusterId, Clusters::EClusters::BATTERY );
	EXPECT_EQ( mFrame.commandId, Clusters::EBatteryCommands::GET_VOLTAGE );
	EXPECT_EQ( mFrame.nbParams, 10U );
	EXPECT_EQ( mFrame.params[0U], 0x40U );
	EXPECT_EQ( mFrame.params[1U], 0x41U );
	EXPECT_EQ( mFrame.params[2U], 0x42U );
	EXPECT_EQ( mFrame.params[3U], 0x33U );
	EXPECT_EQ( mFrame.params[4U], 0x55U );
	EXPECT_EQ( mFrame.params[5U], 0xAAU );
	EXPECT_EQ( mFrame.params[6U], 0x39U );
	EXPECT_EQ( mFrame.params[7U], 0x40U );
	EXPECT_EQ( mFrame.params[8U], 0x41U );
	EXPECT_EQ( mFrame.params[9U], 0x42U );
	EXPECT_EQ( mFrame.Get3BytesParam( 0U ), 0x424140U );
	EXPECT_EQ( mFrame.Get2BytesParam( 3U ), 0x5533U );
	EXPECT_EQ( mFrame.Get1ByteParam( 5U ), 0xAAU );
	EXPECT_EQ( mFrame.Get4BytesParam( 6U ), 0x42414039U );
	EXPECT_TRUE( success );
}

TEST_F( UT_CLU_FRAME, BuildAndAdd6BytesArg_Ok )
{
	Core::CoreStatus success = Core::CoreStatus::CORE_ERROR;

	uint64_t params = 0x424140394241U;
	success = mFrame.Build(
		EClusters::BATTERY,
		EBatteryCommands::GET_VOLTAGE );
	mFrame.Set6BytesParam( params );

	EXPECT_EQ( mFrame.clusterId, Clusters::EClusters::BATTERY );
	EXPECT_EQ( mFrame.commandId, Clusters::EBatteryCommands::GET_VOLTAGE );
	EXPECT_EQ( mFrame.nbParams, 6U );
	EXPECT_EQ( mFrame.params[0U], 0x41U );
	EXPECT_EQ( mFrame.params[1U], 0x42U );
	EXPECT_EQ( mFrame.params[2U], 0x39U );
	EXPECT_EQ( mFrame.params[3U], 0x40U );
	EXPECT_EQ( mFrame.params[4U], 0x41U );
	EXPECT_EQ( mFrame.params[5U], 0x42U );
	EXPECT_EQ( mFrame.Get6BytesParam( 0U ), 0x424140394241U );
	EXPECT_TRUE( success );
}

TEST_F( UT_CLU_FRAME, BuildAndAdd8BytesArg_Ok )
{
	Core::CoreStatus success = Core::CoreStatus::CORE_ERROR;

	uint64_t params = 0x4241403942414039U;
	success = mFrame.Build(
		EClusters::BATTERY,
		EBatteryCommands::GET_VOLTAGE );
	mFrame.Set8BytesParam( params );

	EXPECT_EQ( mFrame.clusterId, Clusters::EClusters::BATTERY );
	EXPECT_EQ( mFrame.commandId, Clusters::EBatteryCommands::GET_VOLTAGE );
	EXPECT_EQ( mFrame.nbParams, 8U );
	EXPECT_EQ( mFrame.params[0U], 0x39U );
	EXPECT_EQ( mFrame.params[1U], 0x40U );
	EXPECT_EQ( mFrame.params[2U], 0x41U );
	EXPECT_EQ( mFrame.params[3U], 0x42U );
	EXPECT_EQ( mFrame.params[4U], 0x39U );
	EXPECT_EQ( mFrame.params[5U], 0x40U );
	EXPECT_EQ( mFrame.params[6U], 0x41U );
	EXPECT_EQ( mFrame.params[7U], 0x42U );
	EXPECT_EQ( mFrame.Get8BytesParam( 0U ), 0x4241403942414039U );
	EXPECT_TRUE( success );
}
