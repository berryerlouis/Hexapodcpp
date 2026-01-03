#include <gmock/gmock.h>
#include <gtest/gtest.h>

#include "../../../../src/Move/Gait/GaitCycle.h"
#include "../../../mock/bot/MockLeg.h"
#include "../../../mock/bot/MockLegs.h"

using ::testing::_;
using ::testing::Return;
using ::testing::StrictMock;

namespace Move
{
    namespace Gait
    {
        class UT_MOVE_GAIT_CYCLE : public ::testing::Test {
        protected:
            UT_MOVE_GAIT_CYCLE()
                : mMockLegs()
                , mMockLeg()
                , gaitParams()
                , gaitCycle(mMockLegs, gaitParams) {
            }

            void SetUp() override {
                // Setup default mock leg behavior
                for (int i = 0; i < 6; i++) {
                    ON_CALL(mMockLegs, GetLeg(_))
                            .WillByDefault(Return(&mMockLeg[i]));
                }
            }

            void TearDown() override {
            }

            virtual ~UT_MOVE_GAIT_CYCLE() = default;

            /* Mocks */
            StrictMock<Bot::Legs::MockLegs> mMockLegs;
            StrictMock<Bot::Leg::MockLeg>   mMockLeg[6U];

            /* Class */
            GaitParams gaitParams;
            GaitCycle  gaitCycle;
        };

        // Test Start
        TEST_F(UT_MOVE_GAIT_CYCLE, TestStart) {
            gaitParams.SetRunning(false);
            EXPECT_TRUE(gaitCycle.Start());
            EXPECT_TRUE(gaitParams.IsRunning());

            // Second start should return false (already
            // running)
            EXPECT_FALSE(gaitCycle.Start());
        }

        // Test Pause
        TEST_F(UT_MOVE_GAIT_CYCLE, TestPause) {
            gaitParams.SetRunning(true);
            EXPECT_TRUE(gaitCycle.Pause());
            EXPECT_FALSE(gaitParams.IsRunning());

            // Second pause should return false (already
            // paused)
            EXPECT_FALSE(gaitCycle.Pause());
        }

        // Test Update when not running
        TEST_F(UT_MOVE_GAIT_CYCLE, TestUpdateNotRunning) {
            gaitParams.SetRunning(false);
            gaitCycle.Start(); // Start to initialize the
                               // start time

            // Mock map to return from GetLegs
            std::map<Bot::Legs::ELeg, Bot::Leg::Leg> mockLegsMap;

            // When not running but cycle not complete,
            // should call GetLegs and Update
            EXPECT_CALL(mMockLegs, GetLegs())
                    .Times(1)
                    .WillOnce(::testing::ReturnRef(mockLegsMap));
            EXPECT_CALL(mMockLegs, Update())
                    .Times(1)
                    .WillOnce(Return(Core::Status::CORE_OK));

            gaitCycle.Pause(); // Pause to set running to
                               // false
            gaitCycle.Update(100U); // Update before cycle completes
        } // Test Update when running
        TEST_F(UT_MOVE_GAIT_CYCLE, TestUpdateRunning) {
            gaitParams.SetRunning(true);
            gaitCycle.Start();

            // Mock map to return from GetLegs
            std::map<Bot::Legs::ELeg, Bot::Leg::Leg> mockLegsMap;

            // Update should call GetLegs to get the map,
            // then Update
            EXPECT_CALL(mMockLegs, GetLegs())
                    .Times(1)
                    .WillOnce(::testing::ReturnRef(mockLegsMap));
            EXPECT_CALL(mMockLegs, Update())
                    .Times(1)
                    .WillOnce(Return(Core::Status::CORE_OK));

            gaitCycle.Update(500U);
        }

        // Test cycle completion
        TEST_F(UT_MOVE_GAIT_CYCLE, TestCycleCompletion) {
            gaitParams.SetRunning(true);
            gaitParams.SetCycleDuration(1000U);

            // Mock map to return from GetLegs
            std::map<Bot::Legs::ELeg, Bot::Leg::Leg> mockLegsMap;

            // Start cycle
            gaitCycle.Start();

            // Update at 500ms (mid-cycle) - should call
            // GetLegs once and Update once
            EXPECT_CALL(mMockLegs, GetLegs())
                    .Times(1)
                    .WillOnce(::testing::ReturnRef(mockLegsMap));
            EXPECT_CALL(mMockLegs, Update())
                    .Times(1)
                    .WillOnce(Return(Core::Status::CORE_OK));
            gaitCycle.Update(500U);

            // Update at 1100ms (cycle complete) - should
            // call GetLegs once and Update once
            EXPECT_CALL(mMockLegs, GetLegs())
                    .Times(1)
                    .WillOnce(::testing::ReturnRef(mockLegsMap));
            EXPECT_CALL(mMockLegs, Update())
                    .Times(1)
                    .WillOnce(Return(Core::Status::CORE_OK));
            gaitCycle.Update(1100U);
        }
    } // namespace Gait
} // namespace Move
