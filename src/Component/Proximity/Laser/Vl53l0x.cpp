#include "Vl53l0x.h"

namespace Component
{
    namespace Proximity
    {
        namespace Laser
        {
#define decodeVcselPeriod( reg_val )             ( ( ( reg_val ) + 1 ) << 1 )
#define encodeVcselPeriod( period_pclks )        ( ( ( period_pclks ) >> 1 ) - 1 )
#define calcMacroPeriod( vcsel_period_pclks )    ( ( ( (uint32_t) 2304 * ( vcsel_period_pclks ) * 1655 ) + 500 ) / 1000 )


            Vl53l0x::Vl53l0x(Twi::TwiInterface &i2c, Tick::TickInterface &tick, const uint8_t address)
                : mI2c(i2c)
                  , mTick(tick)
                  , mAddress(address)
                  , mDistance(0)
                  , mThreshold(DISTANCE_THRESHOLD)
                  , mMeasurementTimingBudget(0U)
                  , mStop(0U) {
            }

            Core::CoreStatus Vl53l0x::Initialize(void) {
                Core::CoreStatus success = Core::CoreStatus::CORE_ERROR;
                uint8_t data = 0U;

                this->mI2c.ReadRegister(this->mAddress, VL53L0X_IDENTIFICATION_MODEL_ID, data);
                if (data == 0xEE) {
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
                    this->mI2c.WriteRegister(this->mAddress, 0x80, 0x00);

                    // disable SIGNAL_RATE_MSRC (bit 1) and SIGNAL_RATE_PRE_RANGE (bit 4) limit checks
                    this->mI2c.ReadRegister(this->mAddress, VL53L0X_MSRC_CONFIG_CONTROL, data);
                    this->mI2c.WriteRegister(this->mAddress, VL53L0X_MSRC_CONFIG_CONTROL, data | 0x12);

                    // set final range signal rate limit to 0.25 MCPS (million counts per second)
                    uint16_t data16 = 32U;
                    this->mI2c.WriteRegister16Bits(this->mAddress, VL53L0X_FINAL_RANGE_CONFIG_MIN_COUNT_RATE_RTN_LIMIT,
                                                   data16);

                    this->mI2c.WriteRegister(this->mAddress, VL53L0X_SYSTEM_SEQUENCE_CONFIG, 0xFF);

                    uint8_t spad_count;
                    bool spad_type_is_aperture;

                    if (this->GetSpadInfo(&spad_count, &spad_type_is_aperture)) {
                        //The SPAD map (RefGoodSpadMap) is read by VL53L0X_get_info_from_device() in
                        //the API, but the same data seems to be more easily readable from
                        //GLOBAL_CONFIG_SPAD_ENABLES_REF_0 through _6, so read it from there
                        uint8_t ref_spad_map[6];
                        this->mI2c.ReadRegisters(this->mAddress, VL53L0X_GLOBAL_CONFIG_SPAD_ENABLES_REF_0, ref_spad_map,
                                                 6U);

                        // -- VL53L0X_set_reference_spads() begin (assume NVM values are valid)

                        this->mI2c.WriteRegister(this->mAddress, 0xFF, 0x01);
                        this->mI2c.WriteRegister(this->mAddress, VL53L0X_DYNAMIC_SPAD_REF_EN_START_OFFSET, 0x00);
                        this->mI2c.WriteRegister(this->mAddress, VL53L0X_DYNAMIC_SPAD_NUM_REQUESTED_REF_SPAD, 0x2C);
                        this->mI2c.WriteRegister(this->mAddress, 0xFF, 0x00);
                        this->mI2c.WriteRegister(this->mAddress, VL53L0X_GLOBAL_CONFIG_REF_EN_START_SELECT, 0xB4);

                        uint8_t first_spad_to_enable = spad_type_is_aperture ? 12 : 0;
                        uint8_t spads_enabled = 0U;

                        for (size_t i = 0U; i < 48U; i++) {
                            if (i < first_spad_to_enable || spads_enabled == spad_count) {
                                // This bit is lower than the first one that should be enabled, or
                                // (reference_spad_count) bits have already been enabled, so zero this bit
                                ref_spad_map[i / 8] &= ~(1 << (i % 8));
                            } else if ((ref_spad_map[i / 8] >> (i % 8)) & 0x1) {
                                spads_enabled++;
                            }
                        }

                        this->mI2c.WriteRegisters(this->mAddress, VL53L0X_GLOBAL_CONFIG_SPAD_ENABLES_REF_0,
                                                  ref_spad_map, 6U);

                        this->Tune();

                        // "Set interrupt config to new sample ready"
                        this->mI2c.WriteRegister(this->mAddress, VL53L0X_SYSTEM_INTERRUPT_CONFIG_GPIO, 0x04);
                        this->mI2c.ReadRegister(this->mAddress, VL53L0X_GPIO_HV_MUX_ACTIVE_HIGH, data);
                        this->mI2c.WriteRegister(this->mAddress, VL53L0X_GPIO_HV_MUX_ACTIVE_HIGH, data & ~0x10);
                        this->mI2c.WriteRegister(this->mAddress, VL53L0X_SYSTEM_INTERRUPT_CLEAR, 0x01);


                        this->mMeasurementTimingBudget = this->GetMeasurementTimingBudget();

                        // "Disable MSRC and TCC by default"
                        this->mI2c.WriteRegister(this->mAddress, VL53L0X_SYSTEM_SEQUENCE_CONFIG, 0xE8);
                        // "Recalculate timing budget"
                        this->SetMeasurementTimingBudget(this->mMeasurementTimingBudget);
                        this->mI2c.WriteRegister(this->mAddress, VL53L0X_SYSTEM_SEQUENCE_CONFIG, 0x01);

                        if (this->PerformSingleRefCalibration(0x40) == true) {
                            this->mI2c.WriteRegister(this->mAddress, VL53L0X_SYSTEM_SEQUENCE_CONFIG, 0x02);

                            if (this->PerformSingleRefCalibration(0x00) == true) {
                                // "restore the previous Sequence Config"UT_CMP_COMMUNICATION.cpp:70
                                this->mI2c.WriteRegister(this->mAddress, VL53L0X_SYSTEM_SEQUENCE_CONFIG, 0xE8);
                                this->SetVcselPulsePeriod(VcselPeriodPreRange, 18);
                                this->SetVcselPulsePeriod(VcselPeriodFinalRange, 14);

                                this->StartContinuous();
                                success = Core::CoreStatus::CORE_OK;
                            }
                        }
                    }
                }
                return (success);
            }

