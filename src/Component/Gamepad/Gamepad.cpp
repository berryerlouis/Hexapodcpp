#include "Gamepad.h"

namespace Component
{
    namespace Gamepad
    {
        using Driver::Joystick::JoystickEvent;
        using Driver::Joystick::JS_EVENT_AXIS;
        using Driver::Joystick::JS_EVENT_BUTTON;
        using Driver::Joystick::JS_EVENT_INIT;

        static constexpr float    AXIS_SCALE = 1.0F / 32767.0F;
        // Below this magnitude we treat axes as zero (joysticks drift slightly at rest).
        static constexpr float    AXIS_DEADZONE = 0.12F;
        // Limit how many events we drain per Update() so we never starve other services.
        static constexpr uint8_t  MAX_EVENTS_PER_UPDATE = 16U;

        Gamepad::Gamepad(Driver::Joystick::JoystickInterface &joystick)
            : mJoystick(joystick)
            , mButtonState{}
            , mConnectedState{}
            , mConnected(false)
            , mDirty(false) {
        }

        Core::Status Gamepad::Initialize() {
            const Core::Status success = this->mJoystick.Initialize();
            for (uint8_t i = 0U; i < NB_AXES; i++) {
                this->mButtonState.axes[i] = 0.0F;
            }
            for (uint8_t i = 0U; i < NB_BUTTONS; i++) {
                this->mButtonState.buttons[i] = false;
            }
            this->mConnectedState.connected = this->mJoystick.IsConnected();
            LOG_COMPONENT_DEBUG("Gamepad", "Initialized (connected=%d).", this->mConnectedState.connected);
            return success;
        }

        void Gamepad::Update(const uint64_t currentTime) {
            this->mJoystick.Update(currentTime);

            if (this->mConnected != this->mJoystick.IsConnected()) {
                this->mDirty                     = true;
                if (this->mJoystick.IsConnected()) {
                    LOG_COMPONENT_INFO("Gamepad", "Controller connected.");
                } else {
                    LOG_COMPONENT_INFO("Gamepad", "Controller disconnected.");
                }

                this->mConnectedState.connected = this->mJoystick.IsConnected();
                Core::Observable<3U, GamepadConnectedState>::Notify(this->mConnectedState);
                this->mConnected = this->mJoystick.IsConnected();
            }

            if (this->mConnected) {
                JoystickEvent event;
                for (uint8_t i = 0U; (i < MAX_EVENTS_PER_UPDATE) && this->mJoystick.ReadEvent(event); i++) {
                    const uint8_t kind = event.type & ~JS_EVENT_INIT;
                    if (kind == JS_EVENT_AXIS && event.number < NB_AXES) {
                        float value = static_cast<float>(event.value) * AXIS_SCALE;
                        if (value > -AXIS_DEADZONE && value < AXIS_DEADZONE) {
                            value = 0.0F;
                        }
                        if (value != this->mButtonState.axes[event.number]) {
                            this->mButtonState.axes[event.number] = value;
                            this->mDirty                    = true;
                        }
                    } else if (kind == JS_EVENT_BUTTON && event.number < NB_BUTTONS) {
                        const bool pressed = (event.value != 0);
                        if (pressed != this->mButtonState.buttons[event.number]) {
                            this->mButtonState.buttons[event.number] = pressed;
                            this->mDirty                       = true;
                        }
                    }
                }

                if (this->mDirty) {
                    this->mDirty = false;
                    ComponentInterface<3U, GamepadButtonState>::Notify(this->mButtonState);
                }
            }
        }

        const GamepadButtonState &Gamepad::GetButtonState() const {
            return this->mButtonState;
        }

        const GamepadConnectedState &Gamepad::GetConnectedState() const {
            return this->mConnectedState;
        }

        bool Gamepad::IsConnected() const {
            return this->mConnectedState.connected;
        }
    } // namespace Gamepad
} // namespace Component
