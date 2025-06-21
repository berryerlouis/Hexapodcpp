#include "GaitBase.h"

namespace Bot
{
    namespace Gait
    {

        GaitBase::GaitBase(const GaitType type, const Groups &groups, const GaitParams &params) :
            mType(type)
            , mSteps()
            , mGroups{groups}
            , mParams(params) {
        }

        Groups &GaitBase::GetGroups() {
            return this->mGroups;
        }

        Group &GaitBase::GetGroupId(const uint8_t groupId) const {
            return *this->mGroups[groupId];
        }

        std::vector<ELeg> &GaitBase::GetGroupLeg(const uint8_t groupId) const {
            return this->mGroups[groupId]->legs;
        }

        uint8_t GaitBase::GetNbGroups(void) const {
            return this->mGroups.size();
        }

        Steps &GaitBase::GetSteps() {
            return this->mSteps;
        }

        uint8_t &GaitBase::GetCurrentIndex(const uint8_t groupId) const {
            return this->GetGroupId(groupId).indexStep;
        }

        void GaitBase::ResetIndex(const uint8_t groupId) const {
            this->GetGroupId(groupId).indexStep = 0U;
        }

        Step &GaitBase::GetCurrentStep(const uint8_t groupId) {
            return this->mSteps.steps[this->GetCurrentIndex(groupId)];
        }

        uint16_t GaitBase::GetCurrentStepDuration(const uint8_t groupId) const {
            const float cycleDuration = this->GetCycleDuration();
            return cycleDuration * this->mSteps.steps[this->GetCurrentIndex(groupId)].duration;
        }

        uint16_t GaitBase::GetCurrentOffsetDuration(const uint8_t groupId) const {
            const float cycleDuration = this->GetCycleDuration();
            return cycleDuration * this->GetGroupId(groupId).offsetTime;
        }

        uint8_t GaitBase::GetNbSteps() const {
            return this->mSteps.steps.size();
        }

        GaitType GaitBase::GetType() const {
            return this->mType;
        }

        uint16_t GaitBase::GetCycleDuration(void) const {
            return this->mParams.GetCycleDuration();
        }

        void GaitBase::NextStep(const uint8_t groupId) const {
            this->GetCurrentIndex(groupId)++;
            if (this->GetCurrentIndex(groupId) == this->GetNbSteps()) {
                this->GetCurrentIndex(groupId) = 0U;
            }
        }

        void GaitBase::SetInitialStartTime(const uint8_t groupId, const uint64_t startTime) const {
            const uint16_t stepDuration = this->GetCurrentStepDuration(groupId);
            const uint16_t offsetTime = this->GetCurrentOffsetDuration(groupId);
            this->GetGroupId(groupId).startTime = startTime + offsetTime;
            this->GetGroupId(groupId).endTime = this->GetGroupId(groupId).startTime + stepDuration;
        }

        void GaitBase::SetStartTime(const uint8_t groupId, const uint64_t startTime) const {
            const uint16_t stepDuration = this->GetCurrentStepDuration(groupId);
            this->GetGroupId(groupId).startTime = startTime;
            this->GetGroupId(groupId).endTime = this->GetGroupId(groupId).startTime + stepDuration;
        }

        bool GaitBase::CanUpdate(const uint64_t currentTime, const uint8_t groupId) const {
            return (currentTime >= this->GetGroupId(groupId).startTime);
        }

        void GaitBase::Update(const uint64_t currentTime, const uint8_t groupId) const {
            if (currentTime >= this->GetGroupId(groupId).endTime) {
                this->NextStep(groupId);
                this->SetStartTime(groupId, currentTime);
            }
        }

        float GaitBase::GetDeltaTime(const uint64_t currentTime, const uint8_t groupId) const {
            const float stepDuration = this->GetCurrentStepDuration(groupId);
            const float startTime = this->GetGroupId(groupId).startTime;
            return (stepDuration - (stepDuration - (currentTime - startTime))) / stepDuration;
        }

        Misc::Maths::Position3d &GaitBase::GetPosition(const uint8_t posId) {
            return this->mSteps.steps[posId].position;
        }
    }
}
