#pragma once

#include "../../../Driver/Twi/TwiInterface.h"
#include "../../Led/LedInterface.h"
#include "../SensorProximityInterface.h"

namespace Component
{
    namespace Proximity
    {
        namespace Laser
        {
            using namespace Driver;

            class Vl53l0x : public SensorProximityInterface {
            public:
                static constexpr uint8_t  VL53L0X_ADDRESS = 0x29U;
                static constexpr uint16_t DISTANCE_THRESHOLD = 300U;

                typedef enum {
                    CALIBRATION_TYPE_VHV,
                    CALIBRATION_TYPE_PHASE
                } calibration_type_t;

                Vl53l0x(Twi::TwiInterface &i2c,
                        Led::LedInterface &led,
                        const uint8_t      address = 0x29U);


                ~Vl53l0x() = default;

                Core::Status         Initialize(void);

                void                 Update(const uint64_t currentTime);

                virtual uint16_t     GetDistance(void) final override;

                virtual Core::Status SetThreshold(const uint16_t threshold) final override;

                virtual uint16_t     GetThreshold(void) final override;

            private:
                Twi::TwiInterface &mI2c;
                Led::LedInterface &mLed;
                uint8_t            mAddress;
                uint16_t           mDistance;
                uint16_t           mThreshold;
                uint32_t           mMeasurementTimingBudget = 0U;
                uint8_t            mStop;

                void               Tune(void) const;

                bool               PerformSingleRefCalibration(const calibration_type_t calib) const;

                bool               WriteRegister16Bits(const uint8_t   reg,
                                                       const uint16_t &data);

                bool               WriteRegister32Bits(const uint8_t   reg,
                                                       const uint32_t &data);

                bool               ReadRegister16Bits(const uint8_t reg,
                                                      uint16_t     &data);

