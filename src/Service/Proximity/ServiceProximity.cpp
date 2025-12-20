#include "ServiceProximity.h"

#include "../../Cluster/Proximity/ClusterProximity.h"

namespace Service
{
    namespace Proximity
    {
        ServiceProximity::ServiceProximity(
                SensorProximityMultipleInterface &proximity,
                Message::MessageInterface &messageListener,
                Event::EventListenerInterface &eventListener
        ) :
            Service(PROXIMITY, 100U, messageListener, eventListener), mProximity(proximity) {
        }

        Core::Status ServiceProximity::Initialize(void) {
            const Core::Status success = this->mProximity.Initialize();
            if (Core::Status::CORE_OK == success) {
                this->mProximity.Attach(this);
                this->mInitialized = true;
            }
            return success;
        }

        void ServiceProximity::Update(const uint64_t currentTime) {
            this->mProximity.Update(currentTime);
        }

        void ServiceProximity::Notified(const SensorsStruct &sensor) {
            Frame response;
            Cluster::Proximity::ClusterProximity::BuildFrameDistance(sensor.id, sensor.distance, response);
            this->SendMessage(response);
        }

        void ServiceProximity::DispatchEvent(const Event::EventType event) const {
            (void) event;
        }
    } // namespace Proximity
} // namespace Service
