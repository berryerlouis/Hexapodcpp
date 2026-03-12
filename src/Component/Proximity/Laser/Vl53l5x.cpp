#include "Vl53l5x.h"
#include "../../Driver/Timer/Tick.h"
#include "string.h"

namespace Component
{
    namespace Proximity
    {
        namespace Laser
        {
            Vl53l5x::Vl53l5x(Twi::TwiInterface &i2c, Led::LedInterface &led, const uint8_t address)
                : mI2c(i2c)
                , mLed(led)
                , mAddress(address)
                , mDistance(0U)
                , mThreshold(DISTANCE_THRESHOLD) {
                // Initialize distance matrix to zeros
                for (uint8_t row = 0U; row < 8U; ++row) {
                    for (uint8_t col = 0U; col < 8U; ++col) {
                        this->mDistanceMatrix[row][col] = 0U;
                    }
                }
            }

            Core::Status Vl53l5x::Initialize() {
                this->mLed.Initialize();
                if (this->IsAlive()) {
                    if (!this->Start()) {
                        LOG_COMPONENT_ERROR(
                                "Laser", "VL53L5X address 0x%02X failed to start.", this->mAddress);
                        return Core::Status::CORE_ERROR;
                    }
                    this->StartRanging();
                    /*this->MotionIndicatorInit(VL53L5CX_RESOLUTION_4X4);
                    this->MotionSetDistance(400U, 1000U);
                    this->MotionSetRangingFrequency(2U);*/
                    LOG_COMPONENT_DEBUG(
                            "Laser", "VL53L5X address 0x%02X initialized.", this->mAddress);
                } else {
                    LOG_COMPONENT_ERROR("Laser",
                                        "VL53L5X address 0x%02X I2C communication failed.",
                                        this->mAddress);
                    return Core::Status::CORE_ERROR;
                }
                return Core::Status::CORE_OK;
            }

            void Vl53l5x::Update(const uint64_t currentTime) {
                (void) currentTime;
                if (this->IsDataReady()) {
                    this->mDistance = this->GetDistance();

                    const bool detection =
                            this->mDistance != 0U && this->mDistance <= this->mThreshold;
                    if (detection) {
                        this->mLed.On();
                        this->Notify({VLX, this->mDistance, this->mDistanceMatrix});
                    } else {
                        this->mLed.Off();
                    }
                }
            }

            uint16_t Vl53l5x::GetDistance() {
                return this->mDistance;
            }

            Core::Status Vl53l5x::SetThreshold(const uint16_t threshold) {
                this->mThreshold = threshold;
                return Core::Status::CORE_OK;
            }

            uint16_t Vl53l5x::GetThreshold() {
                return this->mThreshold;
            }

            bool Vl53l5x::Poll(const uint8_t  size,
                               const uint8_t  pos,
                               const uint16_t reg,
                               const uint8_t  mask,
                               const uint8_t  expected_value) const {
                uint8_t status = 0U;
                uint8_t timeout = 0;
                uint8_t buffer[4U] = {0U, 0U, 0U, 0U};

                do {
                    status |= this->mI2c.Read16Registers(this->mAddress, reg, buffer, size);
                    Driver::Timer::Tick::GetInstance().DelayMs(10U);

                    if (timeout >= (uint8_t) 200U) { /* 2s timeout */
                        status |= 1U; // timeout
                        break;
                    } else if ((size >= (uint8_t) 4U) && (buffer[2U] >= (uint8_t) 0x7f)) {
                        status |= 1U; // MCU error
                        break;
                    } else {
                        timeout++;
                    }
                } while ((buffer[pos] & mask) != expected_value);

                return status;
            }


            bool Vl53l5x::IsAlive(void) const {
                uint8_t modelId = 0U;
                uint8_t revisionId = 0U;
                uint8_t status = this->mI2c.Write16Register(this->mAddress, PAGE_SELECT, 0U);
                status |= this->mI2c.Read16Register(this->mAddress, 0, modelId);
                status |= this->mI2c.Read16Register(this->mAddress, 1, revisionId);
                status |= this->mI2c.Write16Register(this->mAddress, PAGE_SELECT, 1U);
                return (status == 1U) && (modelId == 0xF0U) && (revisionId == 0x02U);
            }

            void Vl53l5x::SwapBuffer(uint8_t *buffer, uint16_t size) {
                uint32_t i, tmp;

                /* Example of possible implementation using <string.h> */
                for (i = 0; i < size; i = i + 4) {
                    tmp = (buffer[i] << 24) | (buffer[i + 1] << 16) | (buffer[i + 2] << 8) |
                          (buffer[i + 3]);

                    memcpy(&(buffer[i]), &tmp, 4);
                }
            }


