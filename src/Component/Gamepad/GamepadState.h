#pragma once

#include <cstdint>

namespace Component
{
    namespace Gamepad
    {
        // PS3 (Sixaxis / DualShock 3) Linux hid-sony mapping.
        // Note: depending on the kernel driver some indexes can shift; tweak here if needed.
        enum EGamepadButton : uint8_t {
            BTN_CROSS    = 0U,
            BTN_CIRCLE   = 1U,
            BTN_TRIANGLE = 2U,
            BTN_SQUARE   = 3U,
            BTN_L1       = 4U,
            BTN_R1       = 5U,
            BTN_L2       = 6U,
            BTN_R2       = 7U,
            BTN_SELECT   = 8U,
            BTN_START    = 9U,
            BTN_PS       = 10U,
            BTN_L3       = 11U,
            BTN_R3       = 12U,
            BTN_DPAD_UP    = 13U,
            BTN_DPAD_DOWN  = 14U,
            BTN_DPAD_LEFT  = 15U,
            BTN_DPAD_RIGHT = 16U,
            NB_BUTTONS     = 17U
        };

        enum EGamepadAxis : uint8_t {
            AXIS_LEFT_X  = 0U,
            AXIS_LEFT_Y  = 1U,
            AXIS_RIGHT_X = 2U,
            AXIS_RIGHT_Y = 3U,
            AXIS_L2      = 4U,
            AXIS_R2      = 5U,
            NB_AXES      = 6U
        };

        /// Snapshot of the gamepad state. Axes are normalized to [-1.0, +1.0].
        struct GamepadButtonState {
            float axes[NB_AXES];
            bool  buttons[NB_BUTTONS];
        };
        
        /// Snapshot of the gamepad state. Axes are normalized to [-1.0, +1.0].
        struct GamepadConnectedState {
            bool  connected;
        };
    } // namespace Gamepad
} // namespace Component
