#include "Protocol.h"

namespace Cluster
{
    namespace Decoding
    {
        Protocol::Protocol() {
        }

        Core::Status Protocol::Decode(const char *frameBuffer, Frame &frame) {
            if (frameBuffer == nullptr) {
                return (Core::Status::CORE_ERROR_NULLPTR);
            }
            const uint8_t frameLength = strlen(frameBuffer);
            if (frameLength >= 6U && frameLength % 2U == 0U) {
                uint8_t commandId = 0U;
                uint8_t clusterId = 0U;
                uint8_t nbParams = 0U;
                uint8_t params[FRAME_MAX_PARAMS] = {0U};
                unsigned int tempClusterId = 0U;
                unsigned int tempCommandId = 0U;
                unsigned int tempNbParams = 0U;
                unsigned int tempParam = 0U;
                sscanf(frameBuffer, "%02x%02x%02x", &tempClusterId, &tempCommandId, &tempNbParams);
                clusterId = static_cast<uint8_t>(tempClusterId);
                commandId = static_cast<uint8_t>(tempCommandId);
                nbParams = static_cast<uint8_t>(tempNbParams);

                if (nbParams == 0U && frameLength == 6U) {
                    return (frame.Build(clusterId, commandId, params, nbParams));
                }
                if ((nbParams * 2U) + 6U == frameLength) {
                    for (size_t i = 0U; i < nbParams * 2U; i += 2U) {
                        sscanf(&frameBuffer[6U + i], "%02x", &tempParam);
                        params[i / 2U] = static_cast<uint8_t>(tempParam);
                    }
                    return (frame.Build(clusterId, commandId, params, nbParams));
                }

                // wrong param size
                return (Core::Status::CORE_ERROR_OVERLOAD);
            }
            // frameLength is not or less than 6 bytes
            return (Core::Status::CORE_ERROR_SIZE);
        }

        uint8_t Protocol::Encode(const Frame &response, char *buffer) {
            if (buffer == nullptr) {
                return (0U);
            }
            const uint8_t size = response.GetNbParams();
            const uint8_t cluster = response.GetClusterId();
            const uint8_t command = response.GetCommandId();

            uint8_t length = snprintf(buffer, 8U, "<%02X%02X%02X", cluster, command, size);

            for (size_t i = 0U; i < size; i++) {
                length += snprintf(&buffer[length], 3U, "%02X", response.Get1ByteParam(i));
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