            uint8_t Vl53l5x::OffsetData(uint8_t resolution) {
                uint8_t  status = 0U;
                uint32_t signal_grid[64];
                int16_t  range_grid[64];
                uint8_t  dss_4x4[] = {0x0F, 0x04, 0x04, 0x00, 0x08, 0x10, 0x10, 0x07};
                uint8_t  footer[] = {0x00, 0x00, 0x00, 0x0F, 0x03, 0x01, 0x01, 0xE4};
                int8_t   i, j;
                uint16_t k;

                (void) memcpy(this->temp_buffer, this->offset_buffer, VL53L5CX_OFFSET_BUFFER_SIZE);

                /* Data extrapolation is required for 4X4 offset */
                if (resolution == (uint8_t) VL53L5CX_RESOLUTION_4X4) {
                    (void) memcpy(&(this->temp_buffer[0x10]), dss_4x4, sizeof(dss_4x4));
                    SwapBuffer(this->temp_buffer, VL53L5CX_OFFSET_BUFFER_SIZE);
                    (void) memcpy(signal_grid, &(this->temp_buffer[0x3C]), sizeof(signal_grid));
                    (void) memcpy(range_grid, &(this->temp_buffer[0x140]), sizeof(range_grid));

                    for (j = 0; j < (int8_t) 4; j++) {
                        for (i = 0; i < (int8_t) 4; i++) {
                            signal_grid[i + (4 * j)] =
                                    (signal_grid[(2 * i) + (16 * j) + (int8_t) 0] +
                                     signal_grid[(2 * i) + (16 * j) + (int8_t) 1] +
                                     signal_grid[(2 * i) + (16 * j) + (int8_t) 8] +
                                     signal_grid[(2 * i) + (16 * j) + (int8_t) 9]) /
                                    (uint32_t) 4;
                            range_grid[i + (4 * j)] = (range_grid[(2 * i) + (16 * j)] +
                                                       range_grid[(2 * i) + (16 * j) + 1] +
                                                       range_grid[(2 * i) + (16 * j) + 8] +
                                                       range_grid[(2 * i) + (16 * j) + 9]) /
                                                      (int16_t) 4;
                        }
                    }
                    (void) memset(&range_grid[0x10], 0, (uint16_t) 96);
                    (void) memset(&signal_grid[0x10], 0, (uint16_t) 192);
                    (void) memcpy(&(this->temp_buffer[0x3C]), signal_grid, sizeof(signal_grid));
                    (void) memcpy(&(this->temp_buffer[0x140]), range_grid, sizeof(range_grid));
                    this->SwapBuffer(this->temp_buffer, VL53L5CX_OFFSET_BUFFER_SIZE);
                }
                for (k = 0; k < (VL53L5CX_OFFSET_BUFFER_SIZE - (uint16_t) 4); k++) {
                    this->temp_buffer[k] = this->temp_buffer[k + (uint16_t) 8];
                }

                (void) memcpy(&(this->temp_buffer[0x1E0]), footer, 8);
                status |= this->mI2c.Write16Registers(
                        this->mAddress, 0x2e18, this->temp_buffer, VL53L5CX_OFFSET_BUFFER_SIZE);
                status |= this->Poll(4, 1, VL53L5CX_UI_CMD_STATUS, 0xff, 0x03);

                return status;
            }

