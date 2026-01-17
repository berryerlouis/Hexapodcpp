#include <gmock/gmock.h>
#include <gtest/gtest.h>


#include "../../../mock/cmp/MockLed.h"
#include "../../../mock/drv/MockGpio.h"
#include "../../../mock/drv/MockInputCapture.h"

#include "../../../../src/Component/Proximity/Ultrasound/Srf05.h"

using ::testing::_;
using ::testing::Return;
using ::testing::ReturnRef;
using ::testing::StrictMock;


namespace Component
{
    namespace Proximity
    {
        namespace Ultrasound
        {
            class UT_CMP_SRF05 : public ::testing::Test {
            protected:
                UT_CMP_SRF05()
                    : mMockGpio()
                    , mMockInputCapture()
                    , mMockLed()
                    , mSrf05(Cluster::EProximityCommands::US_LEFT,
                             mMockGpio,
                             mMockInputCapture,
                             mMockLed) {
                }

                virtual void SetUp() {
                    Driver::Gpio::SGpio gpio;
                    EXPECT_CALL(mMockGpio, GetPin()).WillOnce(ReturnRef(gpio));
                    EXPECT_CALL(mMockInputCapture, Initialize())
                            .Times(1U)
                            .WillOnce(Return(Core::Status::CORE_OK));
                    EXPECT_CALL(mMockLed, Initialize())
                            .Times(1U)
                            .WillOnce(Return(Core::Status::CORE_OK));

                    Core::Status success = mSrf05.Initialize();

                    EXPECT_EQ(mSrf05.GetThreshold(), 30U);
                    EXPECT_EQ(success, Core::Status::CORE_OK);
                }

                virtual void TearDown() {
                }

                virtual ~UT_CMP_SRF05() = default;

                /* Mocks */
                StrictMock<Driver::Gpio::MockGpio> mMockGpio;
                StrictMock<Driver::InputCapture::MockInputCapture>
                                                    mMockInputCapture;
                StrictMock<Component::Led::MockLed> mMockLed;
                /* Test class */
                Srf05 mSrf05;
            };

            TEST_F(UT_CMP_SRF05, SetThreshold) {
                EXPECT_EQ(mSrf05.SetThreshold(35U), Core::Status::CORE_OK);
                EXPECT_EQ(35U, mSrf05.GetThreshold());
            }

            TEST_F(UT_CMP_SRF05, UnderThreshold) {
                // mSrf05.Detect();
                EXPECT_CALL(mMockInputCapture, GetInputCaptureTime())
                        .Times(1U)
                        .WillOnce(Return(58U));
                EXPECT_CALL(mMockGpio, Set())
                        .Times(1U)
                        .WillOnce(Return(Core::Status::CORE_OK));
                EXPECT_CALL(mMockGpio, Reset())
                        .Times(1U)
                        .WillOnce(Return(Core::Status::CORE_OK));
                EXPECT_CALL(mMockLed, On())
                        .Times(1U)
                        .WillOnce(Return(Core::Status::CORE_OK));

                mSrf05.Update(0U);
            }

            TEST_F(UT_CMP_SRF05, AboveThreshold) {
                // mSrf05.Detect();
                EXPECT_CALL(mMockInputCapture, GetInputCaptureTime())
                        .Times(1U)
                        .WillOnce(Return(58U * 31U));
                EXPECT_CALL(mMockGpio, Set())
                        .Times(1U)
                        .WillOnce(Return(Core::Status::CORE_OK));
                EXPECT_CALL(mMockGpio, Reset())
                        .Times(1U)
                        .WillOnce(Return(Core::Status::CORE_OK));
                EXPECT_CALL(mMockLed, Off())
                        .Times(1U)
                        .WillOnce(Return(Core::Status::CORE_OK));

                mSrf05.Update(0U);
            }
        } // namespace Ultrasound
    } // namespace Proximity
} // namespace Component
