#pragma once

#include <cstdint>
#include "../DriverInterface.h"

namespace Driver
{
    namespace Joystick
    {
        /// One raw event coming from the kernel joystick interface.
        struct JoystickEvent {
            uint32_t timeMs;   ///< Event timestamp in ms (kernel-provided).
            int16_t  value;    ///< Axis value (-32767..32767) or button state (0/1).
            uint8_t  type;     ///< Bitmask: BUTTON / AXIS / INIT (see EJoystickEventType).
            uint8_t  number;   ///< Axis or button index.
        };

        enum EJoystickEventType : uint8_t {
            JS_EVENT_BUTTON = 0x01U,
            JS_EVENT_AXIS   = 0x02U,
            JS_EVENT_INIT   = 0x80U
        };

        class JoystickInterface : public DriverInterface {
        public:
            JoystickInterface()           = default;
            ~JoystickInterface()          = default;

            /// True if the device node was successfully opened.
            virtual bool IsConnected(void) const                          = 0;

            /// Reads one pending event. Returns true if an event was read.
            virtual bool ReadEvent(JoystickEvent &event)                  = 0;
        };
    } // namespace Joystick
} // namespace Driver
