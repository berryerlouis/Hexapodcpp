#include <gmock/gmock.h>
#include <gtest/gtest.h>

#include "../../../mock/drv/MockTwi.h"

#include "../../../../src/Component/Imu/Ahrs.h"

using ::testing::StrictMock;
using ::testing::_;
using ::testing::DoAll;
using ::testing::SetArgReferee;
using ::testing::Return;

namespace Component
{
	namespace Imu
	{
		class UT_CMP_AHRS : public ::testing::Test {
		protected:
			UT_CMP_AHRS() : mAhrs() {
			}

			virtual void SetUp() {
			}

			virtual void TearDown() {
			}

			virtual ~UT_CMP_AHRS() = default;

			/* Mocks */

			/* Test class */
			Ahrs mAhrs;
		};


		TEST_F(UT_CMP_AHRS, MadgwickQuaternionUpdate_Ok) {
			Vector3F mAcc;
			constexpr Vector3 mAccRaw = {1250, -556, -16600};
			mAcc.x = (mAccRaw.x * 2.0F) / 32768.0F;
			mAcc.y = (mAccRaw.y * 2.0F) / 32768.0F;
			mAcc.z = (mAccRaw.z * 2.0F) / 32768.0F;

			Vector3F mGyr;
			constexpr Vector3 mGyrRaw = {-21, -151, -65};
			mGyr.x = mGyrRaw.x * 250.0F / 32768.0F;
			mGyr.y = mGyrRaw.y * 250.0F / 32768.0F;
			mGyr.z = mGyrRaw.z * 250.0F / 32768.0F;

			Vector3F mMag;
			constexpr Vector3 mMagRaw = {19537, -16141, -11014};
			mMag.x = mMagRaw.x * 10.0F * 1229.0F / 4096.0F + 18.0F;
			mMag.y = mMagRaw.y * 10.0F * 1229.0F / 4096.0F + 70.0F;
			mMag.z = mMagRaw.z * 10.0F * 1229.0F / 4096.0F + 270.0F;

			constexpr float deltaTime = 0.01;
			Position3D mYawPitchRoll;

			mAhrs.MadgwickQuaternionUpdate(mAcc, mGyr, mMag, deltaTime);
			mAhrs.GetYawPitchRoll(mYawPitchRoll);
			mAhrs.MadgwickQuaternionUpdate(mAcc, mGyr, mMag, deltaTime);
			mAhrs.GetYawPitchRoll(mYawPitchRoll);
		}
	}
}
