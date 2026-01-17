#pragma once

#include "../../Component/Barometer/BarometerInterface.h"
#include "../../Component/Imu/Mpu9150Interface.h"
#include "../../Misc/Maths/Geometry.h"
#include "../ClusterBase.h"

namespace Cluster
{
    namespace Imu
    {
        using namespace Component::Imu;
        using namespace Component::Barometer;

        class ClusterImu : public ClusterBase, ClusterCommand {
        public:
            ClusterImu(Mpu9150Interface &imu, BarometerInterface &barometer);

            ~ClusterImu() = default;

            virtual Core::Status ExecuteFrame(const Frame &request,
                                              Frame       &response) override;

            static Core::Status  BuildFrameAll(const Vector3 &acc,
                                               const Vector3 &gyr,
                                               const Vector3 &mag,
                                               const uint16_t temp,
                                               Frame         &response);

            static Core::Status  BuildFrameAcc(const Vector3 &acc,
                                               Frame         &response);

            static Core::Status  BuildFrameGyr(const Vector3 &gyr,
                                               Frame         &response);

            static Core::Status  BuildFrameMag(const Vector3 &mag,
                                               Frame         &response);

            static Core::Status  BuildFrameTmp(const uint16_t temp,
                                               Frame         &response);

            static Core::Status  BuildFrameYawPitchRoll(const Imu3d ypr,
                                                        Frame      &response);

            static Core::Status  BuildFramePressure(const int32_t pressure,
                                                    Frame        &response);

            static Core::Status  BuildFrameSeaLevel(const uint16_t seaLevel,
                                                    Frame         &response);

            static Core::Status  BuildFrameTmpBar(const int16_t temp,
                                                  Frame        &response);

            static Core::Status  BuildFrameStartCalibMag(Frame &response);

        private:
            Mpu9150Interface   &mImu;
            BarometerInterface &mBarometer;
        };
    } // namespace Imu
} // namespace Cluster
