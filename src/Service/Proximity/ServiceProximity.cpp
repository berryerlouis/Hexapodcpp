#include "ServiceProximity.h"

namespace Service
{
    namespace Proximity
    {
        ServiceProximity::ServiceProximity(SensorProximityMultipleInterface &proximity)
            : Service(25)
              , mProximity(proximity) {
        }

        Core::CoreStatus ServiceProximity::Initialize(void) {
            Core::CoreStatus success = Core::CoreStatus::CORE_ERROR;
            if (this->mProximity.Initialize()) {
                this->mProximity.Attach(this);
                success = Core::CoreStatus::CORE_OK;
            }
            return (success);
        }

        void ServiceProximity::Update(const uint64_t currentTime) {
            this->mProximity.Update(currentTime);
        }

        void ServiceProximity::Detect(const SensorsId &sensorId) {
            this->mComMediator->SendMessage({id: Cluster::EClusters::PROXIMITY, value: (uint8_t) sensorId});
        }

        void ServiceProximity::NoDetect(const SensorsId &sensorId) {
            this->mComMediator->SendMessage({id: Cluster::EClusters::PROXIMITY, value: (uint8_t) sensorId});
        }
    }
}
