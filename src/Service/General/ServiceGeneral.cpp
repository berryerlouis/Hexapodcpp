#include "ServiceGeneral.h"

namespace Service
{
    namespace General
    {
        ServiceGeneral::ServiceGeneral(SoftwareInterface &software)
            : Service(1U)
              , mSoftware(software) {
        }

        Core::CoreStatus ServiceGeneral::Initialize(void) {
            return (this->mSoftware.Initialize());
        }

        void ServiceGeneral::Update(const uint64_t currentTime) {
            (void) currentTime;
            const uint64_t deltaTime = this->GetDeltaTime();

            if (deltaTime < this->mSoftware.GetMinTime()) {
                this->mSoftware.SetMinTime(deltaTime);
                this->mComMediator->SendMessage({
                    .id = Cluster::EClusters::GENERAL,
                    .value = static_cast<uint8_t>(Cluster::EGeneralCommands::MIN_EXECUTION_TIME)
                });
            } else if (deltaTime > this->mSoftware.GetMaxTime()) {
                this->mSoftware.SetMaxTime(deltaTime);
                this->mComMediator->SendMessage({
                    .id = Cluster::EClusters::GENERAL,
                    .value = static_cast<uint8_t>(Cluster::EGeneralCommands::MAX_EXECUTION_TIME)
                });
            }
        }
    }
}
