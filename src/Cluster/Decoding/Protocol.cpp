#include "Protocol.h"

namespace Cluster
{
    namespace Decoding
    {
        Protocol::Protocol() {
        }

        Core::CoreStatus Protocol::Decode(const char *frameBuffer, Frame &frame) {
            if (frameBuffer == nullptr) {
                return (Core::CoreStatus::CORE_ERROR_NULLPTR);
            }
            const uint8_t frameLength = strlen(frameBuffer);

            if (frameLength >= 6U && frameLength % 2U == 0U) {
                for (size_t i = 0U; i < frameLength; i++) {
                    if (Protocol::ConvertHexCharToInt(frameBuffer[i]) == 0xFFU) {
                        // char is invalid
                        return (Core::CoreStatus::CORE_ERROR_ARGUMENT);
                    }
                }

                frame.clusterId = Protocol::ConvertHexCharToInt(frameBuffer[0U]) * 16U + Protocol::ConvertHexCharToInt(
                                      frameBuffer[1U]);
                frame.commandId = Protocol::ConvertHexCharToInt(frameBuffer[2U]) * 16U + Protocol::ConvertHexCharToInt(
                                      frameBuffer[3U]);
                frame.nbParams = Protocol::ConvertHexCharToInt(frameBuffer[4U]) * 16U + Protocol::ConvertHexCharToInt(
                                     frameBuffer[5U]);

                if (frame.nbParams == 0U && frameLength == 6U) {
                    return (Core::CoreStatus::CORE_OK);
                } else if ((frame.nbParams * 2U) + 6U == frameLength) {
                    for (size_t i = 0U; i < frame.nbParams * 2U; i += 2U) {
                        frame.params[i / 2U] = Protocol::ConvertHexCharToInt(frameBuffer[6 + i]) * 16U +
                                               Protocol::ConvertHexCharToInt(frameBuffer[7U + i]);
                    }

                    return (Core::CoreStatus::CORE_OK);
                }

                // wrong param size
                return (Core::CoreStatus::CORE_ERROR_OVERLOAD);
            }
            // frameLength is not or less than 6 bytes
            return (Core::CoreStatus::CORE_ERROR_SIZE);
        }

        uint8_t Protocol::Encode(const Frame &response, const char *buffer) {
            if (buffer == nullptr) {
                return (0U);
            }
            const uint8_t *params = response.params;
            const uint8_t size = response.nbParams;
            const uint8_t cluster = response.clusterId;
            const uint8_t command = response.commandId;

            uint8_t length = sprintf(const_cast<char *>(buffer), "<%02x%02x%02x", cluster, command, size);

            for (size_t i = 0U; i < size; i++) {
                length += sprintf(const_cast<char *>(&buffer[length]), "%02x", params[i]);
            }

            sprintf(const_cast<char *>(&buffer[length]), ">");
            return (strlen(buffer));
        }

        uint8_t Protocol::ConvertHexCharToInt(const uint8_t byte) {
            if ((byte >= '0') && (byte <= '9')) {
                return (byte - '0');
            }

            if ((byte >= 'A') && (byte <= 'F')) {
                return (byte + 10U - 'A');
            }

            if ((byte >= 'a') && (byte <= 'f')) {
                return (byte + 10U - 'a');
            }
            return (0xFFU);
        }
    }
}
