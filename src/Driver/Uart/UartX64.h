#pragma once

#include "UartInterface.h"
#include "../../Misc/Logger/Logger.h"

namespace Driver {
    namespace Uart {
        class Uart : public UartInterface {
        public:

            Uart(const EBaudRate &baud = BAUDRATE_115200) {
                (void) baud;
            }

            ~Uart() = default;

            virtual Core::CoreStatus Initialize(void) final override {
                return (Core::CoreStatus::CORE_OK);
            }

            virtual void Update(const uint64_t currentTime) final override {
                (void) currentTime;
            }

            virtual void Send(const char *data, const size_t len) final override {
                (void) data;
                (void) len;
                LOG(data);
            }

            virtual void Send(const uint8_t *data, const size_t len) final override {
                (void) data;
                (void) len;
                LOG(data);
            }

            virtual void Send(const uint8_t data) final override {
                (void) data;
                LOG(data);
            }

            virtual uint8_t Read(void) final override {
                return 0;
            }

            virtual uint8_t DataAvailable(void) const final override {
                return 0;
            }
        };
    }
}