            void Vl53l0x::Update(const uint64_t currentTime) {
                (void) currentTime;

                this->mDistance = this->GetDistance();
                const bool detection = this->mDistance != 0U && this->mDistance <= this->mThreshold;
                if (true == detection) {
                    this->Notify(VLX, detection);
                }
            }

            Core::CoreStatus Vl53l0x::SetThreshold(uint16_t mThreshold) {
                this->mThreshold = mThreshold;
                return (Core::CoreStatus::CORE_OK);
            }

            uint16_t Vl53l0x::GetThreshold(void) {
                return (this->mThreshold);
            }

            uint16_t Vl53l0x::GetDistance(void) {
                uint8_t data = 0U;

                this->mI2c.ReadRegister(this->mAddress, VL53L0X_RESULT_INTERRUPT_STATUS, data);

                if ((data & 0x07) != 0U) {
                    this->mI2c.ReadRegister16Bits(this->mAddress, VL53L0X_RESULT_RANGE_STATUS + 10, this->mDistance);
                    this->mI2c.WriteRegister(this->mAddress, VL53L0X_SYSTEM_INTERRUPT_CLEAR, 0x01);
                }
                return (this->mDistance);
            }

            void Vl53l0x::StartContinuous(uint32_t period_ms) {
                this->mI2c.WriteRegister(this->mAddress, 0x80, 0x01);
                this->mI2c.WriteRegister(this->mAddress, 0xFF, 0x01);
                this->mI2c.WriteRegister(this->mAddress, 0x00, 0x00);
                this->mI2c.WriteRegister(this->mAddress, 0x91, this->mStop);
                this->mI2c.WriteRegister(this->mAddress, 0x00, 0x01);
                this->mI2c.WriteRegister(this->mAddress, 0xFF, 0x00);
                this->mI2c.WriteRegister(this->mAddress, 0x80, 0x00);

                if (period_ms != 0U) {
                    // continuous timed mode

                    uint16_t osc_calibrate_val = 0U;
                    this->mI2c.ReadRegister16Bits(this->mAddress, VL53L0X_OSC_CALIBRATE_VAL, osc_calibrate_val);

                    if (osc_calibrate_val != 0) {
                        period_ms *= osc_calibrate_val;
                    }
                    this->mI2c.WriteRegister32Bits(this->mAddress, VL53L0X_SYSTEM_INTERMEASUREMENT_PERIOD, period_ms);


                    this->mI2c.WriteRegister(this->mAddress, VL53L0X_SYSRANGE_START, 0x04);
                } else {
                    // continuous back-to-back mode
                    this->mI2c.WriteRegister(this->mAddress, VL53L0X_SYSRANGE_START, 0x02);
                }
            }

