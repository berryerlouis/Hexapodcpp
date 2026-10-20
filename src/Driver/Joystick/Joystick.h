#pragma once

#include "JoystickInterface.h"

namespace Driver
{
    namespace Joystick
    {
        /// Reads PS3 (or any) gamepad events from a Linux /dev/input/jsX device
        /// using the legacy joystick API in non-blocking mode. Same implementation
        /// works on X64 (development host) and RPI (target).
        class Joystick : public JoystickInterface {
        public:
            explicit Joystick(const char *devicePath = "/dev/input/js0");

            ~Joystick();

#ifndef GTEST
            virtual Core::Status Initialize(void) final override;

            virtual void         Update(const uint64_t currentTime) final override;

            virtual bool         IsConnected(void) const final override;

            virtual bool         ReadEvent(JoystickEvent &event) final override;
#else
            virtual Core::Status Initialize(void) final override {
                return Core::Status::CORE_OK;
            }

            virtual void Update(const uint64_t currentTime) final override {
                (void) currentTime;
            }

            virtual bool IsConnected(void) const final override {
                return false;
            }

            virtual bool ReadEvent(JoystickEvent &event) final override {
                (void) event;
                return false;
            }
#endif

        private:
            const char *mDevicePath;
            int         mFd;
            uint64_t    mLastReopenAttemptMs;
        };
    } // namespace Joystick
} // namespace Driver
