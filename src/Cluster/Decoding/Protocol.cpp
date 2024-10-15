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
                sscanf(frameBuffer, "%02x%02x%02x",
                       (unsigned int *) &frame.clusterId,
                       (unsigned int *) &frame.commandId,
                       (unsigned int *) &frame.nbParams);

                if (frame.nbParams == 0U && frameLength == 6U) {
                    return (Core::CoreStatus::CORE_OK);
                } else if ((frame.nbParams * 2U) + 6U == frameLength) {
                    for (size_t i = 0U; i < frame.nbParams * 2U; i += 2U) {
                        sscanf(&frameBuffer[6U + i], "%02x", (unsigned int *) &frame.params[i / 2U]);
                    }
                    return (Core::CoreStatus::CORE_OK);
                }

                // wrong param size
                return (Core::CoreStatus::CORE_ERROR_OVERLOAD);
            }
            // frameLength is not or less than 6 bytes
            return (Core::CoreStatus::CORE_ERROR_SIZE);
        }

        uint8_t Protocol::Encode(const Frame &response, char *buffer) {
            if (buffer == nullptr) {
                return (0U);
            }
            const uint8_t *params = response.params;
            const uint8_t size = response.nbParams;
            const uint8_t cluster = response.clusterId;
            const uint8_t command = response.commandId;

            uint8_t length = snprintf(const_cast<char *>(buffer), 8U, "<%02X%02X%02X", cluster, command, size);

            for (size_t i = 0U; i < size; i++) {
                length += snprintf(const_cast<char *>(&buffer[length]), 3U, "%02X", params[i]);
            }

            buffer[length] = '>';
            buffer[length + 1U] = '\0';
            return (length + 1U);
        }

        uint8_t Protocol::ConvertHexCharToInt(const uint8_t byte) {
            if ((byte >= '0') && (byte <= '9')) {
                return (byte - '0');
            }
            if ((byte >= 'A') && (byte <= 'F')) {
                return (byte + 10U - 'A');
            }
            return (0xFFU);
        }
    }
}
