#include "Vl53l0x.h"
#ifdef RPI
#include "wiringPi/wiringPiI2C.h"
#endif
namespace Component
{
    namespace Proximity
    {
        namespace Laser
        {
#define RANGE_SEQUENCE_STEP_TCC (0x10) /* Target CentreCheck */
#define RANGE_SEQUENCE_STEP_MSRC (0x04) /* Minimum Signal Rate Check */
#define RANGE_SEQUENCE_STEP_DSS (0x28) /* Dynamic SPAD selection */
#define RANGE_SEQUENCE_STEP_PRE_RANGE (0x40)
#define RANGE_SEQUENCE_STEP_FINAL_RANGE (0x80)

            Vl53l0x::Vl53l0x(Twi::TwiInterface &i2c,
                             Led::LedInterface &led,
                             const uint8_t      address) :
                mI2c(i2c),
                mLed(led),
                mAddress(address),
                mDistance(0),
                mThreshold(DISTANCE_THRESHOLD),
                mMeasurementTimingBudget(0U),
                mStop(0U) {
#ifdef RPI
                this->mAddress = wiringPiI2CSetup(address);
#endif
            }

            Core::Status Vl53l0x::Initialize(void) {
                Core::Status success = Core::Status::CORE_ERROR;
                uint8_t      data = 0U;
                uint8_t      timeout = 0U;
                this->mLed.Initialize();

                // reset
                this->mI2c.WriteRegister(this->mAddress, 0xBF, 0x00);
                do {
                    this->mI2c.ReadRegister(this->mAddress, VL53L0X_IDENTIFICATION_MODEL_ID, data);
                } while (data != 0x00U);
                // release reset
                this->mI2c.WriteRegister(this->mAddress, 0xBF, 0x01);
                Timer::Tick::GetInstance().DelayMs(1U);
                do {
                    timeout++;
                    this->mI2c.ReadRegister(this->mAddress, VL53L0X_IDENTIFICATION_MODEL_ID, data);
                } while (data != 0xEEU && timeout < 100U);

                if (data == 0xEEU) {
                    this->mI2c.ReadRegister(this->mAddress, VL53L0X_VHV_CONFIG_PAD_SCL_SDA_EXTSUP_HV, data);
                    this->mI2c.WriteRegister(this->mAddress, VL53L0X_VHV_CONFIG_PAD_SCL_SDA_EXTSUP_HV, data | 0x01);

                    // "Set I2C standard mode"
                    this->mI2c.WriteRegister(this->mAddress, 0x88, 0x00);

                    this->mI2c.WriteRegister(this->mAddress, 0x80, 0x01);
                    this->mI2c.WriteRegister(this->mAddress, 0xFF, 0x01);
                    this->mI2c.WriteRegister(this->mAddress, 0x00, 0x00);
                    this->mI2c.ReadRegister(this->mAddress, 0x91, this->mStop);
                    this->mI2c.WriteRegister(this->mAddress, 0x00, 0x01);
                    this->mI2c.WriteRegister(this->mAddress, 0xFF, 0x00);
                    this->mI2c.WriteRegister(this->mAddress, 0x80U, 0x00U);

                    this->Tune();

                    this->mI2c.WriteRegister(this->mAddress, VL53L0X_SYSTEM_INTERRUPT_CONFIG_GPIO, 0x04);
                    this->mI2c.ReadRegister(this->mAddress, VL53L0X_GPIO_HV_MUX_ACTIVE_HIGH, data);
                    this->mI2c.WriteRegister(this->mAddress, VL53L0X_GPIO_HV_MUX_ACTIVE_HIGH, data & ~0x10);
                    this->mI2c.WriteRegister(this->mAddress, VL53L0X_SYSTEM_INTERRUPT_CLEAR, 0x01);

                    this->mI2c.WriteRegister(this->mAddress, VL53L0X_SYSTEM_INTERRUPT_CLEAR, 0x01);

                    if (this->PerformSingleRefCalibration(CALIBRATION_TYPE_VHV)) {
                        if (this->PerformSingleRefCalibration(CALIBRATION_TYPE_PHASE)) {
                            this->mI2c.WriteRegister(this->mAddress,
                                                     VL53L0X_SYSTEM_SEQUENCE_CONFIG,
                                                     RANGE_SEQUENCE_STEP_DSS + RANGE_SEQUENCE_STEP_PRE_RANGE +
                                                             RANGE_SEQUENCE_STEP_FINAL_RANGE);
                            LOG_COMPONENT_DEBUG("Laser", "address 0x%02X Initialized.", this->mAddress);
                            success = Core::Status::CORE_OK;
                        }
                    }
                }
                LOG_COMPONENT_WARNING("Laser", "address 0x%02X Initialized.", this->mAddress);
                return success;
            }

