#include <gtest/gtest.h>
#include "../../../../src/Move/Gait/GaitParams.h"

using namespace Move::Gait;

class UT_MOVE_GAIT_PARAMS : public ::testing::Test {
protected:
    void
    SetUp() override {
        gaitParams = new GaitParams();
    }

    void
    TearDown() override {
        delete gaitParams;
    }

    GaitParams *gaitParams;
};

// Test default constructor
TEST_F(UT_MOVE_GAIT_PARAMS, TestDefaultValues) {
    EXPECT_EQ(gaitParams->GetGaitType(), GaitType::TRIPOD);
    EXPECT_FALSE(gaitParams->IsRunning());
    EXPECT_FLOAT_EQ(gaitParams->GetDirection(), 0.0F);
    EXPECT_FLOAT_EQ(gaitParams->GetRotation(), 0.0F);
    EXPECT_FALSE(gaitParams->IsRotated());
    EXPECT_FLOAT_EQ(gaitParams->GetAmplitude(), 3.0F);
    EXPECT_FLOAT_EQ(gaitParams->GetElevation(), 3.0F); // Default is ELEVATION_MAX = 3.0F
    EXPECT_EQ(gaitParams->GetCycleDuration(), 1000U);
}

// Test SetGaitType
TEST_F(UT_MOVE_GAIT_PARAMS, TestSetGaitType) {
    EXPECT_TRUE(gaitParams->SetGaitType(GaitType::WAVE));
    EXPECT_EQ(gaitParams->GetGaitType(), GaitType::WAVE);

    EXPECT_TRUE(gaitParams->SetGaitType(GaitType::RIPPLE));
    EXPECT_EQ(gaitParams->GetGaitType(), GaitType::RIPPLE);
}

// Test SetDirection
TEST_F(UT_MOVE_GAIT_PARAMS, TestSetDirection) {
    // Valid: angle <= 2*PI (6.28...)
    EXPECT_TRUE(gaitParams->SetDirection(3.14F)); // PI radians
    EXPECT_FLOAT_EQ(gaitParams->GetDirection(), 3.14F);

    EXPECT_TRUE(gaitParams->SetDirection(1.57F)); // PI/2 radians
    EXPECT_FLOAT_EQ(gaitParams->GetDirection(), 1.57F);

    // Invalid: angle > 2*PI
    EXPECT_FALSE(gaitParams->SetDirection(10.0F));
}

// Test SetRotation
TEST_F(UT_MOVE_GAIT_PARAMS, TestSetRotation) {
    // Valid: 0 < angle <= 2*PI
    EXPECT_TRUE(gaitParams->SetRotation(1.57F, true)); // PI/2 radians
    EXPECT_FLOAT_EQ(gaitParams->GetRotation(), 1.57F);
    EXPECT_TRUE(gaitParams->IsRotated());
    EXPECT_TRUE(gaitParams->GetRotationClockWize());

    EXPECT_TRUE(gaitParams->SetRotation(0.78F, false)); // PI/4 radians
    EXPECT_FLOAT_EQ(gaitParams->GetRotation(), 0.78F);
    EXPECT_TRUE(gaitParams->IsRotated());
    EXPECT_FALSE(gaitParams->GetRotationClockWize());

    // Setting rotation to 0 should succeed and disable rotation
    EXPECT_TRUE(gaitParams->SetRotation(0.0F, true));
    EXPECT_FLOAT_EQ(gaitParams->GetRotation(), 0.0F);
    EXPECT_FALSE(gaitParams->IsRotated());

    // Invalid: angle > 2*PI
    EXPECT_FALSE(gaitParams->SetRotation(10.0F, true));
}

// Test SetAmplitude
TEST_F(UT_MOVE_GAIT_PARAMS, TestSetAmplitude) {
    // Valid: amplitude <= AMPLITUDE_MAX (3.0F)
    EXPECT_TRUE(gaitParams->SetAmplitude(2.5F));
    EXPECT_FLOAT_EQ(gaitParams->GetAmplitude(), 2.5F);

    EXPECT_TRUE(gaitParams->SetAmplitude(1.0F));
    EXPECT_FLOAT_EQ(gaitParams->GetAmplitude(), 1.0F);

    // Invalid: amplitude > AMPLITUDE_MAX
    EXPECT_FALSE(gaitParams->SetAmplitude(5.0F));
}

// Test SetElevation
TEST_F(UT_MOVE_GAIT_PARAMS, TestSetElevation) {
    EXPECT_TRUE(gaitParams->SetElevation(2.0F));
    EXPECT_FLOAT_EQ(gaitParams->GetElevation(), 2.0F);

    EXPECT_TRUE(gaitParams->SetElevation(1.0F));
    EXPECT_FLOAT_EQ(gaitParams->GetElevation(), 1.0F);
}

// Test SetCycleDuration
TEST_F(UT_MOVE_GAIT_PARAMS, TestSetCycleDuration) {
    // Valid: duration >= DURATION_MIN (1000)
    EXPECT_TRUE(gaitParams->SetCycleDuration(2000U));
    EXPECT_EQ(gaitParams->GetCycleDuration(), 2000U);

    EXPECT_TRUE(gaitParams->SetCycleDuration(1000U));
    EXPECT_EQ(gaitParams->GetCycleDuration(), 1000U);

    // Invalid: duration < DURATION_MIN
    EXPECT_FALSE(gaitParams->SetCycleDuration(500U));
}

// Test Running state
TEST_F(UT_MOVE_GAIT_PARAMS, TestRunningState) {
    EXPECT_FALSE(gaitParams->IsRunning());

    EXPECT_TRUE(gaitParams->SetRunning(true));
    EXPECT_TRUE(gaitParams->IsRunning());

    EXPECT_TRUE(gaitParams->SetRunning(false));
    EXPECT_FALSE(gaitParams->IsRunning());
}