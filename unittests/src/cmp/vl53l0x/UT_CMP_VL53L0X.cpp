#include <gmock/gmock.h>
#include <gtest/gtest.h>


#include "../../../mock/drv/MockTwi.h"

#include "../../../../src/Component/Proximity/Laser/Vl53l0x.h"
#include "../../../../src/Component/Proximity/SensorProximityInterface.h"
#include "../../../mock/cmp/MockLed.h"

using ::testing::_;
using ::testing::DoAll;
using ::testing::Return;
using ::testing::SetArgPointee;
using ::testing::SetArgReferee;
using ::testing::StrictMock;


namespace Component
{
    namespace Proximity
    {
        namespace Laser
        {
            class UT_CMP_VL53L0X : public ::testing::Test {
            protected:
                UT_CMP_VL53L0X()
                    : mMockTwi()
                    , mVl53l0x(mMockTwi, mMockLed) {
                }

                virtual void SetUp() {
                    Core::Status        success = Core::Status::CORE_ERROR;
                    ::testing::Sequence sRead;
                    ::testing::Sequence sWrite;

                    EXPECT_CALL(mMockLed, Initialize());
                    EXPECT_CALL(mMockTwi, WriteRegister(_, 0xBF, 0x00))
                            .InSequence(sWrite)
                            .WillOnce(Return(true));
                    EXPECT_CALL(mMockTwi,
                                ReadRegister(_, Vl53l0x::VL53L0X_IDENTIFICATION_MODEL_ID, _))
                            .InSequence(sRead)
                            .WillOnce(DoAll(SetArgReferee<2U>(0x00U), Return(true)));
                    EXPECT_CALL(mMockTwi, WriteRegister(_, 0xBF, 0x01))
                            .InSequence(sWrite)
                            .WillOnce(Return(true));
                    EXPECT_CALL(mMockTwi,
                                ReadRegister(_, Vl53l0x::VL53L0X_IDENTIFICATION_MODEL_ID, _))
                            .InSequence(sRead)
                            .WillOnce(DoAll(SetArgReferee<2U>(0xEEU), Return(true)));

                    EXPECT_CALL(mMockTwi, ReadRegister(_, _, _))
                            .InSequence(sRead)
                            .WillRepeatedly(DoAll(SetArgReferee<2U>(0x07U), Return(true)));
                    EXPECT_CALL(mMockTwi, WriteRegister(_, _, _))
                            .InSequence(sWrite)
                            .WillRepeatedly(Return(true));
                    EXPECT_CALL(mMockTwi, ReadRegisters(_, _, _, _)).WillRepeatedly(Return(true));
                    EXPECT_CALL(mMockTwi, WriteRegisters(_, _, _, _)).WillRepeatedly(Return(true));
                    EXPECT_CALL(mMockTwi, ReadRegister16Bits(_, _, _)).WillRepeatedly(Return(true));
                    EXPECT_CALL(mMockTwi, WriteRegister16Bits(_, _, _))
                            .WillRepeatedly(Return(true));
                    EXPECT_CALL(mMockTwi, WriteRegister32Bits(_, _, _))
                            .WillRepeatedly(Return(true));

                    success = mVl53l0x.Initialize();

                    EXPECT_EQ(success, Core::Status::CORE_OK);
                }

                virtual void TearDown() {
                }

                virtual ~UT_CMP_VL53L0X() = default;

                /* Mocks */
                StrictMock<Component::Led::MockLed> mMockLed;
                StrictMock<Driver::Twi::MockTwi>    mMockTwi;

                /* Test class */
                Vl53l0x mVl53l0x;
            };

            TEST_F(UT_CMP_VL53L0X, Update_Ok) {
                ::testing::Sequence s;
                EXPECT_CALL(mMockLed, Off());
                EXPECT_CALL(mMockTwi, ReadRegister(_, _, _))
                        .InSequence(s)
                        .WillOnce(DoAll(SetArgReferee<2U>(0x07U), Return(true)));
                EXPECT_CALL(mMockTwi, ReadRegister(_, _, _))
                        .InSequence(s)
                        .WillOnce(DoAll(SetArgReferee<2U>(0x00U), Return(true)));
                EXPECT_CALL(mMockTwi, ReadRegister(_, _, _))
                        .InSequence(s)
                        .WillOnce(DoAll(SetArgReferee<2U>(0x07U), Return(true)));
                mVl53l0x.Update(0U);
            }

            TEST_F(UT_CMP_VL53L0X, SetThreshold) {
                EXPECT_EQ(Core::Status::CORE_OK, mVl53l0x.SetThreshold(350U));
                EXPECT_EQ(350U, mVl53l0x.GetThreshold());
            }
        } // namespace Laser
    } // namespace Proximity
} // namespace Component