            void Vl53l0x::Update(const uint64_t currentTime) {
                (void) currentTime;

                this->mDistance = this->GetDistance();
                const bool detection = this->mDistance != 0U && this->mDistance <= this->mThreshold;
                if (true == detection) {
                    this->mLed.On();
                    this->Notify({VLX, this->mDistance});
                } else {
                    this->mLed.Off();
                }
            }

            Core::Status Vl53l0x::SetThreshold(uint16_t mThreshold) {
                this->mThreshold = mThreshold;
                return Core::Status::CORE_OK;
            }

            uint16_t Vl53l0x::GetThreshold(void) {
                return this->mThreshold;
            }

            uint16_t Vl53l0x::GetDistance(void) {
                this->mI2c.WriteRegister(this->mAddress, 0x80, 0x01);
                this->mI2c.WriteRegister(this->mAddress, 0xFF, 0x01);
                this->mI2c.WriteRegister(this->mAddress, 0x00, 0x00);
                this->mI2c.ReadRegister(this->mAddress, 0x91, this->mStop);
                this->mI2c.WriteRegister(this->mAddress, 0x00, 0x01);
                this->mI2c.WriteRegister(this->mAddress, 0xFF, 0x00);
                this->mI2c.WriteRegister(this->mAddress, 0x80U, 0x00U);

                this->mI2c.WriteRegister(this->mAddress, VL53L0X_SYSRANGE_START, 0x01);

                uint8_t timeout = 0U;
                uint8_t sysrange_start = 0;
                do {
                    this->mI2c.ReadRegister(this->mAddress, VL53L0X_SYSRANGE_START, sysrange_start);
                    timeout++;
                } while (sysrange_start & 0x01 && timeout < 100U);

                if (timeout >= 100U) {
                    return this->mDistance;
                }
                uint8_t interrupt_status = 0;
                timeout = 0U;
                do {
                    this->mI2c.ReadRegister(this->mAddress, VL53L0X_RESULT_INTERRUPT_STATUS, interrupt_status);
                    timeout++;
                } while ((interrupt_status & 0x07) == 0 && timeout < 100U);

                if (timeout >= 100U) {
                    return this->mDistance;
                }
                this->ReadRegister16Bits(VL53L0X_RESULT_RANGE_STATUS + 10, this->mDistance);
                this->mI2c.WriteRegister(this->mAddress, VL53L0X_SYSTEM_INTERRUPT_CLEAR, 0x01U);
                return this->mDistance;
            }

            bool Vl53l0x::PerformSingleRefCalibration(const calibration_type_t calib) const {
                uint8_t sysrange_start = 0;
                uint8_t sequence_config = 0;
                switch (calib) {
                    case CALIBRATION_TYPE_VHV:
                        sequence_config = 0x01;
                        sysrange_start = 0x01 | 0x40;
                        break;

                    case CALIBRATION_TYPE_PHASE:
                        sequence_config = 0x02;
                        sysrange_start = 0x01 | 0x00;
                        break;
                }
                this->mI2c.WriteRegister(this->mAddress, VL53L0X_SYSTEM_SEQUENCE_CONFIG, sequence_config);

                this->mI2c.WriteRegister(this->mAddress, VL53L0X_SYSRANGE_START, sysrange_start);
                uint8_t interrupt_status = 0;
                uint8_t timeout = 0U;
                do {
                    this->mI2c.ReadRegister(this->mAddress, VL53L0X_RESULT_INTERRUPT_STATUS, interrupt_status);
                    timeout++;
                } while ((interrupt_status & 0x07) == 0 && timeout < 100U);

                if (timeout < 100U) {
                    this->mI2c.WriteRegister(this->mAddress, VL53L0X_SYSTEM_INTERRUPT_CLEAR, 0x01);
                    this->mI2c.WriteRegister(this->mAddress, VL53L0X_SYSRANGE_START, 0x00);
                    return true;
                }
                return false;
            }

