#include "ClusterImu.h"

namespace Cluster
{
    namespace Imu
    {
        using namespace Component::Imu;
        using namespace Component::Barometer;

        ClusterImu::ClusterImu(Mpu9150Interface &imu, BarometerInterface &barometer) :
            ClusterBase(IMU, this)
            , ClusterCommand(NB_COMMANDS_IMU)
            , mImu(imu)
            , mBarometer(barometer) {
            this->AddClusterItem((ClusterItem){.commandId = EImuCommands::ALL, .expectedSize = 0U});
            this->AddClusterItem((ClusterItem){.commandId = EImuCommands::ACC, .expectedSize = 0U});
            this->AddClusterItem((ClusterItem){.commandId = EImuCommands::GYR, .expectedSize = 0U});
            this->AddClusterItem((ClusterItem){.commandId = EImuCommands::MAG, .expectedSize = 0U});
            this->AddClusterItem((ClusterItem){.commandId = EImuCommands::TMP, .expectedSize = 0U});
            this->AddClusterItem((ClusterItem){.commandId = EImuCommands::YAW_PITCH_ROLL, .expectedSize = 0U});
            this->AddClusterItem((ClusterItem){.commandId = EImuCommands::PRESSURE, .expectedSize = 0U});
            this->AddClusterItem((ClusterItem){.commandId = EImuCommands::ALTITUDE, .expectedSize = 0U});
            this->AddClusterItem((ClusterItem){.commandId = EImuCommands::TMP_BAR, .expectedSize = 0U});
            this->AddClusterItem((ClusterItem){.commandId = EImuCommands::CALIB_SENSOR, .expectedSize = 2U});
        }

        Core::Status ClusterImu::ExecuteFrame(const Frame &request, Frame &response) {
            Core::Status success = Core::Status::CORE_ERROR;
            if (request.commandId == EImuCommands::ALL) {
                const Vector3 acc = this->mImu.ReadAcc();
                const Vector3 gyr = this->mImu.ReadGyr();
                const Vector3 mag = this->mImu.ReadMag();
                const uint16_t temp = this->mImu.ReadTemp();
                success = this->BuildFrameAll(acc, gyr, mag, temp, response);
            } else if (request.commandId == EImuCommands::ACC) {
                const Vector3 acc = this->mImu.ReadAcc();
                success = this->BuildFrameAcc(acc, response);
            } else if (request.commandId == EImuCommands::GYR) {
                const Vector3 gyr = this->mImu.ReadGyr();
                success = this->BuildFrameGyr(gyr, response);
            } else if (request.commandId == EImuCommands::MAG) {
                const Vector3 mag = this->mImu.ReadMag();
                success = this->BuildFrameMag(mag, response);
            } else if (request.commandId == EImuCommands::TMP) {
                const uint16_t temp = this->mImu.ReadTemp();
                success = this->BuildFrameTmp(temp, response);
            } else if (request.commandId == EImuCommands::YAW_PITCH_ROLL) {
                const Position3D ypr = this->mImu.ReadYawPitchRoll();
                success = this->BuildFrameYawPitchRoll(ypr, response);
            } else if (request.commandId == EImuCommands::PRESSURE) {
                const int32_t pressure = this->mBarometer.GetPressure();
                success = this->BuildFramePressure(pressure, response);
            } else if (request.commandId == EImuCommands::ALTITUDE) {
                const uint16_t seaLevel = this->mBarometer.GetAltitude();
                success = this->BuildFrameSeaLevel(seaLevel, response);
            } else if (request.commandId == EImuCommands::TMP_BAR) {
                const int16_t temp = this->mBarometer.GetTemp();
                success = this->BuildFrameTmpBar(temp, response);
            } else if (request.commandId == EImuCommands::CALIB_SENSOR) {
                const SensorsImu sensor = static_cast<SensorsImu>(request.Get1ByteParam(0U));
                const bool enable = request.Get1ByteParam(1U);
                this->mImu.StartCalibration(sensor, enable);
                success = this->BuildFrameStartCalibMag(response);
            }
            return success;
        }