            uint8_t Vl53l5x::SendXtalkData(uint8_t resolution) {
                uint8_t  status = 0U;
                uint8_t  res4x4[] = {0x0F, 0x04, 0x04, 0x17, 0x08, 0x10, 0x10, 0x07};
                uint8_t  dss_4x4[] = {0x00, 0x78, 0x00, 0x08, 0x00, 0x00, 0x00, 0x08};
                uint8_t  profile_4x4[] = {0xA0, 0xFC, 0x01, 0x00};
                uint32_t signal_grid[64];
                int8_t   i, j;

                (void) memcpy(
                        this->temp_buffer, &(this->xtalk_buffer[0]), VL53L5CX_XTALK_BUFFER_SIZE);

                /* Data extrapolation is required for 4X4 Xtalk */
                if (resolution == (uint8_t) VL53L5CX_RESOLUTION_4X4) {
                    (void) memcpy(&(this->temp_buffer[0x8]), res4x4, sizeof(res4x4));
                    (void) memcpy(&(this->temp_buffer[0x020]), dss_4x4, sizeof(dss_4x4));

                    SwapBuffer(this->temp_buffer, VL53L5CX_XTALK_BUFFER_SIZE);
                    (void) memcpy(signal_grid, &(this->temp_buffer[0x34]), sizeof(signal_grid));

                    for (j = 0; j < (int8_t) 4; j++) {
                        for (i = 0; i < (int8_t) 4; i++) {
                            signal_grid[i + (4 * j)] = (signal_grid[(2 * i) + (16 * j) + 0] +
                                                        signal_grid[(2 * i) + (16 * j) + 1] +
                                                        signal_grid[(2 * i) + (16 * j) + 8] +
                                                        signal_grid[(2 * i) + (16 * j) + 9]) /
                                                       (uint32_t) 4;
                        }
                    }
                    (void) memset(&signal_grid[0x10], 0, (uint32_t) 192);
                    (void) memcpy(&(this->temp_buffer[0x34]), signal_grid, sizeof(signal_grid));
                    SwapBuffer(this->temp_buffer, VL53L5CX_XTALK_BUFFER_SIZE);
                    (void) memcpy(&(this->temp_buffer[0x134]), profile_4x4, sizeof(profile_4x4));
                    (void) memset(&(this->temp_buffer[0x078]), 0, (uint32_t) 4 * sizeof(uint8_t));
                }

                status |= this->mI2c.Write16Registers(
                        this->mAddress, 0x2cf8, this->temp_buffer, VL53L5CX_XTALK_BUFFER_SIZE);
                status |= this->Poll(4, 1, VL53L5CX_UI_CMD_STATUS, 0xff, 0x03);

                return status;
            }


