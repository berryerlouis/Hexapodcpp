#include "ServiceProximity.h"

#include "../../Cluster/Proximity/ClusterProximity.h"

namespace Service
{
    namespace Proximity
    {
        ServiceProximity::ServiceProximity(SensorProximityMultipleInterface &proximity,
                                           Event::EventListenerInterface &eventListener) :
            Service(25U, eventListener), mProximity(proximity), mTimeoutDetection{0xFFU, 0xFFU, 0xFFU} {
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

            for (int i = 0; i < NB_SENSORS; ++i) {
                if (this->mTimeoutDetection[i] < MAX_TIMEOUT_DETECTION) {
                    this->mTimeoutDetection[i]++;
                } else if (
                    this->mTimeoutDetection[i] == MAX_TIMEOUT_DETECTION) {
                    this->mTimeoutDetection[i] = 0xFFU;
                    Frame response;
                    const uint16_t distance = this->mProximity.GetDistance(static_cast<SensorsId>(i));
                    Cluster::Proximity::ClusterProximity::BuildFrameDistance(
                            static_cast<SensorsId>(i), distance, response);
                    this->SendMessage(response);
                }
            }
        }

        void ServiceProximity::Detect(const SensorsId &sensorId, const uint16_t distance) {
            Frame response;
            Cluster::Proximity::ClusterProximity::BuildFrameDistance(sensorId, distance, response);
            this->SendMessage(response);
            this->mTimeoutDetection[sensorId] = 0U;
        }
    } // namespace Proximity
} // namespace Service
