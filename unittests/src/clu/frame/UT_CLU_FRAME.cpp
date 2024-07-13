#include <gmock/gmock.h>
#include <gtest/gtest.h>

#include "../../../../src/clu/Constants.h"
#include "../../../../src/clu/Frame.h"

using ::testing::_;
using ::testing::Return;
using ::testing::StrictMock;

using namespace Clusters;

TEST( Frame, BuildNoArg_Ok )
{
	Core::CoreStatus success = Core::CoreStatus::CORE_ERROR;
	Clusters::Frame  response;

	success = response.Build(
		EClusters::BATTERY,
		EBatteryCommands::GET_VOLTAGE );

	EXPECT_EQ( response.clusterId, Clusters::EClusters::BATTERY );
	EXPECT_EQ( response.commandId, Clusters::EBatteryCommands::GET_VOLTAGE );
	EXPECT_EQ( response.nbParams, 0U );
	EXPECT_EQ( response.params[0U], 0U );
	EXPECT_TRUE( success );
}

TEST( Frame, BuildNoArgNullptr_Ok )
{
	Core::CoreStatus success = Core::CoreStatus::CORE_ERROR;
	Clusters::Frame  response;

	success = response.Build(
		EClusters::BATTERY,
		EBatteryCommands::GET_VOLTAGE,
		nullptr,
		0U );

	EXPECT_EQ( response.clusterId, Clusters::EClusters::BATTERY );
	EXPECT_EQ( response.commandId, Clusters::EBatteryCommands::GET_VOLTAGE );
	EXPECT_EQ( response.nbParams, 0U );
	EXPECT_EQ( response.params[0U], 0U );
	EXPECT_TRUE( success );
}

TEST( Frame, Build1ByteArg_Ok )
{
	Core::CoreStatus success = Core::CoreStatus::CORE_ERROR;
	Clusters::Frame  response;

	uint8_t params = 42;
	success = response.Build(
		EClusters::BATTERY,
		EBatteryCommands::GET_VOLTAGE,
		&params,
		1U );

	EXPECT_EQ( response.clusterId, Clusters::EClusters::BATTERY );
	EXPECT_EQ( response.commandId, Clusters::EBatteryCommands::GET_VOLTAGE );
	EXPECT_EQ( response.nbParams, 1U );
	EXPECT_EQ( response.params[0U], 42U );
	EXPECT_EQ( response.Get1ByteParam( 0U ), 42U );
	EXPECT_TRUE( success );
}

TEST( Frame, BuildAndAdd1ByteArg_Ok )
{
	Core::CoreStatus success = Core::CoreStatus::CORE_ERROR;
	Clusters::Frame  response;

	uint8_t params = 42;
	success = response.Build(
		EClusters::BATTERY,
		EBatteryCommands::GET_VOLTAGE );
	response.Set1ByteParam( 42U );

	EXPECT_EQ( response.clusterId, Clusters::EClusters::BATTERY );
	EXPECT_EQ( response.commandId, Clusters::EBatteryCommands::GET_VOLTAGE );
	EXPECT_EQ( response.nbParams, 1U );
	EXPECT_EQ( response.params[0U], 42U );
	EXPECT_EQ( response.Get1ByteParam( 0U ), 42U );
	EXPECT_TRUE( success );
}

TEST( Frame, BuildAndAddMultiple1ByteArg_Ok )
{
	Core::CoreStatus success = Core::CoreStatus::CORE_ERROR;
	Clusters::Frame  response;

	uint8_t params = 42;
	success = response.Build(
		EClusters::BATTERY,
		EBatteryCommands::GET_VOLTAGE );
	response.Set1ByteParam( 42U );
	response.Set1ByteParam( 24U );
	response.Set1ByteParam( 55U );
	response.Set1ByteParam( 64U );
	response.Set1ByteParam( 15U );

	EXPECT_EQ( response.clusterId, Clusters::EClusters::BATTERY );
	EXPECT_EQ( response.commandId, Clusters::EBatteryCommands::GET_VOLTAGE );
	EXPECT_EQ( response.nbParams, 5U );
	EXPECT_EQ( response.params[0U], 42U );
	EXPECT_EQ( response.Get1ByteParam( 0U ), 42U );
	EXPECT_EQ( response.params[1U], 24U );
	EXPECT_EQ( response.Get1ByteParam( 1U ), 24U );
	EXPECT_EQ( response.params[2U], 55U );
	EXPECT_EQ( response.Get1ByteParam( 2U ), 55U );
	EXPECT_EQ( response.params[3U], 64U );
	EXPECT_EQ( response.Get1ByteParam( 3U ), 64U );
	EXPECT_EQ( response.params[4U], 15U );
	EXPECT_EQ( response.Get1ByteParam( 4U ), 15U );
	EXPECT_TRUE( success );
}