            bool Vl53l5x::Start(void) {
                uint8_t  tmp, status = 0U;
                uint8_t  pipe_ctrl[] = {VL53L5CX_NB_TARGET_PER_ZONE, 0x00, 0x01, 0x00};
                uint32_t single_range = 0x01;
                uint8_t  temp_buffer[VL53L5CX_NVM_DATA_SIZE] = {0U};
                uint8_t  offset_buffer[VL53L5CX_OFFSET_BUFFER_SIZE] = {0U};


                /* SW reboot sequence */
                status |= this->mI2c.Write16Register(this->mAddress, PAGE_SELECT, 0x00);
                status |= this->mI2c.Write16Register(this->mAddress, 0x0009, 0x04);
                status |= this->mI2c.Write16Register(this->mAddress, 0x000F, 0x40);
                status |= this->mI2c.Write16Register(this->mAddress, 0x000A, 0x03);
                status |= this->mI2c.Read16Register(this->mAddress, PAGE_SELECT, tmp);
                status |= this->mI2c.Write16Register(this->mAddress, 0x000C, 0x01);

                status |= this->mI2c.Write16Register(this->mAddress, 0x0101, 0x00);
                status |= this->mI2c.Write16Register(this->mAddress, 0x0102, 0x00);
                status |= this->mI2c.Write16Register(this->mAddress, 0x010A, 0x01);
                status |= this->mI2c.Write16Register(this->mAddress, 0x4002, 0x01);
                status |= this->mI2c.Write16Register(this->mAddress, 0x4002, 0x00);
                status |= this->mI2c.Write16Register(this->mAddress, 0x010A, 0x03);
                status |= this->mI2c.Write16Register(this->mAddress, 0x0103, 0x01);
                status |= this->mI2c.Write16Register(this->mAddress, 0x000C, 0x00);
                status |= this->mI2c.Write16Register(this->mAddress, 0x000F, 0x43);

                Driver::Timer::Tick::GetInstance().DelayMs(1U);

                status |= this->mI2c.Write16Register(this->mAddress, 0x000F, 0x40);
                status |= this->mI2c.Write16Register(this->mAddress, 0x000A, 0x01);
                Driver::Timer::Tick::GetInstance().DelayMs(100);

                /* Wait for sensor booted (several ms required to get sensor ready ) */
                status |= this->mI2c.Write16Register(this->mAddress, PAGE_SELECT, 0x00);
                status |= this->Poll(1, 0, 0x06, 0xff, 1);

                status |= this->mI2c.Write16Register(this->mAddress, 0x000E, 0x01);
                status |= this->mI2c.Write16Register(this->mAddress, PAGE_SELECT, 0x02);

                /* Enable FW access */
                status |= this->mI2c.Write16Register(this->mAddress, 0x03, 0x0D);
                status |= this->mI2c.Write16Register(this->mAddress, PAGE_SELECT, 0x01);
                status |= this->Poll(1, 0, 0x21, 0x10, 0x10);
                status |= this->mI2c.Write16Register(this->mAddress, PAGE_SELECT, 0x00);

                /* Enable host access to GO1 */
                status |= this->mI2c.Write16Register(this->mAddress, 0x0C, 0x01);

                /* Power ON status */
                status |= this->mI2c.Write16Register(this->mAddress, PAGE_SELECT, 0x00);
                status |= this->mI2c.Write16Register(this->mAddress, 0x101, 0x00);
                status |= this->mI2c.Write16Register(this->mAddress, 0x102, 0x00);
                status |= this->mI2c.Write16Register(this->mAddress, 0x010A, 0x01);
                status |= this->mI2c.Write16Register(this->mAddress, 0x4002, 0x01);
                status |= this->mI2c.Write16Register(this->mAddress, 0x4002, 0x00);
                status |= this->mI2c.Write16Register(this->mAddress, 0x010A, 0x03);
                status |= this->mI2c.Write16Register(this->mAddress, 0x103, 0x01);
                status |= this->mI2c.Write16Register(this->mAddress, 0x400F, 0x00);
                status |= this->mI2c.Write16Register(this->mAddress, 0x21A, 0x43);
                status |= this->mI2c.Write16Register(this->mAddress, 0x21A, 0x03);
                status |= this->mI2c.Write16Register(this->mAddress, 0x21A, 0x01);
                status |= this->mI2c.Write16Register(this->mAddress, 0x21A, 0x00);
                status |= this->mI2c.Write16Register(this->mAddress, 0x219, 0x00);
                status |= this->mI2c.Write16Register(this->mAddress, 0x21B, 0x00);

                /* Wake up MCU */
                status |= this->mI2c.Write16Register(this->mAddress, PAGE_SELECT, 0x00);
                status |= this->mI2c.Write16Register(this->mAddress, 0x0C, 0x00);
                status |= this->mI2c.Write16Register(this->mAddress, PAGE_SELECT, 0x01);
                status |= this->mI2c.Write16Register(this->mAddress, 0x20, 0x07);
                status |= this->mI2c.Write16Register(this->mAddress, 0x20, 0x06);

                /* Download FW into VL53L5 */
                status |= this->mI2c.Write16Register(this->mAddress, PAGE_SELECT, 0x09);

                status |= this->mI2c.Write16Registers(
                        this->mAddress, 0, (uint8_t *) &VL53L5CX_FIRMWARE[0], 0x8000);
                status |= this->mI2c.Write16Register(this->mAddress, PAGE_SELECT, 0x0a);
                status |= this->mI2c.Write16Registers(
                        this->mAddress, 0, (uint8_t *) &VL53L5CX_FIRMWARE[0x8000], 0x8000);
                status |= this->mI2c.Write16Register(this->mAddress, PAGE_SELECT, 0x0b);
                status |= this->mI2c.Write16Registers(
                        this->mAddress, 0, (uint8_t *) &VL53L5CX_FIRMWARE[0x10000], 0x5000);
                status |= this->mI2c.Write16Register(this->mAddress, PAGE_SELECT, 0x01);

                /* Check if FW correctly downloaded */
                status |= this->mI2c.Write16Register(this->mAddress, PAGE_SELECT, 0x02);
                status |= this->mI2c.Write16Register(this->mAddress, 0x03, 0x0D);
                status |= this->mI2c.Write16Register(this->mAddress, PAGE_SELECT, 0x01);
                status |= this->Poll(1, 0, 0x21, 0x10, 0x10);
                status |= this->mI2c.Write16Register(this->mAddress, PAGE_SELECT, 0x00);
                status |= this->mI2c.Write16Register(this->mAddress, 0x0C, 0x01);

                /* Reset MCU and wait boot */
                status |= this->mI2c.Write16Register(this->mAddress, PAGE_SELECT, 0x00);
                status |= this->mI2c.Write16Register(this->mAddress, 0x114, 0x00);
                status |= this->mI2c.Write16Register(this->mAddress, 0x115, 0x00);
                status |= this->mI2c.Write16Register(this->mAddress, 0x116, 0x42);
                status |= this->mI2c.Write16Register(this->mAddress, 0x117, 0x00);
                status |= this->mI2c.Write16Register(this->mAddress, 0x0B, 0x00);
                status |= this->mI2c.Write16Register(this->mAddress, 0x0C, 0x00);
                status |= this->mI2c.Write16Register(this->mAddress, 0x0B, 0x01);
                status |= this->Poll(1, 0, 0x06, 0xff, 0x01);
                status |= this->mI2c.Write16Register(this->mAddress, PAGE_SELECT, 0x02);

                /* Get offset NVM data and store them into the offset buffer */
                status |= this->mI2c.Write16Registers(this->mAddress,
                                                      0x2fd8,
                                                      (uint8_t *) VL53L5CX_GET_NVM_CMD,
                                                      sizeof(VL53L5CX_GET_NVM_CMD));
                status |= this->Poll(4, 0, VL53L5CX_UI_CMD_STATUS, 0xff, 2);
                status |= this->mI2c.Read16Registers(
                        this->mAddress, VL53L5CX_UI_CMD_START, temp_buffer, VL53L5CX_NVM_DATA_SIZE);
                (void) memcpy(offset_buffer, temp_buffer, VL53L5CX_OFFSET_BUFFER_SIZE);
                status |= this->OffsetData(VL53L5CX_RESOLUTION_4X4);

                /* Set default Xtalk shape. Send Xtalk to sensor */
                (void) memcpy(this->xtalk_buffer,
                              (uint8_t *) VL53L5CX_DEFAULT_XTALK,
                              VL53L5CX_XTALK_BUFFER_SIZE);
                status |= this->SendXtalkData(VL53L5CX_RESOLUTION_4X4);

                /* Send default configuration to VL53L5CX firmware */
                uint8_t default_configuration[sizeof(VL53L5CX_DEFAULT_CONFIGURATION)] = {0U};
                (void) memcpy(default_configuration,
                              (uint8_t *) VL53L5CX_DEFAULT_CONFIGURATION,
                              sizeof(VL53L5CX_DEFAULT_CONFIGURATION));

                status |= this->mI2c.Write16Registers(this->mAddress,
                                                      0x2c34,
                                                      default_configuration,
                                                      sizeof(VL53L5CX_DEFAULT_CONFIGURATION));
                status |= this->Poll(4, 1, VL53L5CX_UI_CMD_STATUS, 0xff, 0x03);

                status |= this->DciWriteData((uint8_t *) &pipe_ctrl,
                                             VL53L5CX_DCI_PIPE_CONTROL,
                                             (uint16_t) sizeof(pipe_ctrl));
#if VL53L5CX_NB_TARGET_PER_ZONE != 1
                tmp = VL53L5CX_NB_TARGET_PER_ZONE;
                status |= this->DciReplaceData(this->temp_buffer,
                                               VL53L5CX_DCI_FW_NB_TARGET,
                                               16,
                                               (uint8_t *) &tmp,
                                               1,
                                               0x0C);
#endif

                status |= this->DciWriteData((uint8_t *) &single_range,
                                             VL53L5CX_DCI_SINGLE_RANGE,
                                             (uint16_t) sizeof(single_range));
                return status;
            }


