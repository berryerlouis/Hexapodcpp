#pragma once
#include <vector>

#include "GaitParams.h"
#include "Constants.h"
#include "../../Bot/Constants.h"
#include "../../Misc/Maths/Geometry.h"

namespace Bot
{
    namespace Gait
    {
        struct Step {
            Misc::Maths::Position3d position;
            float duration;
        };

        struct Group {
            std::vector<ELeg> legs;
            uint8_t indexStep;
            uint64_t startTime;
            uint64_t endTime;
            float offsetTime;

            Group(const std::vector<ELeg> &legs, const float offsetTime):
                legs(legs)
                , indexStep(0)
                , startTime(offsetTime)
                , endTime(0)
                , offsetTime(offsetTime) {
            }
        };

        using Groups = std::vector<Group *>;

        struct Steps {
            std::vector<Step> steps;

            Steps() {
                this->steps.push_back({0.0F, -1.0F, 0.0F, 1 / 3.0F});
                this->steps.push_back({0.0F, 0.75F, 1.0F, 1 / 3.0F});
                this->steps.push_back({0.0F, 1.0F, 0.0F, 1 / 3.0F});
            }

            void
            SetStepDuration(const uint8_t stepIndex, const uint16_t duration) {
                this->steps[stepIndex].duration = duration;
                LOG_BOT_DEBUG("Gaits", "step duration: %f", duration);
            }

            void
            SetCycleDuration(const uint16_t duration) const {
                for (Step step: this->steps) {
                    step.duration = (static_cast<float>(duration) / static_cast<float>(this->steps.size()) / static_cast
                                     <float>(duration));
                    LOG_BOT_DEBUG("Gaits", "cycle duration: %f", step.duration);
                }
            }
        };

        class GaitBase {
        public:
            GaitBase(GaitType type, const Groups &groups, GaitParams &params);

            ~GaitBase() = default;

            void Update(uint64_t currentTime, uint8_t groupId) const;

            float GetDeltaTime(uint64_t currentTime, uint8_t groupId) const;

            Misc::Maths::Position3d &GetPosition(uint8_t posId);

            Groups &GetGroups(void);

            Group &GetGroupId(uint8_t groupId) const;

            std::vector<ELeg> &GetGroupLeg(uint8_t groupId) const;

            uint8_t GetNbGroups(void) const;

            Steps &GetSteps(void);

            uint8_t &GetCurrentIndex(uint8_t groupId) const;

            void ResetIndex(uint8_t groupId) const;

            Step &GetCurrentStep(uint8_t groupId);

            uint16_t GetCurrentStepDuration(uint8_t groupId) const;

            uint16_t GetCurrentOffsetDuration(uint8_t groupId) const;

            uint8_t GetNbSteps(void) const;

            GaitType GetType(void) const;

            uint16_t GetCycleDuration() const;

            void NextStep(uint8_t groupId) const;

            void SetInitialStartTime(uint8_t groupId, uint64_t startTime) const;

            void SetStartTime(uint8_t groupId, uint64_t startTime) const;

            void ResetCycleStep(uint8_t groupId, uint64_t startTime) const;

            bool CanUpdate(uint64_t currentTime, uint8_t groupId) const;

        private:
            GaitType mType;
            Steps mSteps;
            Groups mGroups;
            GaitParams &mParams;
        };
    }
}
