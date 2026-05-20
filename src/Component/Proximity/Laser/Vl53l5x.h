#pragma once

#include "../../../Driver/Twi/TwiInterface.h"
#include "../../Led/LedInterface.h"
#include "Vl53l5x_buffers.h"
#include "Vl53lxxInterface.h"

namespace Component
{
    namespace Proximity
    {
        namespace Laser
        {
            using namespace Driver;

            class Vl53l5x : public Vl53lxxInterface {
            public:
                static constexpr uint8_t  VL53L5X_ADDRESS = 0x29U;
                static constexpr uint16_t DISTANCE_THRESHOLD = 1000U;

                explicit Vl53l5x(Twi::TwiInterface &i2c,
                                 Led::LedInterface &led,
                                 const uint8_t      address = VL53L5X_ADDRESS);

                ~Vl53l5x() = default;

                Core::Status Initialize(void) override;

                void         Update(const uint64_t currentTime) override;

                uint16_t     GetDistance(void) final override;

                Core::Status SetThreshold(const uint16_t threshold) final override;

                uint16_t     GetThreshold(void) final override;

                bool         DataIsReady(void);

                void         ReadData(void);

                uint8_t      GetPixelCount(void) const;

                uint8_t      GetTargetStatus(const uint8_t pixel) const;

                int16_t      GetDistanceMm(const uint8_t pixel) const;

                uint8_t      GetTargetDetectedCount(const uint8_t pixel) const;

                uint8_t      GetAmbientPerSpad(const uint8_t pixel) const;

                auto         GetDistanceMatrix(void) const -> const uint16_t (*)[8U] override {
                    return this->mDistanceMatrix;
                }

                uint16_t GetDistanceBufferSize(void) const override {
                    return 128U; // 8x8 matrix = 128 bytes
                }

                LaserModel GetLaserModelName(void) const override {
                    return LaserModel::VL53L5X;
                }

            private:
                static constexpr uint16_t PAGE_SELECT = 0x7FFFU;
                static constexpr uint8_t  MATRIX_SIDE = 8U;
                static constexpr uint8_t  VL53L5CX_RESOLUTION_4X4 = ((uint8_t) 16U);
                static constexpr uint8_t  VL53L5CX_RESOLUTION_8X8 = ((uint8_t) 64U);
                static constexpr uint8_t  VL53L5CX_TARGET_ORDER_CLOSEST = ((uint8_t) 1U);
                static constexpr uint8_t  VL53L5CX_RANGING_MODE_CONTINUOUS = ((uint8_t) 1U);
                static constexpr uint8_t  VL53L5CX_RES_8X8_HZ_10 = ((uint8_t) 10U);

                static constexpr uint8_t  DEFAULT_RESOLUTION = VL53L5CX_RESOLUTION_8X8;
                static constexpr uint8_t  DEFAULT_RANGING_MODE = VL53L5CX_RANGING_MODE_CONTINUOUS;
                static constexpr uint8_t  DEFAULT_FREQUENCY_HZ = VL53L5CX_RES_8X8_HZ_10;
                static constexpr uint8_t  DEFAULT_TARGET_ORDER = VL53L5CX_TARGET_ORDER_CLOSEST;

#define VL53L5CX_START_BH ((uint32_t) 0x0000000DU)
#define VL53L5CX_METADATA_BH ((uint32_t) 0x54B400C0U)
#define VL53L5CX_COMMONDATA_BH ((uint32_t) 0x54C00040U)
#define VL53L5CX_AMBIENT_RATE_BH ((uint32_t) 0x54D00104U)
#define VL53L5CX_SPAD_COUNT_BH ((uint32_t) 0x55D00404U)
#define VL53L5CX_NB_TARGET_DETECTED_BH ((uint32_t) 0xCF7C0401U)
#define VL53L5CX_SIGNAL_RATE_BH ((uint32_t) 0xCFBC0404U)
#define VL53L5CX_RANGE_SIGMA_MM_BH ((uint32_t) 0xD2BC0402U)
#define VL53L5CX_DISTANCE_BH ((uint32_t) 0xD33C0402U)
#define VL53L5CX_REFLECTANCE_BH ((uint32_t) 0xD43C0401U)
#define VL53L5CX_TARGET_STATUS_BH ((uint32_t) 0xD47C0401U)
#define VL53L5CX_MOTION_DETECT_BH ((uint32_t) 0xCC5008C0U)

#define VL53L5CX_METADATA_IDX ((uint16_t) 0x54B4U)
#define VL53L5CX_SPAD_COUNT_IDX ((uint16_t) 0x55D0U)
#define VL53L5CX_AMBIENT_RATE_IDX ((uint16_t) 0x54D0U)
#define VL53L5CX_NB_TARGET_DETECTED_IDX ((uint16_t) 0xCF7CU)
#define VL53L5CX_SIGNAL_RATE_IDX ((uint16_t) 0xCFBCU)
#define VL53L5CX_RANGE_SIGMA_MM_IDX ((uint16_t) 0xD2BCU)
#define VL53L5CX_DISTANCE_IDX ((uint16_t) 0xD33CU)
#define VL53L5CX_REFLECTANCE_EST_PC_IDX ((uint16_t) 0xD43CU)
#define VL53L5CX_TARGET_STATUS_IDX ((uint16_t) 0xD47CU)
#define VL53L5CX_MOTION_DETEC_IDX ((uint16_t) 0xCC50U)