            uint8_t Vl53l5x::DciWriteData(uint8_t *data, uint32_t index, uint16_t data_size) {
                uint8_t  status = 0U;
                int16_t  i;

                uint8_t  headers[] = {0x00, 0x00, 0x00, 0x00};
                uint8_t  footer[] = {0x00,
                                     0x00,
                                     0x00,
                                     0x0f,
                                     0x05,
                                     0x01,
                                     (uint8_t) ((data_size + (uint16_t) 8) >> 8),
                                     (uint8_t) ((data_size + (uint16_t) 8) & (uint8_t) 0xFF)};

                uint16_t reg =
                        (uint16_t) VL53L5CX_UI_CMD_END - (data_size + (uint16_t) 12) + (uint16_t) 1;

                /* Check if cmd buffer is large enough */
                if ((data_size + (uint16_t) 12) > (uint16_t) 1024) {
                    status |= 1U;
                } else {
                    headers[0] = (uint8_t) (index >> 8);
                    headers[1] = (uint8_t) (index & (uint32_t) 0xff);
                    headers[2] = (uint8_t) (((data_size & (uint16_t) 0xff0) >> 4));
                    headers[3] = (uint8_t) ((data_size & (uint16_t) 0xf) << 4);

                    /* Copy data from structure to FW format (+4 bytes to add header) */
                    SwapBuffer(data, data_size);
                    for (i = (int16_t) data_size - (int16_t) 1; i >= 0; i--) {
                        this->temp_buffer[i + 4] = data[i];
                    }

                    /* Add headers and footer */
                    (void) memcpy(&this->temp_buffer[0], headers, sizeof(headers));
                    (void) memcpy(
                            &this->temp_buffer[data_size + (uint16_t) 4], footer, sizeof(footer));

                    /* Send data to FW */
                    status |= this->mI2c.Write16Registers(
                            this->mAddress,
                            reg,
                            this->temp_buffer,
                            (uint32_t) ((uint32_t) data_size + (uint32_t) 12));
                    status |= this->Poll(4, 1, VL53L5CX_UI_CMD_STATUS, 0xff, 0x03);

                    SwapBuffer(data, data_size);
                }

                return status;
            }

