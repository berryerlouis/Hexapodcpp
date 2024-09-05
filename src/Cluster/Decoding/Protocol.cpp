#include "Protocol.h"

namespace Cluster
{
    namespace Decoding
    {
        Protocol::Protocol() {
        }

        Protocol::ProtocolStatus Protocol::Decode(const char *frameBuffer, Frame &frame) {
            if (frameBuffer == nullptr) {
                return (ERROR_NULL_BUFFER);
            }
            uint8_t frameLength = strlen(frameBuffer);

            if (frameLength >= 6U && frameLength % 2U == 0U) {
                for (size_t i = 0U; i < frameLength; i++) {
                    if (Protocol::ConvertHexCharToInt(frameBuffer[i]) == 0xFFU) {
                        // char is invalid
                        return (ERROR_CHAR_INVALID);
                    }
                }

                frame.clusterId = Protocol::ConvertHexCharToInt(frameBuffer[0U]) * 16U + Protocol::ConvertHexCharToInt(
                                      frameBuffer[1U]);
                frame.commandId = Protocol::ConvertHexCharToInt(frameBuffer[2U]) * 16U + Protocol::ConvertHexCharToInt(
                                      frameBuffer[3U]);
                frame.nbParams = Protocol::ConvertHexCharToInt(frameBuffer[4U]) * 16U + Protocol::ConvertHexCharToInt(
                                     frameBuffer[5U]);

                if (frame.nbParams == 0U && frameLength == 6U) {
                    return (NO_ERROR);
                } else if ((frame.nbParams * 2U) + 6U == frameLength) {
                    for (size_t i = 0U; i < frame.nbParams * 2U; i += 2U) {
                        frame.params[i / 2U] = Protocol::ConvertHexCharToInt(frameBuffer[6 + i]) * 16U +
                                               Protocol::ConvertHexCharToInt(frameBuffer[7U + i]);
                    }

                    return (NO_ERROR);
                } else {
                    // wrong param size
                    return (ERROR_SIZE_PARAMS);
                }
            } else {
                // frameLength is not or less than 6 bytes
                return (ERROR_LENGHT);
            }
        }

        uint8_t Protocol::Encode(const Frame &response, const uint8_t *buffer) {
            if (buffer == nullptr || response.nbParams == 0) {
                return (0U);
            }
            const uint8_t *params = response.params;
            const uint8_t size = response.nbParams;
            const uint8_t cluster = response.clusterId;
            const uint8_t command = response.commandId;

            uint8_t length = sprintf((char *) (buffer), "<%02x%02x%02x", cluster, command, size);

            for (size_t i = 0U; i < size; i++) {
                length += sprintf((char *) &buffer[length], "%02x", params[i]);
            }

            sprintf((char *) &buffer[length], ">");
            return (strlen((char *) buffer));
        }

        uint8_t Protocol::ConvertHexCharToInt(uint8_t byte) {
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