        Core::Status ClusterImu::BuildFrameAll(const Vector3 &acc, const Vector3 &gyr, const Vector3 &mag,
                                               const uint16_t temp, Frame &response) {
            const Core::Status success = response.Build(
                    EClusters::IMU,
                    EImuCommands::ALL);
            if (success == Core::Status::CORE_OK) {
                response.SetxBytesParam(6U, (uint8_t *) &acc);
                response.SetxBytesParam(6U, (uint8_t *) &gyr);
                response.SetxBytesParam(6U, (uint8_t *) &mag);
                response.Set2BytesParam(temp);
            }
            return (success);
        }

        Core::Status ClusterImu::BuildFrameAcc(const Vector3 &acc, Frame &response) {
            const Core::Status success = response.Build(
                    EClusters::IMU,
                    EImuCommands::ACC);
            if (success == Core::Status::CORE_OK) {
                response.SetxBytesParam(6U, (uint8_t *) &acc);
            }
            return (success);
        }

        Core::Status ClusterImu::BuildFrameGyr(const Vector3 &gyr, Frame &response) {
            const Core::Status success = response.Build(
                    EClusters::IMU,
                    EImuCommands::GYR);
            if (success == Core::Status::CORE_OK) {
                response.SetxBytesParam(6U, (uint8_t *) &gyr);
            }
            return (success);
        }

        Core::Status ClusterImu::BuildFrameMag(const Vector3 &mag, Frame &response) {
            const Core::Status success = response.Build(
                    EClusters::IMU,
                    EImuCommands::MAG);
            if (success == Core::Status::CORE_OK) {
                response.SetxBytesParam(6U, (uint8_t *) &mag);
            }
            return (success);
        }

        Core::Status ClusterImu::BuildFrameTmp(const uint16_t temp, Frame &response) {
            const Core::Status success = response.Build(
                    EClusters::IMU,
                    EImuCommands::TMP);
            if (success == Core::Status::CORE_OK) {
                response.Set2BytesParam(temp);
            }
            return (success);
        }

        Core::Status ClusterImu::BuildFrameYawPitchRoll(const Position3D ypr, Frame &response) {
            Vector3 cmp;
            cmp.x = static_cast<int16_t>(ypr.roll * 10U);
            cmp.y = static_cast<int16_t>(ypr.pitch * 10U);
            cmp.z = static_cast<int16_t>(ypr.yaw * 10U);
            const Core::Status success = response.Build(
                    EClusters::IMU,
                    EImuCommands::YAW_PITCH_ROLL);
            if (success == Core::Status::CORE_OK) {
                response.SetxBytesParam(6U, (uint8_t *) &cmp);
            }
            return (success);
        }

        Core::Status ClusterImu::BuildFramePressure(const int32_t pressure, Frame &response) {
            const Core::Status success = response.Build(
                    EClusters::IMU,
                    EImuCommands::PRESSURE);
            if (success == Core::Status::CORE_OK) {
                response.Set4BytesParam(pressure);
            }
            return (success);
        }

        Core::Status ClusterImu::BuildFrameSeaLevel(const uint16_t seaLevel, Frame &response) {
            const Core::Status success = response.Build(
                    EClusters::IMU,
                    EImuCommands::ALTITUDE);
            if (success == Core::Status::CORE_OK) {
                response.Set2BytesParam(seaLevel);
            }
            return (success);
        }

        Core::Status ClusterImu::BuildFrameTmpBar(const int16_t temp, Frame &response) {
            const Core::Status success = response.Build(
                    EClusters::IMU,
                    EImuCommands::TMP_BAR);
            if (success == Core::Status::CORE_OK) {
                response.Set2BytesParam(temp);
            }
            return (success);
        }

        Core::Status ClusterImu::BuildFrameCalibMag(const bool min, const Vector3F &calib, Frame &response) {
            const Core::Status success = response.Build(
                    EClusters::IMU,
                    EImuCommands::YAW_PITCH_ROLL);
            if (success == Core::Status::CORE_OK) {
                response.Set1ByteParam(min);
                response.SetxBytesParam(12U, (uint8_t *) &calib);
            }
            return (success);
        }

        Core::Status ClusterImu::BuildFrameStartCalibMag(Frame &response) {
            return response.Build(
                    EClusters::IMU,
                    EImuCommands::CALIB_SENSOR);
        }
    };
}