            uint8_t Vl53l5x::DciReadData(uint8_t *data, uint32_t index, uint16_t data_size) {
                int16_t  i;
                uint8_t  status = 0U;
                uint32_t rd_size = (uint32_t) data_size + (uint32_t) 12;
                uint8_t  cmd[] = {
                        0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x0f, 0x00, 0x02, 0x00, 0x08};

                /* Check if tmp buffer is large enough */
                if ((data_size + (uint16_t) 12) > (uint16_t) 1024) {
                    status |= 1U;
                } else {
                    cmd[0] = (uint8_t) (index >> 8);
                    cmd[1] = (uint8_t) (index & (uint32_t) 0xff);
                    cmd[2] = (uint8_t) ((data_size & (uint16_t) 0xff0) >> 4);
                    cmd[3] = (uint8_t) ((data_size & (uint16_t) 0xf) << 4);

                    /* Request data reading from FW */
                    status |= this->mI2c.Write16Registers(this->mAddress,
                                                          (VL53L5CX_UI_CMD_END - (uint16_t) 11),
                                                          cmd,
                                                          sizeof(cmd));
                    status |= this->Poll(4, 1, VL53L5CX_UI_CMD_STATUS, 0xff, 0x03);

                    /* Read new data sent (4 bytes header + data_size + 8 bytes footer) */
                    status |= this->mI2c.Read16Registers(
                            this->mAddress, VL53L5CX_UI_CMD_START, this->temp_buffer, rd_size);
                    SwapBuffer(this->temp_buffer, data_size + (uint16_t) 12);

                    /* Copy data from FW into input structure (-4 bytes to remove header) */
                    for (i = 0; i < (int16_t) data_size; i++) {
                        data[i] = this->temp_buffer[i + 4];
                    }
                }

                return status;
            }

            uint8_t Vl53l5x::DciReplaceData(uint8_t *data,
                                            uint32_t index,
                                            uint16_t data_size,
                                            uint8_t *new_data,
                                            uint16_t new_data_size,
                                            uint16_t new_data_pos) {
                uint8_t status = 0U;

                status |= this->DciReadData(data, index, data_size);
                (void) memcpy(&(data[new_data_pos]), new_data, new_data_size);
                status |= this->DciWriteData(data, index, data_size);

                return status;
            }

