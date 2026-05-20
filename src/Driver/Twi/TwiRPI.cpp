#include "Twi.h"

#include <string.h>
#include "pigpio.h"

namespace Driver
{
    namespace Twi
    {
        Twi::Twi(const EI2cFreq &freq) {
            (void) freq;
        }

        Core::Status Twi::Initialize() {
            if (gpioInitialise() < 0) {
                return (Core::Status::CORE_ERROR);
            }

            LOG_DRIVER_DEBUG("Twi", "Initialized.");
            return (Core::Status::CORE_OK);
        }

        void Twi::Update(const uint64_t currentTime) {
            (void) currentTime;
        }

        namespace
        {

            // The Pi typically uses bus 1 for I2C.
            static constexpr int kDefaultI2cBus = 1;

            bool                 OpenHandle(int address, int &handle) {
                handle = i2cOpen(kDefaultI2cBus, address, 0);
                if (handle < 0) {
                    LOG_DRIVER_ERROR(
                            "Twi", "i2cOpen failed for address 0x%02X: %d", address, handle);
                }
                return handle >= 0;
            }

        } // namespace

        bool Twi::SendCommand(const uint8_t address, const uint8_t command) {
            int handle;
            if (!OpenHandle(address, handle)) {
                return false;
            }
            const int result = i2cWriteByte(handle, command);
            i2cClose(handle);
            return (result >= 0);
        }

        bool Twi::ReadRegister(const uint8_t address, const uint8_t reg, uint8_t &data) {
            int handle;
            if (!OpenHandle(address, handle)) {
                return false;
            }
            const int result = i2cReadByteData(handle, reg);
            i2cClose(handle);
            if (result < 0) {
                return false;
            }
            data = static_cast<uint8_t>(result);
            return true;
        }
        bool Twi::Read16Register(const uint8_t address, const uint16_t reg, uint8_t &data) {
            int handle;
            if (!OpenHandle(address, handle)) {
                return false;
            }
            char      addr[2U] = {static_cast<char>((uint16_t) (reg >> 8U) & 0xFFU),
                                  static_cast<char>(reg & 0xFFU)};
            const int setAddrResult = i2cWriteDevice(handle, addr, 2); // Send 16-bit address
            if (setAddrResult != 0) {
                i2cClose(handle);
                return false;
            }

            char      value = 0;
            const int readResult = i2cReadDevice(handle, &value, 1);
            if (readResult != 1) {
                i2cClose(handle);
                return false;
            }

            data = static_cast<uint8_t>(value);
            i2cClose(handle);
            return true;
        }

        bool Twi::ReadRegister16Bits(const uint8_t address, const uint8_t reg, uint16_t &data) {
            int handle;
            if (!OpenHandle(address, handle)) {
                return false;
            }

            const int low = i2cReadByteData(handle, reg);
            const int high = i2cReadByteData(handle, reg + 1);
            i2cClose(handle);
            if (low < 0 || high < 0) {
                return false;
            }

            data = static_cast<uint16_t>((static_cast<uint16_t>(high) << 8) |
                                         static_cast<uint16_t>(low));
            return true;
        }

        bool Twi::ReadRegister24Bits(const uint8_t address, const uint8_t reg, uint32_t &data) {
            int handle;
            if (!OpenHandle(address, handle)) {
                return false;
            }

            const int low = i2cReadByteData(handle, reg);
            const int mid = i2cReadByteData(handle, reg + 1);
            const int high = i2cReadByteData(handle, reg + 2);
            i2cClose(handle);
            if (low < 0 || mid < 0 || high < 0) {
                return false;
            }

            data = (static_cast<uint32_t>(high) << 16) | (static_cast<uint32_t>(mid) << 8) |
                   static_cast<uint32_t>(low);
            return true;
        }

        bool Twi::ReadRegister32Bits(const uint8_t address, const uint8_t reg, uint32_t &data) {
            int handle;
            if (!OpenHandle(address, handle)) {
                return false;
            }

            const int b0 = i2cReadByteData(handle, reg);
            const int b1 = i2cReadByteData(handle, reg + 1);
            const int b2 = i2cReadByteData(handle, reg + 2);
            const int b3 = i2cReadByteData(handle, reg + 3);
            i2cClose(handle);
            if (b0 < 0 || b1 < 0 || b2 < 0 || b3 < 0) {
                return false;
            }

            data = (static_cast<uint32_t>(b3) << 24) | (static_cast<uint32_t>(b2) << 16) |
                   (static_cast<uint32_t>(b1) << 8) | static_cast<uint32_t>(b0);
            return true;
        }

        bool Twi::ReadRegisters(const uint8_t address,
                                const uint8_t reg,
                                uint8_t      *data,
                                const uint8_t length) {
            int handle;
            if (!OpenHandle(address, handle)) {
                return false;
            }

            // pigpio supports a maximum of 32 bytes per SMBus block transfer.
            constexpr uint8_t kMaxBlockSize = 32;

            uint8_t           remaining = length;
            uint8_t           offset = 0U;
            uint8_t           currentReg = reg;

            while (remaining > 0) {
                const uint8_t chunk = (remaining > kMaxBlockSize) ? kMaxBlockSize : remaining;
                const int     result = i2cReadI2CBlockData(
                        handle, currentReg, reinterpret_cast<char *>(data + offset), chunk);
                if (result < 0) {
                    i2cClose(handle);
                    return false;
                }
                if (result != static_cast<int>(chunk)) {
                    i2cClose(handle);
                    return false;
                }

                remaining -= chunk;
                offset += chunk;
                currentReg += chunk;
            }

            i2cClose(handle);
            return true;
        }

