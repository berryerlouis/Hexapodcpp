#include <gmock/gmock.h>
#include <gtest/gtest.h>


#include "../../../mock/cmp/MockLed.h"
#include "../../../mock/drv/MockUart.h"
#include "../../../mock/clu/MockClusters.h"


#include "../../../../src/clu/Protocol.h"
#include "../../../../src/cmp/Communication.h"

using ::testing::_;
using ::testing::Return;
using ::testing::StrictMock;
using ::testing::Matcher;
using namespace Component;

TEST(ComponentCommunication, Initialize_Ok)
{
	bool success = false;
	StrictMock <MockLed>      led;
	StrictMock <MockUart>     uart;
	StrictMock <MockClusters> clusters;

	Communication comm(uart, clusters, led);

	EXPECT_CALL(led, Initialize() ).WillOnce(Return(true) );

	success = comm.Initialize();

	EXPECT_TRUE(success);
}

TEST(ComponentCommunication, Update_Ok_Noframe)
{
	bool success = false;
	StrictMock <MockLed>      led;
	StrictMock <MockUart>     uart;
	StrictMock <MockClusters> clusters;

	Communication comm(uart, clusters, led);

	EXPECT_CALL(led, Initialize() ).WillOnce(Return(true) );

	EXPECT_CALL(uart, DataAvailable() ).WillOnce(Return(0U) );

	success = comm.Initialize();
	comm.Update(0UL);

	EXPECT_TRUE(success);
}

TEST(ComponentCommunication, Update_Ok_1frame)
{
	bool success = false;
	StrictMock <MockLed>      led;
	StrictMock <MockUart>     uart;
	StrictMock <MockClusters> clusters;
	const char *  bufferRx = "<000000>";
	Communication comm(uart, clusters, led);

	EXPECT_CALL(led, Initialize() ).WillOnce(Return(true) );

	EXPECT_CALL(led, On() ).WillOnce(Return(true) );
	EXPECT_CALL(led, Off() ).WillOnce(Return(true) );
	EXPECT_CALL(uart, Send(Matcher <const uint8_t *>(_), _) ).Times(1U);
	EXPECT_CALL(clusters, GetCluster(0) ).Times(1U);

	success = comm.Initialize();

	for (size_t i = 0; i < strlen(bufferRx); i++)
	{
		EXPECT_CALL(uart, DataAvailable() ).WillOnce(Return(8U) );
		EXPECT_CALL(uart, Read() ).WillOnce(Return(bufferRx[i]) );
		comm.Update(0UL);
	}

	EXPECT_TRUE(success);
}

TEST(ComponentCommunication, Update_Ko_1frame)
{
	bool success = false;
	StrictMock <MockLed>      led;
	StrictMock <MockUart>     uart;
	StrictMock <MockClusters> clusters;
	const char *  bufferRx = "<0<0000>";
	Communication comm(uart, clusters, led);

	EXPECT_CALL(led, Initialize() ).WillOnce(Return(true) );

	EXPECT_CALL(led, On() ).WillOnce(Return(true) );
	EXPECT_CALL(led, Off() ).WillOnce(Return(true) );
	EXPECT_CALL(uart, Send(Matcher <const uint8_t *>(_), _) ).Times(1U);

	success = comm.Initialize();

	for (size_t i = 0; i < strlen(bufferRx); i++)
	{
		EXPECT_CALL(uart, DataAvailable() ).WillOnce(Return(8U) );
		EXPECT_CALL(uart, Read() ).WillOnce(Return(bufferRx[i]) );
		comm.Update(0UL);
	}

	EXPECT_TRUE(success);
}