            uint8_t Vl53l5x::StartRanging(void) {

                uint8_t             resolution, status = 0U;
                uint32_t            i;
                uint32_t            header_config[2] = {0, 0};

                union Block_header *bh_ptr;
                uint8_t             cmd[] = {0x00, 0x03, 0x00, 0x00};

                status |= this->MotionGetResolution(&resolution);
                uint8_t data_read_size = 0;
                this->streamcount = 255;

                /* Enable mandatory output (meta and common data) */
                uint32_t output_bh_enable[] = {0x00000007U, 0x00000000U, 0x00000000U, 0xC0000000U};

                /* Send addresses of possible output */
                uint32_t output[] = {VL53L5CX_START_BH,
                                     VL53L5CX_METADATA_BH,
                                     VL53L5CX_COMMONDATA_BH,
                                     VL53L5CX_AMBIENT_RATE_BH,
                                     VL53L5CX_SPAD_COUNT_BH,
                                     VL53L5CX_NB_TARGET_DETECTED_BH,
                                     VL53L5CX_SIGNAL_RATE_BH,
                                     VL53L5CX_RANGE_SIGMA_MM_BH,
                                     VL53L5CX_DISTANCE_BH,
                                     VL53L5CX_REFLECTANCE_BH,
                                     VL53L5CX_TARGET_STATUS_BH,
                                     VL53L5CX_MOTION_DETECT_BH};

#define VL53L5CX_DISABLE_AMBIENT_PER_SPAD
#define VL53L5CX_DISABLE_NB_SPADS_ENABLED
// #define VL53L5CX_DISABLE_NB_TARGET_DETECTED
#define VL53L5CX_DISABLE_SIGNAL_PER_SPAD
#define VL53L5CX_DISABLE_RANGE_SIGMA_MM
// #define VL53L5CX_DISABLE_DISTANCE_MM
#define VL53L5CX_DISABLE_REFLECTANCE_PERCENT
// #define VL53L5CX_DISABLE_TARGET_STATUS
#define VL53L5CX_DISABLE_MOTION_INDICATOR
#ifndef VL53L5CX_DISABLE_AMBIENT_PER_SPAD
                output_bh_enable[0] += (uint32_t) 8;
#endif
#ifndef VL53L5CX_DISABLE_NB_SPADS_ENABLED
                output_bh_enable[0] += (uint32_t) 16;
#endif
#ifndef VL53L5CX_DISABLE_NB_TARGET_DETECTED
                output_bh_enable[0] += (uint32_t) 32;
#endif
#ifndef VL53L5CX_DISABLE_SIGNAL_PER_SPAD
                output_bh_enable[0] += (uint32_t) 64;
#endif
#ifndef VL53L5CX_DISABLE_RANGE_SIGMA_MM
                output_bh_enable[0] += (uint32_t) 128;
#endif
#ifndef VL53L5CX_DISABLE_DISTANCE_MM
                output_bh_enable[0] += (uint32_t) 256;
#endif
#ifndef VL53L5CX_DISABLE_REFLECTANCE_PERCENT
                output_bh_enable[0] += (uint32_t) 512;
#endif
#ifndef VL53L5CX_DISABLE_TARGET_STATUS
                output_bh_enable[0] += (uint32_t) 1024;
#endif
#ifndef VL53L5CX_DISABLE_MOTION_INDICATOR
                output_bh_enable[0] += (uint32_t) 2048;
#endif

                /* Update data size */
                for (i = 0; i < (uint32_t) (sizeof(output) / sizeof(uint32_t)); i++) {
                    if ((output[i] == (uint8_t) 0) ||
                        ((output_bh_enable[i / (uint32_t) 32] &
                          ((uint32_t) 1 << (i % (uint32_t) 32))) == (uint32_t) 0)) {
                        continue;
                    }

                    bh_ptr = (union Block_header *) &(output[i]);
                    if (((uint8_t) bh_ptr->type >= (uint8_t) 0x1) &&
                        ((uint8_t) bh_ptr->type < (uint8_t) 0x0d)) {
                        if ((bh_ptr->idx >= (uint16_t) 0x54d0) &&
                            (bh_ptr->idx < (uint16_t) (0x54d0 + 960))) {
                            bh_ptr->size = resolution;
                        } else {
                            bh_ptr->size =
                                    (uint8_t) (resolution * (uint8_t) VL53L5CX_NB_TARGET_PER_ZONE);
                        }
                        data_read_size += bh_ptr->type * bh_ptr->size;
                    } else {
                        data_read_size += bh_ptr->size;
                    }
                    data_read_size += (uint32_t) 4;
                }
                data_read_size += (uint32_t) 20;

                status |= this->DciWriteData(
                        (uint8_t *) &(output), VL53L5CX_DCI_OUTPUT_LIST, (uint16_t) sizeof(output));

                header_config[0] = data_read_size;
                header_config[1] = i + (uint32_t) 1;

                status |= this->DciWriteData((uint8_t *) &(header_config),
                                             VL53L5CX_DCI_OUTPUT_CONFIG,
                                             (uint16_t) sizeof(header_config));

                status |= this->DciWriteData((uint8_t *) &(output_bh_enable),
                                             VL53L5CX_DCI_OUTPUT_ENABLES,
                                             (uint16_t) sizeof(output_bh_enable));

                /* Start xshut bypass (interrupt mode) */
                status |= this->mI2c.Write16Register(this->mAddress, 0x7fff, 0x00);
                status |= this->mI2c.Write16Register(this->mAddress, 0x09, 0x05);
                status |= this->mI2c.Write16Register(this->mAddress, 0x7fff, 0x02);

                /* Start ranging session */
                status |= this->mI2c.Write16Registers(this->mAddress,
                                                      VL53L5CX_UI_CMD_END - (uint16_t) (4 - 1),
                                                      (uint8_t *) cmd,
                                                      sizeof(cmd));
                status |= this->Poll(4, 1, VL53L5CX_UI_CMD_STATUS, 0xff, 0x03);

                return status;
            }