            void Vl53l0x::Tune(void) const {
                this->mI2c.WriteRegister(this->mAddress, 0xFFU, 0x01U);
                this->mI2c.WriteRegister(this->mAddress, 0x00U, 0x00U);
                this->mI2c.WriteRegister(this->mAddress, 0xFFU, 0x00U);
                this->mI2c.WriteRegister(this->mAddress, 0x09U, 0x00U);
                this->mI2c.WriteRegister(this->mAddress, 0x10U, 0x00U);
                this->mI2c.WriteRegister(this->mAddress, 0x11U, 0x00U);
                this->mI2c.WriteRegister(this->mAddress, 0x24U, 0x01U);
                this->mI2c.WriteRegister(this->mAddress, 0x25U, 0xFFU);
                this->mI2c.WriteRegister(this->mAddress, 0x75U, 0x00U);
                this->mI2c.WriteRegister(this->mAddress, 0xFFU, 0x01U);
                this->mI2c.WriteRegister(this->mAddress, 0x4EU, 0x2CU);
                this->mI2c.WriteRegister(this->mAddress, 0x48U, 0x00U);
                this->mI2c.WriteRegister(this->mAddress, 0x30U, 0x20U);
                this->mI2c.WriteRegister(this->mAddress, 0xFFU, 0x00U);
                this->mI2c.WriteRegister(this->mAddress, 0x30U, 0x09U);
                this->mI2c.WriteRegister(this->mAddress, 0x54U, 0x00U);
                this->mI2c.WriteRegister(this->mAddress, 0x31U, 0x04U);
                this->mI2c.WriteRegister(this->mAddress, 0x32U, 0x03U);
                this->mI2c.WriteRegister(this->mAddress, 0x40U, 0x83U);
                this->mI2c.WriteRegister(this->mAddress, 0x46U, 0x25U);
                this->mI2c.WriteRegister(this->mAddress, 0x60U, 0x00U);
                this->mI2c.WriteRegister(this->mAddress, 0x27U, 0x00U);
                this->mI2c.WriteRegister(this->mAddress, 0x50U, 0x06U);
                this->mI2c.WriteRegister(this->mAddress, 0x51U, 0x00U);
                this->mI2c.WriteRegister(this->mAddress, 0x52U, 0x96U);
                this->mI2c.WriteRegister(this->mAddress, 0x56U, 0x08U);
                this->mI2c.WriteRegister(this->mAddress, 0x57U, 0x30U);
                this->mI2c.WriteRegister(this->mAddress, 0x61U, 0x00U);
                this->mI2c.WriteRegister(this->mAddress, 0x62U, 0x00U);
                this->mI2c.WriteRegister(this->mAddress, 0x64U, 0x00U);
                this->mI2c.WriteRegister(this->mAddress, 0x65U, 0x00U);
                this->mI2c.WriteRegister(this->mAddress, 0x66U, 0xA0U);
                this->mI2c.WriteRegister(this->mAddress, 0xFFU, 0x01U);
                this->mI2c.WriteRegister(this->mAddress, 0x22U, 0x32U);
                this->mI2c.WriteRegister(this->mAddress, 0x47U, 0x14U);
                this->mI2c.WriteRegister(this->mAddress, 0x49U, 0xFFU);
                this->mI2c.WriteRegister(this->mAddress, 0x4AU, 0x00U);
                this->mI2c.WriteRegister(this->mAddress, 0xFFU, 0x00U);
                this->mI2c.WriteRegister(this->mAddress, 0x7AU, 0x0AU);
                this->mI2c.WriteRegister(this->mAddress, 0x7BU, 0x00U);
                this->mI2c.WriteRegister(this->mAddress, 0x78U, 0x21U);
                this->mI2c.WriteRegister(this->mAddress, 0xFFU, 0x01U);
                this->mI2c.WriteRegister(this->mAddress, 0x23U, 0x34U);
                this->mI2c.WriteRegister(this->mAddress, 0x42U, 0x00U);
                this->mI2c.WriteRegister(this->mAddress, 0x44U, 0xFFU);
                this->mI2c.WriteRegister(this->mAddress, 0x45U, 0x26U);
                this->mI2c.WriteRegister(this->mAddress, 0x46U, 0x05U);
                this->mI2c.WriteRegister(this->mAddress, 0x40U, 0x40U);
                this->mI2c.WriteRegister(this->mAddress, 0x0EU, 0x06U);
                this->mI2c.WriteRegister(this->mAddress, 0x20U, 0x1AU);
                this->mI2c.WriteRegister(this->mAddress, 0x43U, 0x40U);
                this->mI2c.WriteRegister(this->mAddress, 0xFFU, 0x00U);
                this->mI2c.WriteRegister(this->mAddress, 0x34U, 0x03U);
                this->mI2c.WriteRegister(this->mAddress, 0x35U, 0x44U);
                this->mI2c.WriteRegister(this->mAddress, 0xFFU, 0x01U);
                this->mI2c.WriteRegister(this->mAddress, 0x31U, 0x04U);
                this->mI2c.WriteRegister(this->mAddress, 0x4BU, 0x09U);
                this->mI2c.WriteRegister(this->mAddress, 0x4CU, 0x05U);
                this->mI2c.WriteRegister(this->mAddress, 0x4DU, 0x04U);
                this->mI2c.WriteRegister(this->mAddress, 0xFFU, 0x00U);
                this->mI2c.WriteRegister(this->mAddress, 0x44U, 0x00U);
                this->mI2c.WriteRegister(this->mAddress, 0x45U, 0x20U);
                this->mI2c.WriteRegister(this->mAddress, 0x47U, 0x08U);
                this->mI2c.WriteRegister(this->mAddress, 0x48U, 0x28U);
                this->mI2c.WriteRegister(this->mAddress, 0x67U, 0x00U);
                this->mI2c.WriteRegister(this->mAddress, 0x70U, 0x04U);
                this->mI2c.WriteRegister(this->mAddress, 0x71U, 0x01U);
                this->mI2c.WriteRegister(this->mAddress, 0x72U, 0xFEU);
                this->mI2c.WriteRegister(this->mAddress, 0x76U, 0x00U);
                this->mI2c.WriteRegister(this->mAddress, 0x77U, 0x00U);
                this->mI2c.WriteRegister(this->mAddress, 0xFFU, 0x01U);
                this->mI2c.WriteRegister(this->mAddress, 0x0DU, 0x01U);
                this->mI2c.WriteRegister(this->mAddress, 0xFFU, 0x00u);
                this->mI2c.WriteRegister(this->mAddress, 0x80U, 0x01u);
                this->mI2c.WriteRegister(this->mAddress, 0x01U, 0xF8u);
                this->mI2c.WriteRegister(this->mAddress, 0xFFU, 0x01U);
                this->mI2c.WriteRegister(this->mAddress, 0x8EU, 0x01U);
                this->mI2c.WriteRegister(this->mAddress, 0x00U, 0x01U);
                this->mI2c.WriteRegister(this->mAddress, 0xFFU, 0x00U);
                this->mI2c.WriteRegister(this->mAddress, 0x80U, 0x00U);
            } // Vl53l0x::Tune

