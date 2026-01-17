#include <gmock/gmock.h>
#include <gtest/gtest.h>

#include "../../../mock/cmp/MockLed.h"
#include "../../../mock/cmp/MockSound.h"
#include "../../../mock/drv/MockGpio.h"
#include "../../../mock/srv/MockEventDispatcherInterface.h"
#include "../../../mock/srv/MockMessageListener.h"

#include "../../../../src/Cluster/Sound/ClusterSound.h"
#include "../../../../src/Component/Proximity/SensorProximity.h"
#include "../../../../src/Service/Sound/ServiceSound.h"

using ::testing::_;
using ::testing::Return;
using ::testing::ReturnRef;
using ::testing::StrictMock;

namespace Service
{
    namespace Sound
    {
        class UT_SRV_SOUND : public ::testing::Test {
        protected:
            UT_SRV_SOUND()
                : mMockGpioLeft()
                , mMockLedLeft()
                , mMockGpioRight()
                , mMockLedRight()
                , mMockEventDispatcherInterface()
                , mMockMessageInterface()
                , mSoundLeft(SOUND_LEFT, mMockGpioLeft, mMockLedLeft)
                , mSoundRight(SOUND_RIGHT, mMockGpioRight, mMockLedRight)
                , mServiceSound(mSoundLeft,
                                mSoundRight,
                                mMockMessageInterface,
                                mMockEventDispatcherInterface) {
            }

            virtual void SetUp() {
                Driver::Gpio::SGpio gpio;
                EXPECT_CALL(mMockGpioLeft, GetPin()).WillOnce(ReturnRef(gpio));
                EXPECT_CALL(mMockLedLeft, Initialize()).Times(1U);
                EXPECT_CALL(mMockLedLeft, Off())
                        .WillOnce(Return(Core::Status::CORE_OK));
                EXPECT_CALL(mMockGpioLeft, SetInterruptPin(_)).Times(1U);

                EXPECT_CALL(mMockGpioRight, GetPin()).WillOnce(ReturnRef(gpio));
                EXPECT_CALL(mMockLedRight, Initialize()).Times(1U);
                EXPECT_CALL(mMockLedRight, Off())
                        .WillOnce(Return(Core::Status::CORE_OK));
                EXPECT_CALL(mMockGpioRight, SetInterruptPin(_)).Times(1U);

                EXPECT_CALL(mMockEventDispatcherInterface, AddListener(_));
                EXPECT_EQ(Core::Status::CORE_OK, mServiceSound.Initialize());
            }

            virtual void TearDown() {
            }

            virtual ~UT_SRV_SOUND() = default;

            /* Mocks */
            StrictMock<Driver::Gpio::MockGpio>  mMockGpioLeft;
            StrictMock<Component::Led::MockLed> mMockLedLeft;
            StrictMock<Driver::Gpio::MockGpio>  mMockGpioRight;
            StrictMock<Component::Led::MockLed> mMockLedRight;
            StrictMock<Event::MockEventDispatcherInterface>
                    mMockEventDispatcherInterface;
            StrictMock<Message::MockMessageInterface> mMockMessageInterface;

            StrictMock<Component::Sound::Sound>       mSoundLeft;
            StrictMock<Component::Sound::Sound>       mSoundRight;

            /* Test class */
            ServiceSound mServiceSound;
        };

        TEST_F(UT_SRV_SOUND, Update) {
            mServiceSound.Update(0U);
        }
        TEST_F(UT_SRV_SOUND, OnEvent) {
            const Component::Proximity::SensorsStruct sensor(
                    {.id = Component::Proximity::SensorsId::SRF_LEFT,
                     .distance = 10U});
            const Event::Event event = Event::Event(
                    BATTERY, EventType::EVENT_SENSOR_UPDATE, sensor);
            mServiceSound.OnEvent(event);
        }
    } // namespace Sound
} // namespace Service