TEST( Frame, Build2BytesArg_Ok )
{
	Core::CoreStatus success = Core::CoreStatus::CORE_ERROR;
	Clusters::Frame  response;

	uint16_t params = 42;
	success = response.Build(
		EClusters::BATTERY,
		EBatteryCommands::GET_VOLTAGE,
		(uint8_t *) &params,
		2U );

	EXPECT_EQ( response.clusterId, Clusters::EClusters::BATTERY );
	EXPECT_EQ( response.commandId, Clusters::EBatteryCommands::GET_VOLTAGE );
	EXPECT_EQ( response.nbParams, 2U );
	EXPECT_EQ( response.params[0U], 42U );
	EXPECT_EQ( response.params[1U], 0U );
	EXPECT_EQ( response.Get2BytesParam( 0U ), 42U );
	EXPECT_TRUE( success );
}

TEST( Frame, BuildAndAdd2BytesArg_Ok )
{
	Core::CoreStatus success = Core::CoreStatus::CORE_ERROR;
	Clusters::Frame  response;

	uint16_t params = 42;
	success = response.Build(
		EClusters::BATTERY,
		EBatteryCommands::GET_VOLTAGE );
	response.Set2BytesParam( 42U );

	EXPECT_EQ( response.clusterId, Clusters::EClusters::BATTERY );
	EXPECT_EQ( response.commandId, Clusters::EBatteryCommands::GET_VOLTAGE );
	EXPECT_EQ( response.nbParams, 2U );
	EXPECT_EQ( response.params[0U], 42U );
	EXPECT_EQ( response.params[1U], 0U );
	EXPECT_EQ( response.Get2BytesParam( 0U ), 42U );
	EXPECT_TRUE( success );
}

TEST( Frame, BuildAndAddMultiple2BytesArg_Ok )
{
	Core::CoreStatus success = Core::CoreStatus::CORE_ERROR;
	Clusters::Frame  response;

	success = response.Build(
		EClusters::BATTERY,
		EBatteryCommands::GET_VOLTAGE );
	response.Set2BytesParam( 42U );
	response.Set2BytesParam( 24U );
	response.Set2BytesParam( 55U );
	response.Set2BytesParam( 64U );
	response.Set2BytesParam( 0x1500U );

	EXPECT_EQ( response.clusterId, Clusters::EClusters::BATTERY );
	EXPECT_EQ( response.commandId, Clusters::EBatteryCommands::GET_VOLTAGE );
	EXPECT_EQ( response.nbParams, 10U );
	EXPECT_EQ( response.params[0U], 42U );
	EXPECT_EQ( response.params[1U], 0U );
	EXPECT_EQ( response.Get2BytesParam( 0U ), 42U );
	EXPECT_EQ( response.params[2U], 24U );
	EXPECT_EQ( response.params[3U], 0U );
	EXPECT_EQ( response.Get2BytesParam( 2U ), 24U );
	EXPECT_EQ( response.params[4U], 55U );
	EXPECT_EQ( response.params[5U], 0U );
	EXPECT_EQ( response.Get2BytesParam( 4U ), 55U );
	EXPECT_EQ( response.params[6U], 64U );
	EXPECT_EQ( response.params[7U], 0U );
	EXPECT_EQ( response.Get2BytesParam( 6U ), 64U );
	EXPECT_EQ( response.params[8U], 0U );
	EXPECT_EQ( response.params[9U], 0x15U );
	EXPECT_EQ( response.Get2BytesParam( 8U ), 0x1500U );
	EXPECT_TRUE( success );
}