                static constexpr uint16_t VL53L5CX_NVM_DATA_SIZE = ((uint16_t) 492U);
                static constexpr uint16_t VL53L5CX_CONFIGURATION_SIZE = ((uint16_t) 972U);
                static constexpr uint16_t VL53L5CX_OFFSET_BUFFER_SIZE = ((uint16_t) 488U);
                static constexpr uint16_t VL53L5CX_XTALK_BUFFER_SIZE = ((uint16_t) 776U);
                static constexpr uint16_t VL53L5CX_DCI_ZONE_CONFIG = ((uint16_t) 0x5450U);
                static constexpr uint16_t VL53L5CX_DCI_FREQ_HZ = ((uint16_t) 0x5458U);
                static constexpr uint16_t VL53L5CX_DCI_INT_TIME = ((uint16_t) 0x545CU);
                static constexpr uint16_t VL53L5CX_DCI_FW_NB_TARGET = ((uint16_t) 0x5478);
                static constexpr uint16_t VL53L5CX_DCI_RANGING_MODE = ((uint16_t) 0xAD30U);
                static constexpr uint16_t VL53L5CX_DCI_DSS_CONFIG = ((uint16_t) 0xAD38U);
                static constexpr uint16_t VL53L5CX_DCI_TARGET_ORDER = ((uint16_t) 0xAE64U);
                static constexpr uint16_t VL53L5CX_DCI_SHARPENER = ((uint16_t) 0xAED8U);
                static constexpr uint16_t VL53L5CX_DCI_MOTION_DETECTOR_CFG = ((uint16_t) 0xBFACU);
                static constexpr uint16_t VL53L5CX_DCI_SINGLE_RANGE = ((uint16_t) 0xCD5CU);
                static constexpr uint16_t VL53L5CX_DCI_OUTPUT_CONFIG = ((uint16_t) 0xCD60U);
                static constexpr uint16_t VL53L5CX_DCI_OUTPUT_ENABLES = ((uint16_t) 0xCD68U);
                static constexpr uint16_t VL53L5CX_DCI_OUTPUT_LIST = ((uint16_t) 0xCD78U);
                static constexpr uint16_t VL53L5CX_DCI_PIPE_CONTROL = ((uint16_t) 0xCF78U);

                static constexpr uint16_t VL53L5CX_UI_CMD_STATUS = ((uint16_t) 0x2C00U);
                static constexpr uint16_t VL53L5CX_UI_CMD_START = ((uint16_t) 0x2C04U);
                static constexpr uint16_t VL53L5CX_UI_CMD_END = ((uint16_t) 0x2FFFU);
                typedef struct {
                    int32_t  ref_bin_offset;
                    uint32_t detection_threshold;
                    uint32_t extra_noise_sigma;
                    uint32_t null_den_clip_value;
                    uint8_t  mem_update_mode;
                    uint8_t  mem_update_choice;
                    uint8_t  sum_span;
                    uint8_t  feature_length;
                    uint8_t  nb_of_aggregates;
                    uint8_t  nb_of_temporal_accumulations;
                    uint8_t  min_nb_for_global_detection;
                    uint8_t  global_indicator_format_1;
                    uint8_t  global_indicator_format_2;
                    uint8_t  spare_1;
                    uint8_t  spare_2;
                    uint8_t  spare_3;
                    int8_t   map_id[64];
                    uint8_t  indicator_format_1[32];
                    uint8_t  indicator_format_2[32];
                } VL53L5CX_Motion_Configuration;

                Twi::TwiInterface            &mI2c;
                Led::LedInterface            &mLed;
                uint8_t                       mAddress;
                uint16_t                      mDistance;
                uint16_t                      mThreshold;
                uint8_t                       mResolution;
                uint32_t                      mDataReadSize;
                uint16_t                      mDistanceMatrix[8][8];
                uint8_t                       temp_buffer[1024U] = {0U};
                uint8_t                       xtalk_buffer[VL53L5CX_XTALK_BUFFER_SIZE] = {0U};
                uint8_t                       offset_buffer[VL53L5CX_OFFSET_BUFFER_SIZE] = {0U};
                VL53L5CX_Motion_Configuration motion_config;
                uint8_t                       streamcount = 0U;

