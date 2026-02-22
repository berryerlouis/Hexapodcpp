#include "ClusterImu.h"

namespace Cluster
{
    namespace Imu
    {
        using namespace Component::Imu;
        using namespace Component::Barometer;

        ClusterImu::ClusterImu(Mpu9150Interface   &imu,
                               BarometerInterface &barometer)
            : ClusterBase(IMU, *this)
            , ClusterCommand(NB_COMMANDS_IMU)
            , mImu(imu)
            , mBarometer(barometer) {
            this->AddClusterItem(ClusterItem(EImuCommands::ALL, 0U));
            this->AddClusterItem(ClusterItem(EImuCommands::ACC, 0U));
            this->AddClusterItem(ClusterItem(EImuCommands::GYR, 0U));
            this->AddClusterItem(ClusterItem(EImuCommands::MAG, 0U));
            this->AddClusterItem(ClusterItem(EImuCommands::TMP, 0U));
            this->AddClusterItem(ClusterItem(EImuCommands::YAW_PITCH_ROLL, 0U));
            this->AddClusterItem(ClusterItem(EImuCommands::PRESSURE, 0U));
            this->AddClusterItem(ClusterItem(EImuCommands::ALTITUDE, 0U));
            this->AddClusterItem(ClusterItem(EImuCommands::TMP_BAR, 0U));
            this->AddClusterItem(ClusterItem(EImuCommands::CALIB_SENSOR, 2U));
            LOG_CLUSTER_DEBUG("Imu", "(%d) Initialized.", IMU);
        }

        auto ClusterImu::ExecuteFrame(const Frame &request,
                                      Frame       &response) -> Core::Status {
            Core::Status success = Core::Status::CORE_ERROR;
            if (request.GetCommandId() == EImuCommands::ALL) {
                const Vector3  acc = this->mImu.ReadAcc();
                const Vector3  gyr = this->mImu.ReadGyr();
                const Vector3  mag = this->mImu.ReadMag();
                const uint16_t temp = this->mImu.ReadTemp();
                success = BuildFrameAll(acc, gyr, mag, temp, response);
            } else if (request.GetCommandId() == EImuCommands::ACC) {
                const Vector3 acc = this->mImu.ReadAcc();
                success = BuildFrameAcc(acc, response);
            } else if (request.GetCommandId() == EImuCommands::GYR) {
                const Vector3 gyr = this->mImu.ReadGyr();
                success = BuildFrameGyr(gyr, response);
            } else if (request.GetCommandId() == EImuCommands::MAG) {
                const Vector3 mag = this->mImu.ReadMag();
                success = BuildFrameMag(mag, response);
            } else if (request.GetCommandId() == EImuCommands::TMP) {
                const uint16_t temp = this->mImu.ReadTemp();
                success = BuildFrameTmp(temp, response);
            } else if (request.GetCommandId() == EImuCommands::YAW_PITCH_ROLL) {
                const Imu3d ypr = this->mImu.ReadYawPitchRoll();
                success = BuildFrameYawPitchRoll(ypr, response);
            } else if (request.GetCommandId() == EImuCommands::PRESSURE) {
                const int32_t pressure = this->mBarometer.GetPressure();
                success = BuildFramePressure(pressure, response);
            } else if (request.GetCommandId() == EImuCommands::ALTITUDE) {
                const uint16_t seaLevel = this->mBarometer.GetAltitude();
                success = BuildFrameSeaLevel(seaLevel, response);
            } else if (request.GetCommandId() == EImuCommands::TMP_BAR) {
                const int16_t temp = this->mBarometer.GetTemp();
                success = BuildFrameTmpBar(temp, response);
            } else if (request.GetCommandId() == EImuCommands::CALIB_SENSOR) {
                const SensorsImu sensor =
                        static_cast<SensorsImu>(request.Get1ByteParam(0U));
                const bool enable =
                        static_cast<bool>(request.Get1ByteParam(1U));
                if (enable) {
                    this->mImu.StartCalibration(sensor);
                } else {
                    this->mImu.StopCalibration(sensor);
                }
                success = BuildFrameStartCalibMag(response);
            }
            return success;
        }