TEST( Frame, BuildAndAdd3BytesArg_Ok )
{
	Core::CoreStatus success = Core::CoreStatus::CORE_ERROR;
	Clusters::Frame  response;

	uint32_t params = 0x424140;
	success = response.Build(
		EClusters::BATTERY,
		EBatteryCommands::GET_VOLTAGE );
	response.Set3BytesParam( params );

	EXPECT_EQ( response.clusterId, Clusters::EClusters::BATTERY );
	EXPECT_EQ( response.commandId, Clusters::EBatteryCommands::GET_VOLTAGE );
	EXPECT_EQ( response.nbParams, 3U );
	EXPECT_EQ( response.params[0U], 0x40U );
	EXPECT_EQ( response.params[1U], 0x41U );
	EXPECT_EQ( response.params[2U], 0x42U );
	EXPECT_EQ( response.Get3BytesParam( 0U ), 0x424140U );
	EXPECT_TRUE( success );
}

TEST( Frame, BuildAndAddMultiple3BytesArg_Ok )
{
	Core::CoreStatus success = Core::CoreStatus::CORE_ERROR;
	Clusters::Frame  response;

	uint32_t params24 = 0x424140U;
	uint16_t params16 = 0x5533U;
	uint8_t  params8  = 0xAAU;
	success = response.Build(
		EClusters::BATTERY,
		EBatteryCommands::GET_VOLTAGE );
	response.Set3BytesParam( params24 );
	response.Set2BytesParam( params16 );
	response.Set1ByteParam( params8 );

	EXPECT_EQ( response.clusterId, Clusters::EClusters::BATTERY );
	EXPECT_EQ( response.commandId, Clusters::EBatteryCommands::GET_VOLTAGE );
	EXPECT_EQ( response.nbParams, 6U );
	EXPECT_EQ( response.params[0U], 0x40U );
	EXPECT_EQ( response.params[1U], 0x41U );
	EXPECT_EQ( response.params[2U], 0x42U );
	EXPECT_EQ( response.params[3U], 0x33U );
	EXPECT_EQ( response.params[4U], 0x55U );
	EXPECT_EQ( response.params[5U], 0xAAU );
	EXPECT_EQ( response.Get3BytesParam( 0U ), 0x424140U );
	EXPECT_EQ( response.Get2BytesParam( 3U ), 0x5533U );
	EXPECT_EQ( response.Get1ByteParam( 5U ), 0xAAU );
	EXPECT_TRUE( success );
}

TEST( Frame, BuildAndAdd4BytesArg_Ok )
{
	Core::CoreStatus success = Core::CoreStatus::CORE_ERROR;
	Clusters::Frame  response;

	uint32_t params = 0x42414039;
	success = response.Build(
		EClusters::BATTERY,
		EBatteryCommands::GET_VOLTAGE );
	response.Set4BytesParam( params );

	EXPECT_EQ( response.clusterId, Clusters::EClusters::BATTERY );
	EXPECT_EQ( response.commandId, Clusters::EBatteryCommands::GET_VOLTAGE );
	EXPECT_EQ( response.nbParams, 4U );
	EXPECT_EQ( response.params[0U], 0x39U );
	EXPECT_EQ( response.params[1U], 0x40U );
	EXPECT_EQ( response.params[2U], 0x41U );
	EXPECT_EQ( response.params[3U], 0x42U );
	EXPECT_EQ( response.Get4BytesParam( 0U ), 0x42414039U );
	EXPECT_TRUE( success );
}