            bool Vl53l0x::SetMeasurementTimingBudget(uint32_t budget_us) {
                SequenceStepEnables enables;
                SequenceStepTimeouts timeouts;

                uint16_t const StartOverhead = 1910;
                uint16_t const EndOverhead = 960;
                uint16_t const MsrcOverhead = 660;
                uint16_t const TccOverhead = 590;
                uint16_t const DssOverhead = 690;
                uint16_t const PreRangeOverhead = 660;
                uint16_t const FinalRangeOverhead = 550;

                uint32_t const MinTimingBudget = 20000;

                if (budget_us < MinTimingBudget) {
                    return (false);
                }
                uint32_t used_budget_us = StartOverhead + EndOverhead;

                this->GetSequenceStepEnables(&enables);
                this->GetSequenceStepTimeouts(&enables, &timeouts);

                if (enables.tcc) {
                    used_budget_us += (timeouts.msrc_dss_tcc_us + TccOverhead);
                }

                if (enables.dss) {
                    used_budget_us += 2 * (timeouts.msrc_dss_tcc_us + DssOverhead);
                } else if (enables.msrc) {
                    used_budget_us += (timeouts.msrc_dss_tcc_us + MsrcOverhead);
                }

                if (enables.pre_range) {
                    used_budget_us += (timeouts.pre_range_us + PreRangeOverhead);
                }

                if (enables.final_range) {
                    used_budget_us += FinalRangeOverhead;

                    if (used_budget_us > budget_us) {
                        // "Requested timeout too big."
                        return (false);
                    }
                    uint32_t final_range_timeout_us = budget_us - used_budget_us;
                    uint32_t final_range_timeout_mclks = this->TimeoutMicrosecondsToMclks(final_range_timeout_us,
                        timeouts.final_range_vcsel_period_pclks);

                    if (enables.pre_range) {
                        final_range_timeout_mclks += timeouts.pre_range_mclks;
                    }
                    uint16_t data16 = this->EncodeTimeout(final_range_timeout_mclks);
                    this->mI2c.WriteRegister16Bits(this->mAddress, VL53L0X_FINAL_RANGE_CONFIG_TIMEOUT_MACROP_HI,
                                                   data16);

                    this->mMeasurementTimingBudget = budget_us;
                }
                return (true);
            } // Vl53l0x::SetMeasurementTimingBudget

            uint32_t Vl53l0x::GetMeasurementTimingBudget(void) {
                SequenceStepEnables enables;
                SequenceStepTimeouts timeouts;

                uint16_t const StartOverhead = 1910;
                uint16_t const EndOverhead = 960;
                uint16_t const MsrcOverhead = 660;
                uint16_t const TccOverhead = 590;
                uint16_t const DssOverhead = 690;
                uint16_t const PreRangeOverhead = 660;
                uint16_t const FinalRangeOverhead = 550;

                // "Start and end overhead times always present"
                uint32_t budget_us = StartOverhead + EndOverhead;

                this->GetSequenceStepEnables(&enables);
                this->GetSequenceStepTimeouts(&enables, &timeouts);

                if (enables.tcc) {
                    budget_us += (timeouts.msrc_dss_tcc_us + TccOverhead);
                }

                if (enables.dss) {
                    budget_us += 2 * (timeouts.msrc_dss_tcc_us + DssOverhead);
                } else if (enables.msrc) {
                    budget_us += (timeouts.msrc_dss_tcc_us + MsrcOverhead);
                }

                if (enables.pre_range) {
                    budget_us += (timeouts.pre_range_us + PreRangeOverhead);
                }

                if (enables.final_range) {
                    budget_us += (timeouts.final_range_us + FinalRangeOverhead);
                }
                this->mMeasurementTimingBudget = budget_us;
                return (budget_us);
            } // Vl53l0x::GetMeasurementTimingBudget

