#include "ServiceProximity.h"

#include "../../Cluster/Proximity/ClusterProximity.h"

namespace Service
{
    namespace Proximity
    {
        ServiceProximity::ServiceProximity(
                SensorProximityMultipleInterface &proximity,
                Message::MessageInterface        &messageListener,
                Event::EventDispatcherInterface  &eventDispatcher)
            : Service(PROXIMITY, 100U, messageListener, eventDispatcher)
            , mProximity(proximity) {
        }

        Core::Status ServiceProximity::Initialize(void) {
            const Core::Status success = this->mProximity.Initialize();
            if (Core::Status::CORE_OK == success) {
                this->GetEventDispatcher().AddListener(this);
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
            Cluster::Proximity::ClusterProximity::BuildFrameDistance(
                    sensor.id, sensor.distance, response);
            this->SendMessage(response);
            this->DispatchEvent<SensorsStruct>(EventType::EVENT_SENSOR_UPDATE,
                                               sensor);
        }

        void ServiceProximity::OnEvent(const Event::Event &event) {
            (void) event;
        }
    } // namespace Proximity
} // namespace Service
