#include <gmock/gmock.h>
#include <gtest/gtest.h>

#include "../../../mock/cmp/MockCommunication.h"
#include "../../../mock/clu/MockClusters.h"
#include "../../../mock/cor/MockEventMediatorInterface.h"

#include "../../../../src/Service/Communication/ServiceCommunication.h"

using ::testing::_;
using ::testing::Return;
using ::testing::StrictMock;

namespace Service {
namespace Communication {
class UT_SRV_COMMUNICATION : public ::testing::Test {
protected:
	UT_SRV_COMMUNICATION() :
		mMockCommunication(),
		mMockClusters(),
		mMockEventMediatorInterface(),
		mServiceCommunication( mMockCommunication, mMockClusters )
	{
	}

	virtual void SetUp ()
	{
		EXPECT_CALL( mMockCommunication, Initialize() ).WillOnce( Return( Core::CoreStatus::CORE_ERROR ) );
		EXPECT_FALSE( mServiceCommunication.Initialize() );

		EXPECT_CALL( mMockCommunication, Initialize() ).WillOnce( Return( Core::CoreStatus::CORE_OK ) );
		EXPECT_TRUE( mServiceCommunication.Initialize() );
		mServiceCommunication.setMediator( &mMockEventMediatorInterface );
	}

	virtual void TearDown ()
	{
	}

	virtual ~UT_SRV_COMMUNICATION() = default;

	/* Mocks */
	StrictMock <Component::Communication::MockCommunication> mMockCommunication;
	StrictMock <Cluster::Clusters::MockClusters> mMockClusters;
	StrictMock <Core::MockEventMediatorInterface> mMockEventMediatorInterface;

	/* Test class */
	ServiceCommunication mServiceCommunication;
};

TEST_F( UT_SRV_COMMUNICATION, Initialize_NotifyNominal )
{
	EXPECT_CALL( mMockCommunication, Update( 12450UL ) ).Times( 1U );

	mServiceCommunication.Update( 12450UL );
}
}
}