            bool Vl53l0x::GetSpadInfo(uint8_t *count, bool *type_is_aperture) {
                uint8_t data = 0U;

                this->mI2c.WriteRegister(this->mAddress, 0x80, 0x01);
                this->mI2c.WriteRegister(this->mAddress, 0xFF, 0x01);
                this->mI2c.WriteRegister(this->mAddress, 0x00, 0x00);

                this->mI2c.WriteRegister(this->mAddress, 0xFF, 0x06);
                this->mI2c.ReadRegister(this->mAddress, 0x83, data);
                this->mI2c.WriteRegister(this->mAddress, 0x83, data | 0x04);
                this->mI2c.WriteRegister(this->mAddress, 0xFF, 0x07);
                this->mI2c.WriteRegister(this->mAddress, 0x81, 0x01);

                this->mI2c.WriteRegister(this->mAddress, 0x80, 0x01);

                this->mI2c.WriteRegister(this->mAddress, 0x94, 0x6b);
                this->mI2c.WriteRegister(this->mAddress, 0x83, 0x00);

                data = 0U;
                uint64_t timeout = this->mTick.GetMs();

                do {
                    this->mI2c.ReadRegister(this->mAddress, 0x83, data);
                    if (((uint64_t) (this->mTick.GetMs() - timeout) > 500)) {
                        return (false);
                    }
                } while (data == 0);

                this->mI2c.WriteRegister(this->mAddress, 0x83, 0x01);

                this->mI2c.ReadRegister(this->mAddress, 0x92, data);

                *count = data & 0x7f;
                *type_is_aperture = (data >> 7) & 0x01;

                this->mI2c.WriteRegister(this->mAddress, 0x81, 0x00);
                this->mI2c.WriteRegister(this->mAddress, 0xFF, 0x06);
                this->mI2c.ReadRegister(this->mAddress, 0x83, data);
                this->mI2c.WriteRegister(this->mAddress, 0x83, data & ~0x04);
                this->mI2c.WriteRegister(this->mAddress, 0xFF, 0x01);
                this->mI2c.WriteRegister(this->mAddress, 0x00, 0x01);

                this->mI2c.WriteRegister(this->mAddress, 0xFF, 0x00);
                this->mI2c.WriteRegister(this->mAddress, 0x80, 0x00);

                return (true);
            }

            void Vl53l0x::GetSequenceStepEnables(SequenceStepEnables *enables) {
                uint8_t sequence_config = 0U;

                this->mI2c.ReadRegister(this->mAddress, VL53L0X_SYSTEM_SEQUENCE_CONFIG, sequence_config);

                enables->tcc = (sequence_config >> 4) & 0x1;
                enables->dss = (sequence_config >> 3) & 0x1;
                enables->msrc = (sequence_config >> 2) & 0x1;
                enables->pre_range = (sequence_config >> 6) & 0x1;
                enables->final_range = (sequence_config >> 7) & 0x1;
            }

            void Vl53l0x::GetSequenceStepTimeouts(SequenceStepEnables const *enables, SequenceStepTimeouts *timeouts) {
                timeouts->pre_range_vcsel_period_pclks = this->GetVcselPulsePeriod(VcselPeriodPreRange);

                timeouts->msrc_dss_tcc_mclks = 0U;
                this->mI2c.ReadRegister16Bits(this->mAddress, VL53L0X_MSRC_CONFIG_TIMEOUT_MACROP,
                                              timeouts->msrc_dss_tcc_mclks);
                timeouts->msrc_dss_tcc_mclks += 1;
                timeouts->msrc_dss_tcc_us =
                        this->TimeoutMclksToMicroseconds(timeouts->msrc_dss_tcc_mclks,
                                                         timeouts->pre_range_vcsel_period_pclks);

                timeouts->pre_range_mclks = 0U;
                this->mI2c.ReadRegister16Bits(this->mAddress, VL53L0X_PRE_RANGE_CONFIG_TIMEOUT_MACROP_HI,
                                              timeouts->pre_range_mclks);
                this->DecodeTimeout(timeouts->pre_range_mclks);

                timeouts->pre_range_us =
                        this->TimeoutMclksToMicroseconds(timeouts->pre_range_mclks,
                                                         timeouts->pre_range_vcsel_period_pclks);

                timeouts->final_range_vcsel_period_pclks = this->GetVcselPulsePeriod(VcselPeriodFinalRange);

                this->mI2c.ReadRegister16Bits(this->mAddress, VL53L0X_FINAL_RANGE_CONFIG_TIMEOUT_MACROP_HI,
                                              timeouts->final_range_mclks);

                if (enables->pre_range) {
                    timeouts->final_range_mclks -= timeouts->pre_range_mclks;
                }
                timeouts->final_range_us =
                        this->TimeoutMclksToMicroseconds(timeouts->final_range_mclks,
                                                         timeouts->final_range_vcsel_period_pclks);
            }

            uint16_t Vl53l0x::DecodeTimeout(uint16_t reg_val) {
                // format: "(LSByte * 2^MSByte) + 1"
                return ((uint16_t) ((reg_val & 0x00FF) << (uint16_t) ((reg_val & 0xFF00) >> 8)) + 1);
            }