                typedef struct {
                    /* Ambiant noise in kcps/spads */
#ifndef VL53L5CX_DISABLE_AMBIENT_PER_SPAD
                    uint32_t ambient_per_spad[VL53L5CX_RESOLUTION_8X8];
#endif

                    /* Number of valid target detected for 1 zone */
#ifndef VL53L5CX_DISABLE_NB_TARGET_DETECTED
                    uint8_t nb_target_detected[VL53L5CX_RESOLUTION_8X8];
#endif

                    /* Number of spads enabled for this ranging */
#ifndef VL53L5CX_DISABLE_NB_SPADS_ENABLED
                    uint32_t nb_spads_enabled[VL53L5CX_RESOLUTION_8X8];
#endif

                    /* Signal returned to the sensor in kcps/spads */
#ifndef VL53L5CX_DISABLE_SIGNAL_PER_SPAD
                    uint32_t signal_per_spad[(VL53L5CX_RESOLUTION_8X8 *
                                              VL53L5CX_NB_TARGET_PER_ZONE)];
#endif

                    /* Sigma of the current distance in mm */
#ifndef VL53L5CX_DISABLE_RANGE_SIGMA_MM
                    uint16_t
                            range_sigma_mm[(VL53L5CX_RESOLUTION_8X8 * VL53L5CX_NB_TARGET_PER_ZONE)];
#endif

                    /* Measured distance in mm */
#ifndef VL53L5CX_DISABLE_DISTANCE_MM
                    int16_t distance_mm[(VL53L5CX_RESOLUTION_8X8 * VL53L5CX_NB_TARGET_PER_ZONE)];
#endif

                    /* Estimated reflectance in percent */
#ifndef VL53L5CX_DISABLE_REFLECTANCE_PERCENT
                    uint8_t reflectance[(VL53L5CX_RESOLUTION_8X8 * VL53L5CX_NB_TARGET_PER_ZONE)];
#endif

                    /* Status indicating the measurement validity (5 & 9 means ranging OK)*/
#ifndef VL53L5CX_DISABLE_TARGET_STATUS
                    uint8_t target_status[(VL53L5CX_RESOLUTION_8X8 * VL53L5CX_NB_TARGET_PER_ZONE)];
#endif

                    /* Motion detector results */
#ifndef VL53L5CX_DISABLE_MOTION_INDICATOR
                    struct {
                        uint32_t global_indicator_1;
                        uint32_t global_indicator_2;
                        uint8_t  status;
                        uint8_t  nb_of_detected_aggregates;
                        uint8_t  nb_of_aggregates;
                        uint8_t  spare;
                        uint32_t motion[32];
                    } motion_indicator;
#endif

                } VL53L5CX_ResultsData;

                VL53L5CX_ResultsData mResults = {};


                union Block_header {
                    uint32_t bytes;
                    struct {
                        uint32_t type : 4;
                        uint32_t size : 12;
                        uint32_t idx : 16;
                    };
                };

                bool        IsAlive(void) const;

                bool        Start(void);

                bool        InitSensor(void);

                static void SwapBuffer(uint8_t *buffer, uint16_t size);

                uint8_t     OffsetData(uint8_t resolution);

                uint8_t     SendXtalkData(uint8_t resolution);

                uint8_t     DciWriteData(const uint8_t *data, uint32_t index, uint16_t data_size);

                uint8_t     DciReadData(uint8_t *data, uint32_t index, uint16_t data_size);

                uint8_t     DciReplaceData(uint8_t       *data,
                                           uint32_t       index,
                                           uint16_t       data_size,
                                           const uint8_t *new_data,
                                           uint16_t       new_data_size,
                                           uint16_t       new_data_pos);

                void        ParseFrame(void);

                void        ConvertResults(void);

                void        UpdateDistanceMatrix(void);

                bool        Poll(const uint8_t  size,
                                 const uint8_t  pos,
                                 const uint16_t reg,
                                 const uint8_t  mask,
                                 const uint8_t  expected_value) const;

                uint8_t     MotionIndicatorInit(uint8_t resolution);
                uint8_t     MotionSetResolution(uint8_t resolution);
                uint8_t     MotionGetResolution(uint8_t *p_resolution);
                uint8_t     MotionSetDistance(uint16_t distance_min_mm, uint16_t distance_max_mm);
                uint8_t     MotionSetRangingFrequency(uint8_t frequency_hz);
                uint8_t     StartRanging(void);
            };
        } // namespace Laser
    } // namespace Proximity
} // namespace Component