            public:
                static constexpr uint8_t VL53L0X_SYSRANGE_START = 0x00U;
                static constexpr uint8_t VL53L0X_SYSTEM_THRESH_HIGH = 0x0CU;
                static constexpr uint8_t VL53L0X_SYSTEM_THRESH_LOW = 0x0EU;
                static constexpr uint8_t VL53L0X_SYSTEM_SEQUENCE_CONFIG = 0x01U;
                static constexpr uint8_t VL53L0X_SYSTEM_RANGE_CONFIG = 0x09U;
                static constexpr uint8_t VL53L0X_SYSTEM_INTERMEASUREMENT_PERIOD = 0x04U;
                static constexpr uint8_t VL53L0X_SYSTEM_INTERRUPT_CONFIG_GPIO = 0x0AU;
                static constexpr uint8_t VL53L0X_GPIO_HV_MUX_ACTIVE_HIGH = 0x84U;
                static constexpr uint8_t VL53L0X_SYSTEM_INTERRUPT_CLEAR = 0x0BU;
                static constexpr uint8_t VL53L0X_RESULT_INTERRUPT_STATUS = 0x13U;
                static constexpr uint8_t VL53L0X_RESULT_RANGE_STATUS = 0x14U;
                static constexpr uint8_t VL53L0X_RESULT_CORE_AMBIENT_WINDOW_EVENTS_RTN = 0xBCU;
                static constexpr uint8_t VL53L0X_RESULT_CORE_RANGING_TOTAL_EVENTS_RTN = 0xC0U;
                static constexpr uint8_t VL53L0X_RESULT_CORE_AMBIENT_WINDOW_EVENTS_REF = 0xD0U;
                static constexpr uint8_t VL53L0X_RESULT_CORE_RANGING_TOTAL_EVENTS_REF = 0xD4U;
                static constexpr uint8_t VL53L0X_RESULT_PEAK_SIGNAL_RATE_REF = 0xB6U;
                static constexpr uint8_t VL53L0X_ALGO_PART_TO_PART_RANGE_OFFSET_MM = 0x28U;
                static constexpr uint8_t VL53L0X_I2C_SLAVE_DEVICE_ADDRESS = 0x8AU;
                static constexpr uint8_t VL53L0X_MSRC_CONFIG_CONTROL = 0x60U;
                static constexpr uint8_t VL53L0X_PRE_RANGE_CONFIG_MIN_SNR = 0x27U;
                static constexpr uint8_t VL53L0X_DYNAMIC_SPAD_REF_EN_START_OFFSET = 0x4FU;
                static constexpr uint8_t VL53L0X_PRE_RANGE_CONFIG_VALID_PHASE_LOW = 0x56U;
                static constexpr uint8_t VL53L0X_PRE_RANGE_CONFIG_VALID_PHASE_HIGH = 0x57U;
                static constexpr uint8_t VL53L0X_PRE_RANGE_MIN_COUNT_RATE_RTN_LIMIT = 0x64U;
                static constexpr uint8_t VL53L0X_FINAL_RANGE_CONFIG_MIN_SNR = 0x67U;
                static constexpr uint8_t VL53L0X_FINAL_RANGE_CONFIG_VALID_PHASE_LOW = 0x47U;
                static constexpr uint8_t VL53L0X_FINAL_RANGE_CONFIG_VALID_PHASE_HIGH = 0x48U;
                static constexpr uint8_t VL53L0X_FINAL_RANGE_CONFIG_MIN_COUNT_RATE_RTN_LIMIT = 0x44U;
                static constexpr uint8_t VL53L0X_PRE_RANGE_CONFIG_SIGMA_THRESH_HI = 0x61U;
                static constexpr uint8_t VL53L0X_PRE_RANGE_CONFIG_SIGMA_THRESH_LO = 0x62U;
                static constexpr uint8_t VL53L0X_PRE_RANGE_CONFIG_VCSEL_PERIOD = 0x50U;
                static constexpr uint8_t VL53L0X_PRE_RANGE_CONFIG_TIMEOUT_MACROP_HI = 0x51U;
                static constexpr uint8_t VL53L0X_PRE_RANGE_CONFIG_TIMEOUT_MACROP_LO = 0x52U;
                static constexpr uint8_t VL53L0X_SYSTEM_HISTOGRAM_BIN = 0x81U;
                static constexpr uint8_t VL53L0X_HISTOGRAM_CONFIG_INITIAL_PHASE_SELECT = 0x33U;
                static constexpr uint8_t VL53L0X_HISTOGRAM_CONFIG_READOUT_CTRL = 0x55U;
                static constexpr uint8_t VL53L0X_FINAL_RANGE_CONFIG_VCSEL_PERIOD = 0x70U;
                static constexpr uint8_t VL53L0X_FINAL_RANGE_CONFIG_TIMEOUT_MACROP_HI = 0x71U;
                static constexpr uint8_t VL53L0X_FINAL_RANGE_CONFIG_TIMEOUT_MACROP_LO = 0x72U;
                static constexpr uint8_t VL53L0X_CROSSTALK_COMPENSATION_PEAK_RATE_MCPS = 0x20U;
                static constexpr uint8_t VL53L0X_MSRC_CONFIG_TIMEOUT_MACROP = 0x46U;
                static constexpr uint8_t VL53L0X_SOFT_RESET_GO2_SOFT_RESET_N = 0xBFU;
                static constexpr uint8_t VL53L0X_IDENTIFICATION_MODEL_ID = 0xC0U;
                static constexpr uint8_t VL53L0X_IDENTIFICATION_REVISION_ID = 0xC2U;
                static constexpr uint8_t VL53L0X_OSC_CALIBRATE_VAL = 0xF8U;
                static constexpr uint8_t VL53L0X_GLOBAL_CONFIG_VCSEL_WIDTH = 0x32U;
                static constexpr uint8_t VL53L0X_GLOBAL_CONFIG_SPAD_ENABLES_REF_0 = 0xB0U;
                static constexpr uint8_t VL53L0X_GLOBAL_CONFIG_SPAD_ENABLES_REF_1 = 0xB1U;
                static constexpr uint8_t VL53L0X_GLOBAL_CONFIG_SPAD_ENABLES_REF_2 = 0xB2U;
                static constexpr uint8_t VL53L0X_GLOBAL_CONFIG_SPAD_ENABLES_REF_3 = 0xB3U;
                static constexpr uint8_t VL53L0X_GLOBAL_CONFIG_SPAD_ENABLES_REF_4 = 0xB4U;
                static constexpr uint8_t VL53L0X_GLOBAL_CONFIG_SPAD_ENABLES_REF_5 = 0xB5U;
                static constexpr uint8_t VL53L0X_GLOBAL_CONFIG_REF_EN_START_SELECT = 0xB6U;
                static constexpr uint8_t VL53L0X_DYNAMIC_SPAD_NUM_REQUESTED_REF_SPAD = 0x4EU;
                static constexpr uint8_t VL53L0X_POWER_MANAGEMENT_GO1_POWER_FORCE = 0x80U;
                static constexpr uint8_t VL53L0X_VHV_CONFIG_PAD_SCL_SDA_EXTSUP_HV = 0x89U;
                static constexpr uint8_t VL53L0X_ALGO_PHASECAL_LIM = 0x30U;
                static constexpr uint8_t VL53L0X_ALGO_PHASECAL_CONFIG_TIMEOUT = 0x30U;
            };
        } // namespace Laser
    } // namespace Proximity
} // namespace Component