            uint16_t Vl53l0x::EncodeTimeout(uint32_t timeout_mclks) {
                // format: "(LSByte * 2^MSByte) + 1"

                uint32_t ls_byte = 0U;
                uint16_t ms_byte = 0U;

                if (timeout_mclks > 0) {
                    ls_byte = timeout_mclks - 1;

                    while ((ls_byte & 0xFFFFFF00) > 0) {
                        ls_byte >>= 1;
                        ms_byte++;
                    }
                    return ((ms_byte << 8) | (ls_byte & 0xFF));
                } else {
                    return (0);
                }
            }

            uint8_t Vl53l0x::GetVcselPulsePeriod(VcselPeriodType type) {
                uint8_t data = 255;

                if (type == VcselPeriodPreRange) {
                    this->mI2c.ReadRegister(this->mAddress, VL53L0X_PRE_RANGE_CONFIG_VCSEL_PERIOD, data);
                    return (decodeVcselPeriod(data));
                } else if (type == VcselPeriodFinalRange) {
                    this->mI2c.ReadRegister(this->mAddress, VL53L0X_FINAL_RANGE_CONFIG_VCSEL_PERIOD, data);
                    return (decodeVcselPeriod(data));
                }
                return (255);
            }

            bool Vl53l0x::SetVcselPulsePeriod(VcselPeriodType type, uint8_t period_pclks) {
                uint8_t vcsel_period_reg = encodeVcselPeriod(period_pclks);

                SequenceStepEnables enables;
                SequenceStepTimeouts timeouts;

                this->GetSequenceStepEnables(&enables);
                this->GetSequenceStepTimeouts(&enables, &timeouts);

                if (type == VcselPeriodPreRange) {
                    // "Set phase check limits"
                    switch (period_pclks) {
                        case 12:
                            this->mI2c.WriteRegister(this->mAddress, VL53L0X_PRE_RANGE_CONFIG_VALID_PHASE_HIGH, 0x18);
                            break;

                        case 14:
                            this->mI2c.WriteRegister(this->mAddress, VL53L0X_PRE_RANGE_CONFIG_VALID_PHASE_HIGH, 0x30);
                            break;

                        case 16:
                            this->mI2c.WriteRegister(this->mAddress, VL53L0X_PRE_RANGE_CONFIG_VALID_PHASE_HIGH, 0x40);
                            break;

                        case 18:
                            this->mI2c.WriteRegister(this->mAddress, VL53L0X_PRE_RANGE_CONFIG_VALID_PHASE_HIGH, 0x50);
                            break;

                        default:
                            // invalid period
                            return (false);
                    }
                    this->mI2c.WriteRegister(this->mAddress, VL53L0X_PRE_RANGE_CONFIG_VALID_PHASE_LOW, 0x08);

                    // apply new VCSEL period
                    this->mI2c.WriteRegister(this->mAddress, VL53L0X_PRE_RANGE_CONFIG_VCSEL_PERIOD, vcsel_period_reg);

                    // update timeouts

                    uint16_t new_pre_range_timeout_mclks =
                            this->TimeoutMicrosecondsToMclks(timeouts.pre_range_us, period_pclks);

                    uint16_t data16 = this->EncodeTimeout(new_pre_range_timeout_mclks);
                    this->mI2c.WriteRegister16Bits(this->mAddress, VL53L0X_PRE_RANGE_CONFIG_TIMEOUT_MACROP_HI, data16);

                    uint16_t new_msrc_timeout_mclks =
                            this->TimeoutMicrosecondsToMclks(timeouts.msrc_dss_tcc_us, period_pclks);

                    this->mI2c.WriteRegister(this->mAddress, VL53L0X_MSRC_CONFIG_TIMEOUT_MACROP,
                                             (new_msrc_timeout_mclks > 256) ? 255 : (new_msrc_timeout_mclks - 1));
                } else if (type == VcselPeriodFinalRange) {
                    switch (period_pclks) {
                        case 8:
                            this->mI2c.WriteRegister(this->mAddress, VL53L0X_FINAL_RANGE_CONFIG_VALID_PHASE_HIGH, 0x10);
                            this->mI2c.WriteRegister(this->mAddress, VL53L0X_FINAL_RANGE_CONFIG_VALID_PHASE_LOW, 0x08);
                            this->mI2c.WriteRegister(this->mAddress, VL53L0X_GLOBAL_CONFIG_VCSEL_WIDTH, 0x02);
                            this->mI2c.WriteRegister(this->mAddress, VL53L0X_ALGO_PHASECAL_CONFIG_TIMEOUT, 0x0C);
                            this->mI2c.WriteRegister(this->mAddress, 0xFF, 0x01);
                            this->mI2c.WriteRegister(this->mAddress, VL53L0X_ALGO_PHASECAL_LIM, 0x30);
                            this->mI2c.WriteRegister(this->mAddress, 0xFF, 0x00);
                            break;

                        case 10:
                            this->mI2c.WriteRegister(this->mAddress, VL53L0X_FINAL_RANGE_CONFIG_VALID_PHASE_HIGH, 0x28);
                            this->mI2c.WriteRegister(this->mAddress, VL53L0X_FINAL_RANGE_CONFIG_VALID_PHASE_LOW, 0x08);
                            this->mI2c.WriteRegister(this->mAddress, VL53L0X_GLOBAL_CONFIG_VCSEL_WIDTH, 0x03);
                            this->mI2c.WriteRegister(this->mAddress, VL53L0X_ALGO_PHASECAL_CONFIG_TIMEOUT, 0x09);
                            this->mI2c.WriteRegister(this->mAddress, 0xFF, 0x01);
                            this->mI2c.WriteRegister(this->mAddress, VL53L0X_ALGO_PHASECAL_LIM, 0x20);
                            this->mI2c.WriteRegister(this->mAddress, 0xFF, 0x00);
                            break;

                        case 12:
                            this->mI2c.WriteRegister(this->mAddress, VL53L0X_FINAL_RANGE_CONFIG_VALID_PHASE_HIGH, 0x38);
                            this->mI2c.WriteRegister(this->mAddress, VL53L0X_FINAL_RANGE_CONFIG_VALID_PHASE_LOW, 0x08);
                            this->mI2c.WriteRegister(this->mAddress, VL53L0X_GLOBAL_CONFIG_VCSEL_WIDTH, 0x03);
                            this->mI2c.WriteRegister(this->mAddress, VL53L0X_ALGO_PHASECAL_CONFIG_TIMEOUT, 0x08);
                            this->mI2c.WriteRegister(this->mAddress, 0xFF, 0x01);
                            this->mI2c.WriteRegister(this->mAddress, VL53L0X_ALGO_PHASECAL_LIM, 0x20);
                            this->mI2c.WriteRegister(this->mAddress, 0xFF, 0x00);
                            break;

                        case 14:
                            this->mI2c.WriteRegister(this->mAddress, VL53L0X_FINAL_RANGE_CONFIG_VALID_PHASE_HIGH, 0x48);
                            this->mI2c.WriteRegister(this->mAddress, VL53L0X_FINAL_RANGE_CONFIG_VALID_PHASE_LOW, 0x08);
                            this->mI2c.WriteRegister(this->mAddress, VL53L0X_GLOBAL_CONFIG_VCSEL_WIDTH, 0x03);
                            this->mI2c.WriteRegister(this->mAddress, VL53L0X_ALGO_PHASECAL_CONFIG_TIMEOUT, 0x07);
                            this->mI2c.WriteRegister(this->mAddress, 0xFF, 0x01);
                            this->mI2c.WriteRegister(this->mAddress, VL53L0X_ALGO_PHASECAL_LIM, 0x20);
                            this->mI2c.WriteRegister(this->mAddress, 0xFF, 0x00);
                            break;

                        default:
                            // invalid period
                            return (false);
                    }
                    // apply new VCSEL period
                    this->mI2c.WriteRegister(this->mAddress, VL53L0X_FINAL_RANGE_CONFIG_VCSEL_PERIOD, vcsel_period_reg);

                    // update timeouts
                    uint16_t new_final_range_timeout_mclks =
                            this->TimeoutMicrosecondsToMclks(timeouts.final_range_us, period_pclks);

                    if (enables.pre_range) {
                        new_final_range_timeout_mclks += timeouts.pre_range_mclks;
                    }
                    uint16_t data16 = this->EncodeTimeout(new_final_range_timeout_mclks);
                    this->mI2c.WriteRegister16Bits(this->mAddress, VL53L0X_FINAL_RANGE_CONFIG_TIMEOUT_MACROP_HI,
                                                   data16);
                }
                this->SetMeasurementTimingBudget(this->mMeasurementTimingBudget);

                uint8_t sequence_config = 0U;
                this->mI2c.ReadRegister(this->mAddress, VL53L0X_SYSTEM_SEQUENCE_CONFIG, sequence_config);
                this->mI2c.WriteRegister(this->mAddress, VL53L0X_SYSTEM_SEQUENCE_CONFIG, 0x02);
                this->PerformSingleRefCalibration(0x0);
                this->mI2c.WriteRegister(this->mAddress, VL53L0X_SYSTEM_SEQUENCE_CONFIG, sequence_config);


                return (true);
            } // Vl53l0x::SetVcselPulsePeriod