        bool Twi::Read16Registers(const uint8_t  address,
                                  const uint16_t reg,
                                  uint8_t       *data,
                                  const uint16_t length) {
            int handle;
            if (!OpenHandle(address, handle)) {
                return false;
            }

            uint16_t offset = 0U;
            while (offset < length) {
                const uint16_t currentReg = static_cast<uint16_t>(reg + offset);
                char           addr[2] = {static_cast<char>((uint16_t) (currentReg >> 8U) & 0xFFU),
                                          static_cast<char>(currentReg & 0xFFU)};

                const int setAddrResult = i2cWriteDevice(handle, addr, 2); // Send 16-bit address
                if (setAddrResult != 0U) {
                    i2cClose(handle);
                    return false;
                }
                const uint16_t chunk =
                        (static_cast<uint16_t>(length - offset) > 32U) ? 32U : (length - offset);
                const int readResult =
                        i2cReadDevice(handle, reinterpret_cast<char *>(data + offset), chunk);
                if (readResult != static_cast<int>(chunk)) {
                    i2cClose(handle);
                    return false;
                }

                offset = static_cast<uint16_t>(offset + chunk);
            }

            i2cClose(handle);
            return true;
        }


        bool Twi::Write(const uint8_t address, const uint8_t reg) {
            int handle;
            if (!OpenHandle(address, handle)) {
                return false;
            }
            const int result = i2cWriteByte(handle, reg);
            i2cClose(handle);
            return (result >= 0);
        }

        bool Twi::WriteRegister(const uint8_t address, const uint8_t reg, uint8_t data) {
            int handle;
            if (!OpenHandle(address, handle)) {
                return false;
            }
            const int result = i2cWriteByteData(handle, reg, data);
            i2cClose(handle);
            return (result >= 0);
        }
        bool Twi::Write16Register(const uint8_t address, const uint16_t reg, uint8_t data) {
            int handle;
            if (!OpenHandle(address, handle)) {
                return false;
            }

            char      payload[3U] = {static_cast<char>((uint16_t) (reg >> 8U) & 0xFFU),
                                     static_cast<char>(reg & 0xFFU),
                                     static_cast<char>(data)};
            const int result = i2cWriteDevice(handle, payload, 3);
            i2cClose(handle);
            return (result == 0);
        }

        bool Twi::WriteRegister16Bits(const uint8_t address, const uint8_t reg, uint16_t &data) {
            int handle;
            if (!OpenHandle(address, handle)) {
                return false;
            }

            // Write low and high bytes separately for portability.
            const int low = i2cWriteByteData(handle, reg, static_cast<uint8_t>(data & 0xFFu));
            const int high =
                    i2cWriteByteData(handle, reg + 1, static_cast<uint8_t>((data >> 8) & 0xFFu));
            i2cClose(handle);
            return (low >= 0 && high >= 0);
        }

        bool Twi::WriteRegister32Bits(const uint8_t address, const uint8_t reg, uint32_t &data) {
            int handle;
            if (!OpenHandle(address, handle)) {
                return false;
            }

            const int b0 = i2cWriteByteData(handle, reg, static_cast<uint8_t>(data & 0xFFu));
            const int b1 =
                    i2cWriteByteData(handle, reg + 1, static_cast<uint8_t>((data >> 8) & 0xFFu));
            const int b2 =
                    i2cWriteByteData(handle, reg + 2, static_cast<uint8_t>((data >> 16) & 0xFFu));
            const int b3 =
                    i2cWriteByteData(handle, reg + 3, static_cast<uint8_t>((data >> 24) & 0xFFu));
            i2cClose(handle);
            return (b0 >= 0 && b1 >= 0 && b2 >= 0 && b3 >= 0);
        }

        bool Twi::WriteRegisters(const uint8_t address,
                                 const uint8_t reg,
                                 uint8_t      *data,
                                 const uint8_t length) {
            int handle;
            if (!OpenHandle(address, handle)) {
                return false;
            }

            // pigpio supports a maximum of 32 bytes per SMBus block transfer.
            constexpr uint8_t kMaxBlockSize = 32;

            uint8_t           remaining = length;
            uint8_t           offset = 0U;
            uint8_t           currentReg = reg;

            while (remaining > 0) {
                const uint8_t chunk = (remaining > kMaxBlockSize) ? kMaxBlockSize : remaining;
                const int     result = i2cWriteI2CBlockData(
                        handle, currentReg, reinterpret_cast<char *>(data + offset), chunk);
                if (result < 0) {
                    i2cClose(handle);
                    return false;
                }

                remaining -= chunk;
                offset += chunk;
                currentReg += chunk;
            }

            i2cClose(handle);
            return true;
        }

        bool Twi::Write16Registers(const uint8_t  address,
                                   const uint16_t reg,
                                   uint8_t       *data,
                                   const uint16_t length) {
            int handle;
            if (!OpenHandle(address, handle)) {
                return false;
            }

            uint16_t offset = 0U;
            while (offset < length) {
                const uint16_t currentReg = static_cast<uint16_t>(reg + offset);
                const uint16_t chunk =
                        (static_cast<uint16_t>(length - offset) > 30U) ? 30U : (length - offset);

                uint8_t payload[32U] = {0U};
                payload[0] = static_cast<uint8_t>((uint16_t) (currentReg >> 8U) & 0xFFU);
                payload[1] = static_cast<uint8_t>(currentReg & 0xFFU);
                (void) memcpy(payload + 2U, data + offset, chunk);

                const int writeResult = i2cWriteDevice(handle,
                                                       reinterpret_cast<char *>(payload),
                                                       static_cast<unsigned>(chunk + 2U));
                if (writeResult != 0U) {
                    i2cClose(handle);
                    return false;
                }

                offset = static_cast<uint16_t>(offset + chunk);
            }

            i2cClose(handle);

            return true;
        }
    } // namespace Twi
} // namespace Driver