            bool Vl53l5x::IsDataReady(void) {
                this->mI2c.Read16Registers(this->mAddress, 0x0, this->temp_buffer, 4);

                if ((this->temp_buffer[0] != this->streamcount) &&
                    (this->temp_buffer[0] != (uint8_t) 255) &&
                    (this->temp_buffer[1] == (uint8_t) 0x5) &&
                    ((this->temp_buffer[2] & (uint8_t) 0x5) == (uint8_t) 0x5) &&
                    ((this->temp_buffer[3] & (uint8_t) 0x10) == (uint8_t) 0x10)) {
                    this->streamcount = this->temp_buffer[0];
                    return true;
                }
                return false;
            }


            uint8_t Vl53l5x::MotionIndicatorInit(uint8_t resolution) {
                uint8_t status = 0U;

                (void) memset(&motion_config, 0, sizeof(VL53L5CX_Motion_Configuration));

                motion_config.ref_bin_offset = 13633;
                motion_config.detection_threshold = 2883584;
                motion_config.extra_noise_sigma = 0;
                motion_config.null_den_clip_value = 0;
                motion_config.mem_update_mode = 6;
                motion_config.mem_update_choice = 2;
                motion_config.sum_span = 4;
                motion_config.feature_length = 9;
                motion_config.nb_of_aggregates = 16;
                motion_config.nb_of_temporal_accumulations = 16;
                motion_config.min_nb_for_global_detection = 1;
                motion_config.global_indicator_format_1 = 8;
                motion_config.global_indicator_format_2 = 0;
                motion_config.spare_1 = 0;
                motion_config.spare_2 = 0;
                motion_config.spare_3 = 0;

                status |= this->MotionSetResolution(resolution);

                return status;
            }
            uint8_t Vl53l5x::MotionGetResolution(uint8_t *p_resolution) {
                uint8_t status = 0U;

                status |= this->DciReadData(this->temp_buffer, VL53L5CX_DCI_ZONE_CONFIG, 8);
                *p_resolution = this->temp_buffer[0x00] * this->temp_buffer[0x01];

                return status;
            }

            uint8_t Vl53l5x::MotionSetResolution(uint8_t resolution) {
                uint8_t i, status = 0U;

                switch (resolution) {
                    case VL53L5CX_RESOLUTION_4X4:
                        for (i = 0; i < (uint8_t) VL53L5CX_RESOLUTION_4X4; i++) {
                            motion_config.map_id[i] = (int8_t) i;
                        }
                        (void) memset(motion_config.map_id + 16, -1, 48);
                        break;

                    case VL53L5CX_RESOLUTION_8X8:
                        for (i = 0; i < (uint8_t) VL53L5CX_RESOLUTION_8X8; i++) {
                            motion_config.map_id[i] =
                                    (int8_t) ((((int8_t) i % 8) / 2) + (4 * ((int8_t) i / 16)));
                        }
                        break;

                    default:
                        status |= 1U;
                        break;
                }

                if (status == (uint8_t) 0) {
                    status |= this->DciWriteData((uint8_t *) (&motion_config),
                                                 VL53L5CX_DCI_MOTION_DETECTOR_CFG,
                                                 (uint16_t) sizeof(motion_config));
                }

                return status;
            }
            uint8_t Vl53l5x::MotionSetDistance(uint16_t distance_min_mm, uint16_t distance_max_mm) {
                uint8_t status = 0U;
                float   tmp;

                if (((distance_max_mm - distance_min_mm) > (uint16_t) 1500) ||
                    (distance_min_mm < (uint16_t) 400) || (distance_max_mm > (uint16_t) 4000)) {
                    status |= 1U;
                } else {
                    tmp = (float) ((((float) distance_min_mm / (float) 37.5348) - (float) 4.0) *
                                   (float) 2048.5);
                    motion_config.ref_bin_offset = (int32_t) tmp;

                    tmp = (float) ((((((float) distance_max_mm - (float) distance_min_mm) /
                                      (float) 10.0) +
                                     (float) 30.02784) /
                                    ((float) 15.01392)) +
                                   (float) 0.5);
                    motion_config.feature_length = (uint8_t) tmp;

                    status |= this->DciWriteData((uint8_t *) (&motion_config),
                                                 VL53L5CX_DCI_MOTION_DETECTOR_CFG,
                                                 (uint16_t) sizeof(motion_config));
                }

                return status;
            }

            uint8_t Vl53l5x::MotionSetRangingFrequency(uint8_t frequency_hz) {
                uint8_t status = 0U;

                status |= this->DciReplaceData(this->temp_buffer,
                                               VL53L5CX_DCI_FREQ_HZ,
                                               4,
                                               (uint8_t *) &frequency_hz,
                                               1,
                                               0x01);

                return status;
            }

        } // namespace Laser
    } // namespace Proximity
} // namespace Component