            uint32_t Vl53l0x::TimeoutMclksToMicroseconds(uint16_t timeout_period_mclks, uint8_t vcsel_period_pclks) {
                uint32_t macro_period_ns = calcMacroPeriod(vcsel_period_pclks);

                return (((timeout_period_mclks * macro_period_ns) + 500) / 1000);
            }

            uint32_t Vl53l0x::TimeoutMicrosecondsToMclks(uint32_t timeout_period_us, uint8_t vcsel_period_pclks) {
                uint32_t macro_period_ns = calcMacroPeriod(vcsel_period_pclks);

                return (((timeout_period_us * 1000) + (macro_period_ns / 2)) / macro_period_ns);
            }

            bool Vl53l0x::SetSignalRateLimit(float limit_Mcps) {
                if (limit_Mcps < 0 || limit_Mcps > 511.99) {
                    return (false);
                }
                // Q9.7 fixed point format (9 integer bits, 7 fractional bits)
                uint16_t data16 = limit_Mcps * (1 << 7);
                this->mI2c.WriteRegister16Bits(this->mAddress, VL53L0X_FINAL_RANGE_CONFIG_MIN_COUNT_RATE_RTN_LIMIT,
                                               data16);
                return (true);
            }

            float Vl53l0x::GetSignalRateLimit(void) {
                uint16_t data16 = 0U;

                this->mI2c.ReadRegister16Bits(this->mAddress, VL53L0X_FINAL_RANGE_CONFIG_MIN_COUNT_RATE_RTN_LIMIT,
                                              data16);
                return ((float) (data16 / (1 << 7)));
            }