        Core::Status ClusterImu::BuildFrameAll(const Vector3 &acc,
                                               const Vector3 &gyr,
                                               const Vector3 &mag,
                                               const uint16_t temp,
                                               Frame         &response) {
            const Core::Status success =
                    response.Build(EClusters::IMU, EImuCommands::ALL);
            if (success == Core::Status::CORE_OK) {
                response.SetxBytesParam(
                        6U, reinterpret_cast<const uint8_t *>(&acc));
                response.SetxBytesParam(
                        6U, reinterpret_cast<const uint8_t *>(&gyr));
                response.SetxBytesParam(
                        6U, reinterpret_cast<const uint8_t *>(&mag));
                response.Set2BytesParam(temp);
            }
            return (success);
        }

        Core::Status ClusterImu::BuildFrameAcc(const Vector3 &acc,
                                               Frame         &response) {
            const Core::Status success =
                    response.Build(EClusters::IMU, EImuCommands::ACC);
            if (success == Core::Status::CORE_OK) {
                response.SetxBytesParam(
                        6U, reinterpret_cast<const uint8_t *>(&acc));
            }
            return (success);
        }

        Core::Status ClusterImu::BuildFrameGyr(const Vector3 &gyr,
                                               Frame         &response) {
            const Core::Status success =
                    response.Build(EClusters::IMU, EImuCommands::GYR);
            if (success == Core::Status::CORE_OK) {
                response.SetxBytesParam(
                        6U, reinterpret_cast<const uint8_t *>(&gyr));
            }
            return (success);
        }

        Core::Status ClusterImu::BuildFrameMag(const Vector3 &mag,
                                               Frame         &response) {
            const Core::Status success =
                    response.Build(EClusters::IMU, EImuCommands::MAG);
            if (success == Core::Status::CORE_OK) {
                response.SetxBytesParam(
                        6U, reinterpret_cast<const uint8_t *>(&mag));
            }
            return (success);
        }

        Core::Status ClusterImu::BuildFrameTmp(const uint16_t temp,
                                               Frame         &response) {
            const Core::Status success =
                    response.Build(EClusters::IMU, EImuCommands::TMP);
            if (success == Core::Status::CORE_OK) {
                response.Set2BytesParam(temp);
            }
            return (success);
        }

        Core::Status ClusterImu::BuildFrameYawPitchRoll(const Imu3d ypr,
                                                        Frame      &response) {
            Vector3 cmp = {};
            cmp.x = static_cast<int16_t>(ypr.roll * 100U);
            cmp.y = static_cast<int16_t>(ypr.pitch * 100U);
            cmp.z = static_cast<int16_t>(ypr.yaw * 100U);
            const Core::Status success = response.Build(
                    EClusters::IMU, EImuCommands::YAW_PITCH_ROLL);
            if (success == Core::Status::CORE_OK) {
                response.SetxBytesParam(
                        6U, reinterpret_cast<const uint8_t *>(&cmp));
            }
            return (success);
        }

        Core::Status ClusterImu::BuildFramePressure(const int32_t pressure,
                                                    Frame        &response) {
            const Core::Status success =
                    response.Build(EClusters::IMU, EImuCommands::PRESSURE);
            if (success == Core::Status::CORE_OK) {
                response.Set4BytesParam(pressure);
            }
            return (success);
        }

        Core::Status ClusterImu::BuildFrameSeaLevel(const uint16_t seaLevel,
                                                    Frame         &response) {
            const Core::Status success =
                    response.Build(EClusters::IMU, EImuCommands::ALTITUDE);
            if (success == Core::Status::CORE_OK) {
                response.Set2BytesParam(seaLevel);
            }
            return (success);
        }

        Core::Status ClusterImu::BuildFrameTmpBar(const int16_t temp,
                                                  Frame        &response) {
            const Core::Status success =
                    response.Build(EClusters::IMU, EImuCommands::TMP_BAR);
            if (success == Core::Status::CORE_OK) {
                response.Set2BytesParam(temp);
            }
            return (success);
        }

        Core::Status ClusterImu::BuildFrameStartCalibMag(Frame &response) {
            return response.Build(EClusters::IMU, EImuCommands::CALIB_SENSOR);
        }
    }; // namespace Imu
} // namespace Cluster