            bool Vl53l0x::WriteRegister16Bits(const uint8_t   reg,
                                              const uint16_t &data) {
                uint8_t buffer[2U];
                buffer[0U] = static_cast<uint8_t>(data >> 8U);
                buffer[1U] = static_cast<uint8_t>(data & 0x00FFU);
                this->mI2c.WriteRegisters(this->mAddress, reg, buffer, 2U);
                return true;
            }

            bool Vl53l0x::WriteRegister32Bits(const uint8_t   reg,
                                              const uint32_t &data) {
                uint8_t buffer[4U];
                buffer[0U] = static_cast<uint8_t>(data >> 24U);
                buffer[1U] = static_cast<uint8_t>((data & 0x00FF0000U) >> 16U);
                buffer[2U] = static_cast<uint8_t>((data & 0x0000FF00U) >> 8U);
                buffer[3U] = static_cast<uint8_t>(data & 0x000000FFU);
                this->mI2c.WriteRegisters(this->mAddress, reg, buffer, 4U);
                return true;
            }

            bool Vl53l0x::ReadRegister16Bits(const uint8_t reg,
                                             uint16_t     &data) {
                uint8_t buffer[2U];
                this->mI2c.ReadRegisters(this->mAddress, reg, buffer, 2U);
                data = (static_cast<uint16_t>(buffer[0U]) << 8U) + static_cast<uint16_t>(buffer[1U]);
                return true;
            }
        } // namespace Laser
    } // namespace Proximity
} // namespace Component