            bool Vl53l0x::PerformSingleRefCalibration(uint8_t vhv_init_byte) {
                this->mI2c.WriteRegister(this->mAddress, VL53L0X_SYSRANGE_START, 0x01 | vhv_init_byte);

                uint64_t timeout = this->mTick.GetMs();
                uint8_t data = 0U;

                do {
                    this->mI2c.ReadRegister(this->mAddress, VL53L0X_RESULT_INTERRUPT_STATUS, data);

                    if (((uint64_t) (this->mTick.GetMs() - timeout) > 500)) {
                        return (false);
                    }
                } while ((data & 0x07) == 0);

                this->mI2c.WriteRegister(this->mAddress, VL53L0X_SYSTEM_INTERRUPT_CLEAR, 0x01);

                this->mI2c.WriteRegister(this->mAddress, VL53L0X_SYSRANGE_START, 0x00);

                return (true);
            }

            void Vl53l0x::Tune(void) {
                this->mI2c.WriteRegister(this->mAddress, 0xFF, 0x01);
                this->mI2c.WriteRegister(this->mAddress, 0x00, 0x00);

                this->mI2c.WriteRegister(this->mAddress, 0xFF, 0x00);
                this->mI2c.WriteRegister(this->mAddress, 0x09, 0x00);
                this->mI2c.WriteRegister(this->mAddress, 0x10, 0x00);
                this->mI2c.WriteRegister(this->mAddress, 0x11, 0x00);

                this->mI2c.WriteRegister(this->mAddress, 0x24, 0x01);
                this->mI2c.WriteRegister(this->mAddress, 0x25, 0xFF);
                this->mI2c.WriteRegister(this->mAddress, 0x75, 0x00);

                this->mI2c.WriteRegister(this->mAddress, 0xFF, 0x01);
                this->mI2c.WriteRegister(this->mAddress, 0x4E, 0x2C);
                this->mI2c.WriteRegister(this->mAddress, 0x48, 0x00);
                this->mI2c.WriteRegister(this->mAddress, 0x30, 0x20);

                this->mI2c.WriteRegister(this->mAddress, 0xFF, 0x00);
                this->mI2c.WriteRegister(this->mAddress, 0x30, 0x09);
                this->mI2c.WriteRegister(this->mAddress, 0x54, 0x00);
                this->mI2c.WriteRegister(this->mAddress, 0x31, 0x04);
                this->mI2c.WriteRegister(this->mAddress, 0x32, 0x03);
                this->mI2c.WriteRegister(this->mAddress, 0x40, 0x83);
                this->mI2c.WriteRegister(this->mAddress, 0x46, 0x25);
                this->mI2c.WriteRegister(this->mAddress, 0x60, 0x00);
                this->mI2c.WriteRegister(this->mAddress, 0x27, 0x00);
                this->mI2c.WriteRegister(this->mAddress, 0x50, 0x06);
                this->mI2c.WriteRegister(this->mAddress, 0x51, 0x00);
                this->mI2c.WriteRegister(this->mAddress, 0x52, 0x96);
                this->mI2c.WriteRegister(this->mAddress, 0x56, 0x08);
                this->mI2c.WriteRegister(this->mAddress, 0x57, 0x30);
                this->mI2c.WriteRegister(this->mAddress, 0x61, 0x00);
                this->mI2c.WriteRegister(this->mAddress, 0x62, 0x00);
                this->mI2c.WriteRegister(this->mAddress, 0x64, 0x00);
                this->mI2c.WriteRegister(this->mAddress, 0x65, 0x00);
                this->mI2c.WriteRegister(this->mAddress, 0x66, 0xA0);

                this->mI2c.WriteRegister(this->mAddress, 0xFF, 0x01);
                this->mI2c.WriteRegister(this->mAddress, 0x22, 0x32);
                this->mI2c.WriteRegister(this->mAddress, 0x47, 0x14);
                this->mI2c.WriteRegister(this->mAddress, 0x49, 0xFF);
                this->mI2c.WriteRegister(this->mAddress, 0x4A, 0x00);

                this->mI2c.WriteRegister(this->mAddress, 0xFF, 0x00);
                this->mI2c.WriteRegister(this->mAddress, 0x7A, 0x0A);
                this->mI2c.WriteRegister(this->mAddress, 0x7B, 0x00);
                this->mI2c.WriteRegister(this->mAddress, 0x78, 0x21);

                this->mI2c.WriteRegister(this->mAddress, 0xFF, 0x01);
                this->mI2c.WriteRegister(this->mAddress, 0x23, 0x34);
                this->mI2c.WriteRegister(this->mAddress, 0x42, 0x00);
                this->mI2c.WriteRegister(this->mAddress, 0x44, 0xFF);
                this->mI2c.WriteRegister(this->mAddress, 0x45, 0x26);
                this->mI2c.WriteRegister(this->mAddress, 0x46, 0x05);
                this->mI2c.WriteRegister(this->mAddress, 0x40, 0x40);
                this->mI2c.WriteRegister(this->mAddress, 0x0E, 0x06);
                this->mI2c.WriteRegister(this->mAddress, 0x20, 0x1A);
                this->mI2c.WriteRegister(this->mAddress, 0x43, 0x40);

                this->mI2c.WriteRegister(this->mAddress, 0xFF, 0x00);
                this->mI2c.WriteRegister(this->mAddress, 0x34, 0x03);
                this->mI2c.WriteRegister(this->mAddress, 0x35, 0x44);

                this->mI2c.WriteRegister(this->mAddress, 0xFF, 0x01);
                this->mI2c.WriteRegister(this->mAddress, 0x31, 0x04);
                this->mI2c.WriteRegister(this->mAddress, 0x4B, 0x09);
                this->mI2c.WriteRegister(this->mAddress, 0x4C, 0x05);
                this->mI2c.WriteRegister(this->mAddress, 0x4D, 0x04);

                this->mI2c.WriteRegister(this->mAddress, 0xFF, 0x00);
                this->mI2c.WriteRegister(this->mAddress, 0x44, 0x00);
                this->mI2c.WriteRegister(this->mAddress, 0x45, 0x20);
                this->mI2c.WriteRegister(this->mAddress, 0x47, 0x08);
                this->mI2c.WriteRegister(this->mAddress, 0x48, 0x28);
                this->mI2c.WriteRegister(this->mAddress, 0x67, 0x00);
                this->mI2c.WriteRegister(this->mAddress, 0x70, 0x04);
                this->mI2c.WriteRegister(this->mAddress, 0x71, 0x01);
                this->mI2c.WriteRegister(this->mAddress, 0x72, 0xFE);
                this->mI2c.WriteRegister(this->mAddress, 0x76, 0x00);
                this->mI2c.WriteRegister(this->mAddress, 0x77, 0x00);

                this->mI2c.WriteRegister(this->mAddress, 0xFF, 0x01);
                this->mI2c.WriteRegister(this->mAddress, 0x0D, 0x01);

                this->mI2c.WriteRegister(this->mAddress, 0xFF, 0x00);
                this->mI2c.WriteRegister(this->mAddress, 0x80, 0x01);
                this->mI2c.WriteRegister(this->mAddress, 0x01, 0xF8);

                this->mI2c.WriteRegister(this->mAddress, 0xFF, 0x01);
                this->mI2c.WriteRegister(this->mAddress, 0x8E, 0x01);
                this->mI2c.WriteRegister(this->mAddress, 0x00, 0x01);
                this->mI2c.WriteRegister(this->mAddress, 0xFF, 0x00);
                this->mI2c.WriteRegister(this->mAddress, 0x80, 0x00);
            } // Vl53l0x::Tune
        }
    }
}
