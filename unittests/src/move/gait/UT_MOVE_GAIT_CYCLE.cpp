#include <gtest/gtest.h>
#include <gmock/gmock.h>

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
            UT_MOVE_GAIT_CYCLE() :
                          mMockLegs()
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
            StrictMock<Bot::Leg::MockLeg> mMockLeg[6U];

            /* Class */
            GaitParams gaitParams;
            GaitCycle gaitCycle;
        };

        // Test Start
        TEST_F(UT_MOVE_GAIT_CYCLE, TestStart) {
            gaitParams.SetRunning(false);
            EXPECT_TRUE(gaitCycle.Start());
            EXPECT_TRUE(gaitParams.IsRunning());
            
            // Second start should return false (already running)
            EXPECT_FALSE(gaitCycle.Start());
        }

        // Test Pause
        TEST_F(UT_MOVE_GAIT_CYCLE, TestPause) {
            gaitParams.SetRunning(true);
            EXPECT_TRUE(gaitCycle.Pause());
            EXPECT_FALSE(gaitParams.IsRunning());
            
            // Second pause should return false (already paused)
            EXPECT_FALSE(gaitCycle.Pause());
        }

        // Test Stop
        TEST_F(UT_MOVE_GAIT_CYCLE, TestStop) {
            gaitParams.SetRunning(true);
            
            // Expect ResetLegs to be called
            EXPECT_CALL(mMockLegs, ResetLegs(gaitParams.GetCycleDuration()))
                .Times(1);
            
            EXPECT_TRUE(gaitCycle.Stop());
            EXPECT_FALSE(gaitParams.IsRunning());
        }

        // Test Update when not running
        TEST_F(UT_MOVE_GAIT_CYCLE, TestUpdateNotRunning) {
            gaitParams.SetRunning(false);
            
            // Update should not call GetLeg when not running
            EXPECT_CALL(mMockLegs, GetLeg(_))
                .Times(0);
            
            gaitCycle.Update(1000U);
        }

        // Test Update when running
        TEST_F(UT_MOVE_GAIT_CYCLE, TestUpdateRunning) {
            gaitParams.SetRunning(true);
            gaitCycle.Start();
            
            // Update should call SetTarget and Update on mMockLegs
            EXPECT_CALL(mMockLegs, SetTarget(_, _, _, _))
                .Times(1);
            EXPECT_CALL(mMockLegs, Update())
                .Times(1)
                .WillOnce(Return(Core::Status::CORE_OK));
            
            gaitCycle.Update(500U);
        }

        // Test cycle completion
        TEST_F(UT_MOVE_GAIT_CYCLE, TestCycleCompletion) {
            gaitParams.SetRunning(true);
            gaitParams.SetCycleDuration(1000U);
            
            // Start cycle
            gaitCycle.Start();
            
            // Update at 500ms (mid-cycle) - should call SetTarget once and Update once
            EXPECT_CALL(mMockLegs, SetTarget(_, _, _, _))
                .Times(1);
            EXPECT_CALL(mMockLegs, Update())
                .Times(1)
                .WillOnce(Return(Core::Status::CORE_OK));
            gaitCycle.Update(500U);
            
            // Update at 1100ms (cycle complete) - should call SetTarget twice (normal + restart at 0.0F) and Update once
            EXPECT_CALL(mMockLegs, SetTarget(_, _, _, _))
                .Times(2);
            EXPECT_CALL(mMockLegs, Update())
                .Times(1)
                .WillOnce(Return(Core::Status::CORE_OK));
            gaitCycle.Update(1100U);
        }
    }
}
