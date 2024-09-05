#pragma once


#include "../Cluster.h"
#include "../../Component/Imu/Mpu9150Interface.h"
#include "../../Misc/Geometry/Geometry.h"

namespace Cluster
{
    namespace Imu
    {
        using namespace Component::Imu;

        class ClusterImu : public Cluster {
        public:
            ClusterImu(Mpu9150Interface &imu)
                : Cluster(IMU)
                  , mImu(imu) {
            }

            ~ClusterImu() = default;

            inline virtual Core::CoreStatus Execute(Frame &request, Frame &response) final override {
                Core::CoreStatus success = Core::CoreStatus::CORE_ERROR;

                if (request.clusterId != this->GetId()) {
                    return (success);
                }

                switch ((EImuCommands) request.commandId) {
                    case EImuCommands::ALL:
                        success = this->BuildFrameAll(response);
                        break;

                    case EImuCommands::ACC:
                        success = this->BuildFrameAcc(response);
                        break;

                    case EImuCommands::GYR:
                        success = this->BuildFrameGyr(response);
                        break;

                    case EImuCommands::MAG:
                        success = this->BuildFrameMag(response);
                        break;

                    case EImuCommands::TMP:
                        success = this->BuildFrameTmp(response);
                        break;

                    default:
                        break;
                }
                return (success);
            }

            inline Core::CoreStatus BuildFrameAll(Frame &response) {
                Core::CoreStatus success = response.Build(
                    EClusters::IMU,
                    EImuCommands::ALL);
                if (success) {
                    Vector3 params = this->mImu.ReadAcc();
                    response.SetnBytesParam(6U, (uint8_t *) &params);
                    params = this->mImu.ReadGyr();
                    response.SetnBytesParam(6U, (uint8_t *) &params);
                    params = this->mImu.ReadMag();
                    response.SetnBytesParam(6U, (uint8_t *) &params);
                    response.Set2BytesParam(this->mImu.ReadTemp());
                }
                return (success);
            }

            inline Core::CoreStatus BuildFrameAcc(Frame &response) {
                Core::CoreStatus success = response.Build(
                    EClusters::IMU,
                    EImuCommands::ACC);
                if (success) {
                    Vector3 params = this->mImu.ReadAcc();
                    response.SetnBytesParam(6U, (uint8_t *) &params);
                }
                return (success);
            }

            inline Core::CoreStatus BuildFrameGyr(Frame &response) {
                Core::CoreStatus success = response.Build(
                    EClusters::IMU,
                    EImuCommands::GYR);
                if (success) {
                    Vector3 params = this->mImu.ReadGyr();
                    response.SetnBytesParam(6U, (uint8_t *) &params);
                }
                return (success);
            }

            inline Core::CoreStatus BuildFrameMag(Frame &response) {
                Core::CoreStatus success = response.Build(
                    EClusters::IMU,
                    EImuCommands::MAG);
                if (success) {
                    Vector3 params = this->mImu.ReadMag();
                    response.SetnBytesParam(6U, (uint8_t *) &params);
                }
                return (success);
            }

            inline Core::CoreStatus BuildFrameTmp(Frame &response) {
                Core::CoreStatus success = response.Build(
                    EClusters::IMU,
                    EImuCommands::TMP);
                if (success) {
                    response.Set2BytesParam(this->mImu.ReadTemp());
                }
                return (success);
            }

        private:
            Mpu9150Interface &mImu;
        };
    }
}
