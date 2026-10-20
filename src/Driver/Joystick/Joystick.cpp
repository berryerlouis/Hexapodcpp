#include "Joystick.h"

#ifndef GTEST
#include <fcntl.h>
#include <linux/joystick.h>
#include <unistd.h>
#include <cerrno>
#endif

namespace Driver
{
    namespace Joystick
    {
        Joystick::Joystick(const char *devicePath)
            : mDevicePath(devicePath)
            , mFd(-1)
            , mLastReopenAttemptMs(0U) {
        }

#ifndef GTEST

        // Re-open period when the controller is unplugged.
        static constexpr uint64_t REOPEN_PERIOD_MS = 1000U;

        Joystick::~Joystick() {
            if (this->mFd >= 0) {
                ::close(this->mFd);
                this->mFd = -1;
            }
        }

        Core::Status Joystick::Initialize() {
            this->mFd = ::open(this->mDevicePath, O_RDONLY | O_NONBLOCK);
            if (this->mFd < 0) {
                LOG_DRIVER_WARNING("Joystick",
                                   "Could not open %s (errno=%d). Will retry.",
                                   this->mDevicePath,
                                   errno);
            } else {
                LOG_DRIVER_DEBUG("Joystick", "Opened %s (fd=%d).", this->mDevicePath, this->mFd);
            }
            // Always OK so the rest of the system runs even when no controller is connected.
            return Core::Status::CORE_OK;
        }

        void Joystick::Update(const uint64_t currentTime) {
            if (this->mFd < 0) {
                if ((currentTime - this->mLastReopenAttemptMs) >= REOPEN_PERIOD_MS) {
                    this->mLastReopenAttemptMs = currentTime;
                    this->mFd = ::open(this->mDevicePath, O_RDONLY | O_NONBLOCK);
                    if (this->mFd >= 0) {
                        LOG_DRIVER_INFO("Joystick", "Reconnected to %s.", this->mDevicePath);
                    }
                }
            }
        }

        bool Joystick::IsConnected() const {
            return this->mFd >= 0;
        }

        bool Joystick::ReadEvent(JoystickEvent &event) {
            if (this->mFd < 0) {
                return false;
            }
            struct js_event raw;
            const ssize_t   bytes = ::read(this->mFd, &raw, sizeof(raw));
            if (bytes == static_cast<ssize_t>(sizeof(raw))) {
                event.timeMs = raw.time;
                event.value  = raw.value;
                event.type   = raw.type;
                event.number = raw.number;
                return true;
            }
            if (bytes < 0 && errno != EAGAIN && errno != EWOULDBLOCK) {
                LOG_DRIVER_WARNING("Joystick", "read() failed (errno=%d). Closing.", errno);
                ::close(this->mFd);
                this->mFd = -1;
            }
            return false;
        }

#else
        Joystick::~Joystick() = default;
#endif
    } // namespace Joystick
} // namespace Driver
