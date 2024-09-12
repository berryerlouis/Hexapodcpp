#pragma once

#include "../../../Cluster/Constants.h"
#include "../../../Driver/Twi/TwiInterface.h"
#include "../../../Driver/Tick/TickInterface.h"
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
#define VL53L0X_ADDRESS                                        0x29U ///< I2C address of the VL53L0X module

#define VL53L0X_SYSRANGE_START                                 0x00U
#define VL53L0X_SYSTEM_THRESH_HIGH                             0x0CU
#define VL53L0X_SYSTEM_THRESH_LOW                              0x0EU
#define VL53L0X_SYSTEM_SEQUENCE_CONFIG                         0x01U
#define VL53L0X_SYSTEM_RANGE_CONFIG                            0x09U
#define VL53L0X_SYSTEM_INTERMEASUREMENT_PERIOD                 0x04U
#define VL53L0X_SYSTEM_INTERRUPT_CONFIG_GPIO                   0x0AU
#define VL53L0X_GPIO_HV_MUX_ACTIVE_HIGH                        0x84U
#define VL53L0X_SYSTEM_INTERRUPT_CLEAR                         0x0BU
#define VL53L0X_RESULT_INTERRUPT_STATUS                        0x13U
#define VL53L0X_RESULT_RANGE_STATUS                            0x14U
#define VL53L0X_RESULT_CORE_AMBIENT_WINDOW_EVENTS_RTN          0xBCU
#define VL53L0X_RESULT_CORE_RANGING_TOTAL_EVENTS_RTN           0xC0U
#define VL53L0X_RESULT_CORE_AMBIENT_WINDOW_EVENTS_REF          0xD0U
#define VL53L0X_RESULT_CORE_RANGING_TOTAL_EVENTS_REF           0xD4U
#define VL53L0X_RESULT_PEAK_SIGNAL_RATE_REF                    0xB6U
#define VL53L0X_ALGO_PART_TO_PART_RANGE_OFFSET_MM              0x28U
#define VL53L0X_I2C_SLAVE_DEVICE_ADDRESS                       0x8AU
#define VL53L0X_MSRC_CONFIG_CONTROL                            0x60U
#define VL53L0X_PRE_RANGE_CONFIG_MIN_SNR                       0x27U
#define VL53L0X_PRE_RANGE_CONFIG_VALID_PHASE_LOW               0x56U
#define VL53L0X_PRE_RANGE_CONFIG_VALID_PHASE_HIGH              0x57U
#define VL53L0X_PRE_RANGE_MIN_COUNT_RATE_RTN_LIMIT             0x64U
#define VL53L0X_FINAL_RANGE_CONFIG_MIN_SNR                     0x67U
#define VL53L0X_FINAL_RANGE_CONFIG_VALID_PHASE_LOW             0x47U
#define VL53L0X_FINAL_RANGE_CONFIG_VALID_PHASE_HIGH            0x48U
#define VL53L0X_FINAL_RANGE_CONFIG_MIN_COUNT_RATE_RTN_LIMIT    0x44U
#define VL53L0X_PRE_RANGE_CONFIG_SIGMA_THRESH_HI               0x61U
#define VL53L0X_PRE_RANGE_CONFIG_SIGMA_THRESH_LO               0x62U
#define VL53L0X_PRE_RANGE_CONFIG_VCSEL_PERIOD                  0x50U
#define VL53L0X_PRE_RANGE_CONFIG_TIMEOUT_MACROP_HI             0x51U
#define VL53L0X_PRE_RANGE_CONFIG_TIMEOUT_MACROP_LO             0x52U
#define VL53L0X_SYSTEM_HISTOGRAM_BIN                           0x81U
#define VL53L0X_HISTOGRAM_CONFIG_INITIAL_PHASE_SELECT          0x33U
#define VL53L0X_HISTOGRAM_CONFIG_READOUT_CTRL                  0x55U
#define VL53L0X_FINAL_RANGE_CONFIG_VCSEL_PERIOD                0x70U
#define VL53L0X_FINAL_RANGE_CONFIG_TIMEOUT_MACROP_HI           0x71U
#define VL53L0X_FINAL_RANGE_CONFIG_TIMEOUT_MACROP_LO           0x72U
#define VL53L0X_CROSSTALK_COMPENSATION_PEAK_RATE_MCPS          0x20U
#define VL53L0X_MSRC_CONFIG_TIMEOUT_MACROP                     0x46U
#define VL53L0X_SOFT_RESET_GO2_SOFT_RESET_N                    0xBFU
#define VL53L0X_IDENTIFICATION_MODEL_ID                        0xC0U
#define VL53L0X_IDENTIFICATION_REVISION_ID                     0xC2U
#define VL53L0X_OSC_CALIBRATE_VAL                              0xF8U
#define VL53L0X_GLOBAL_CONFIG_VCSEL_WIDTH                      0x32U
#define VL53L0X_GLOBAL_CONFIG_SPAD_ENABLES_REF_0               0xB0U
#define VL53L0X_GLOBAL_CONFIG_SPAD_ENABLES_REF_1               0xB1U
#define VL53L0X_GLOBAL_CONFIG_SPAD_ENABLES_REF_2               0xB2U
#define VL53L0X_GLOBAL_CONFIG_SPAD_ENABLES_REF_3               0xB3U
#define VL53L0X_GLOBAL_CONFIG_SPAD_ENABLES_REF_4               0xB4U
#define VL53L0X_GLOBAL_CONFIG_SPAD_ENABLES_REF_5               0xB5U
#define VL53L0X_GLOBAL_CONFIG_REF_EN_START_SELECT              0xB6U
#define VL53L0X_DYNAMIC_SPAD_NUM_REQUESTED_REF_SPAD            0x4EU
#define VL53L0X_DYNAMIC_SPAD_REF_EN_START_OFFSET               0x4FU
#define VL53L0X_POWER_MANAGEMENT_GO1_POWER_FORCE               0x80U
#define VL53L0X_VHV_CONFIG_PAD_SCL_SDA_EXTSUP_HV               0x89U
#define VL53L0X_ALGO_PHASECAL_LIM                              0x30U
#define VL53L0X_ALGO_PHASECAL_CONFIG_TIMEOUT                   0x30U

                static const uint16_t DISTANCE_THRESHOLD = 300U;

                Vl53l0x(Twi::TwiInterface &i2c, Tick::TickInterface &tick, const uint8_t address = 0x29U);

                ~Vl53l0x() = default;

                Core::CoreStatus Initialize(void);

                void Update(const uint64_t currentTime);

                virtual uint16_t GetDistance(void) final override;

                virtual Core::CoreStatus SetThreshold(const uint16_t threshold) final override;

                virtual uint16_t GetThreshold(void) final override;

            private:
                Twi::TwiInterface &mI2c;
                Tick::TickInterface &mTick;
                uint8_t mAddress;
                uint16_t mDistance;
                uint16_t mThreshold;
                uint32_t mMeasurementTimingBudget = 0U;
                uint8_t mStop;

                struct SequenceStepEnables {
                    bool tcc;
                    bool msrc;
                    bool dss;
                    bool pre_range;
                    bool final_range;
                };

                struct SequenceStepTimeouts {
                    uint16_t pre_range_vcsel_period_pclks, final_range_vcsel_period_pclks;
                    uint16_t msrc_dss_tcc_mclks, pre_range_mclks, final_range_mclks;
                    uint32_t msrc_dss_tcc_us, pre_range_us, final_range_us;
                };

                enum VcselPeriodType {
                    VcselPeriodPreRange,
                    VcselPeriodFinalRange
                };


                void StartContinuous(uint32_t period_ms = 50U);

                void Tune(void);

                void GetSequenceStepEnables(SequenceStepEnables *enables);

                void GetSequenceStepTimeouts(SequenceStepEnables const *enables, SequenceStepTimeouts *timeouts);

                uint16_t DecodeTimeout(uint16_t reg_val);

                uint16_t EncodeTimeout(uint32_t timeout_mclks);

                uint8_t GetVcselPulsePeriod(VcselPeriodType type);

                uint32_t TimeoutMclksToMicroseconds(uint16_t timeout_period_mclks, uint8_t vcsel_period_pclks);

                uint32_t TimeoutMicrosecondsToMclks(uint32_t timeout_period_us, uint8_t vcsel_period_pclks);

                bool SetSignalRateLimit(float limit_Mcps);

                float GetSignalRateLimit(void);

                bool GetSpadInfo(uint8_t *count, bool *type_is_aperture);

                bool SetMeasurementTimingBudget(uint32_t budget_us);

                uint32_t GetMeasurementTimingBudget(void);

                bool PerformSingleRefCalibration(uint8_t vhv_init_byte);

                bool SetVcselPulsePeriod(VcselPeriodType type, uint8_t period_pclks);
            };
        }
    }
}