TEST( Frame, BuildAndAddMultiple4BytesArg_Ok )
{
	Core::CoreStatus success = Core::CoreStatus::CORE_ERROR;
	Clusters::Frame  response;

	uint32_t params32 = 0x42414039U;
	uint32_t params24 = 0x424140U;
	uint16_t params16 = 0x5533U;
	uint8_t  params8  = 0xAAU;
	success = response.Build(
		EClusters::BATTERY,
		EBatteryCommands::GET_VOLTAGE );
	response.Set3BytesParam( params24 );
	response.Set2BytesParam( params16 );
	response.Set1ByteParam( params8 );
	response.Set4BytesParam( params32 );

	EXPECT_EQ( response.clusterId, Clusters::EClusters::BATTERY );
	EXPECT_EQ( response.commandId, Clusters::EBatteryCommands::GET_VOLTAGE );
	EXPECT_EQ( response.nbParams, 10U );
	EXPECT_EQ( response.params[0U], 0x40U );
	EXPECT_EQ( response.params[1U], 0x41U );
	EXPECT_EQ( response.params[2U], 0x42U );
	EXPECT_EQ( response.params[3U], 0x33U );
	EXPECT_EQ( response.params[4U], 0x55U );
	EXPECT_EQ( response.params[5U], 0xAAU );
	EXPECT_EQ( response.params[6U], 0x39U );
	EXPECT_EQ( response.params[7U], 0x40U );
	EXPECT_EQ( response.params[8U], 0x41U );
	EXPECT_EQ( response.params[9U], 0x42U );
	EXPECT_EQ( response.Get3BytesParam( 0U ), 0x424140U );
	EXPECT_EQ( response.Get2BytesParam( 3U ), 0x5533U );
	EXPECT_EQ( response.Get1ByteParam( 5U ), 0xAAU );
	EXPECT_EQ( response.Get4BytesParam( 6U ), 0x42414039U );
	EXPECT_TRUE( success );
}

TEST( Frame, BuildAndAdd6BytesArg_Ok )
{
	Core::CoreStatus success = Core::CoreStatus::CORE_ERROR;
	Clusters::Frame  response;

	uint64_t params = 0x424140394241U;
	success = response.Build(
		EClusters::BATTERY,
		EBatteryCommands::GET_VOLTAGE );
	response.Set6BytesParam( params );

	EXPECT_EQ( response.clusterId, Clusters::EClusters::BATTERY );
	EXPECT_EQ( response.commandId, Clusters::EBatteryCommands::GET_VOLTAGE );
	EXPECT_EQ( response.nbParams, 6U );
	EXPECT_EQ( response.params[0U], 0x41U );
	EXPECT_EQ( response.params[1U], 0x42U );
	EXPECT_EQ( response.params[2U], 0x39U );
	EXPECT_EQ( response.params[3U], 0x40U );
	EXPECT_EQ( response.params[4U], 0x41U );
	EXPECT_EQ( response.params[5U], 0x42U );
	EXPECT_EQ( response.Get6BytesParam( 0U ), 0x424140394241U );
	EXPECT_TRUE( success );
}

TEST( Frame, BuildAndAdd8BytesArg_Ok )
{
	Core::CoreStatus success = Core::CoreStatus::CORE_ERROR;
	Clusters::Frame  response;

	uint64_t params = 0x4241403942414039U;
	success = response.Build(
		EClusters::BATTERY,
		EBatteryCommands::GET_VOLTAGE );
	response.Set8BytesParam( params );

	EXPECT_EQ( response.clusterId, Clusters::EClusters::BATTERY );
	EXPECT_EQ( response.commandId, Clusters::EBatteryCommands::GET_VOLTAGE );
	EXPECT_EQ( response.nbParams, 8U );
	EXPECT_EQ( response.params[0U], 0x39U );
	EXPECT_EQ( response.params[1U], 0x40U );
	EXPECT_EQ( response.params[2U], 0x41U );
	EXPECT_EQ( response.params[3U], 0x42U );
	EXPECT_EQ( response.params[4U], 0x39U );
	EXPECT_EQ( response.params[5U], 0x40U );
	EXPECT_EQ( response.params[6U], 0x41U );
	EXPECT_EQ( response.params[7U], 0x42U );
	EXPECT_EQ( response.Get8BytesParam( 0U ), 0x4241403942414039U );
	EXPECT_TRUE( success );
}
