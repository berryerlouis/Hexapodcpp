#include "Barometer.h"
namespace Component
{
    namespace Barometer
    {
        Barometer::Barometer(Driver::Twi::TwiInterface &i2c,
                             const uint8_t              address,
                             const BarometerResolution  resolution)
            : mI2c(i2c)
            , mAddress(address)
            , mResolution(resolution)
            , mProm{0U}
            , mRawTemp(0U)
            , mRawPressure(0U)
            , mTemp(0U)
            , mPressure(0U) {
        }

        Core::Status Barometer::Initialize() {
            this->ReadProm();
            LOG_COMPONENT_DEBUG("Barometer", "address 0x%02X Initialized.", this->mAddress);
            return Core::CORE_OK;
        }

        void Barometer::Update(const uint64_t currentTime) {
            (void) currentTime;
            static uint64_t lastTime = 0UL;

            if (currentTime > lastTime + 10000U) {
                lastTime = currentTime;
                static bool toggleReadTempPressure = false;
                if (!toggleReadTempPressure) {
                    toggleReadTempPressure = true;
                    this->mRawTemp = this->ReadRawTemp();
                    this->mI2c.SendCommand(this->mAddress, MS5611_CMD_CONV_D1 + this->mResolution);
                } else {
                    toggleReadTempPressure = false;
                    this->mRawPressure = this->ReadRawPressure();
                    this->mI2c.SendCommand(this->mAddress, MS5611_CMD_CONV_D2 + this->mResolution);
                }
                const int32_t deltaTime =
                        this->mRawTemp - (static_cast<uint32_t>(this->mProm[4U]) * 256);
                const int32_t temp =
                        2000 + (deltaTime * static_cast<int64_t>(this->mProm[5U])) / 8388608.0F;
                this->mTemp = temp;

                int64_t offset = (static_cast<int64_t>(this->mProm[1U]) * 65536) +
                                 (static_cast<int64_t>(this->mProm[3U]) * deltaTime) / 128.0F;
                int64_t sensibility = (static_cast<int64_t>(this->mProm[0U]) * 32768) +
                                      (static_cast<int64_t>(this->mProm[2U]) * deltaTime) / 256.0F;
                int64_t offset2 = 0;
                int64_t sensibility2 = 0;
                if (temp < 2000) {
                    offset2 = 5 * ((temp - 2000) * (temp - 2000)) / 2.0F;
                    sensibility2 = 5 * ((temp - 2000) * (temp - 2000)) / 4.0F;
                }

                if (temp < -1500) {
                    offset2 = offset2 + (7 * ((temp + 1500) * (temp + 1500)));
                    sensibility2 = sensibility2 + (11 * ((temp + 1500) * (temp + 1500)) / 2.0F);
                }

                offset -= offset2;
                sensibility -= sensibility2;
                this->mPressure =
                        (this->mRawPressure * sensibility / 2097152.0F - offset) / 32768.0F;
            }
        }

        void Barometer::Reset() const {
            this->mI2c.WriteRegister(this->mAddress, MS5611_CMD_RESET, 0U);
        }

        void Barometer::ReadProm() {
            for (size_t offset = 0U; offset < 6U; offset++) {
                uint8_t data[2U] = {0U};
                this->mI2c.ReadRegisters(
                        this->mAddress, MS5611_CMD_READ_PROM + (offset * 2U), data, 2U);
                this->mProm[offset] = ((data[0U] << 8U) | data[1U]);
            }
        }

        uint32_t Barometer::ReadRawTemp() const {
            uint8_t data[3U] = {0U};
            this->mI2c.ReadRegisters(this->mAddress, MS5611_CMD_ADC_READ, data, 3U);
            return ((data[0U] << 16U) | ((data[1U] << 8U)) | ((data[2U])));
        }

        uint32_t Barometer::ReadRawPressure() const {
            uint8_t data[3U] = {0U};
            this->mI2c.ReadRegisters(this->mAddress, MS5611_CMD_ADC_READ, data, 3U);
            return ((data[0U] << 16U) | ((data[1U] << 8U)) | ((data[2U])));
        }
    } // namespace Barometer
} // namespace Component
