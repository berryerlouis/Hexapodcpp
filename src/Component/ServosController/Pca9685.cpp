#include "Pca9685.h"
namespace Component
{
    namespace ServosController
    {
        Pca9685::Pca9685(Twi::TwiInterface &i2c, const uint8_t address)
            : mI2c(i2c)
            , mAddress(address)
            , mInternalOscillatorFrequency(EConstant::FREQUENCY_OSCILLATOR)
            , mPwm{{0U, 0U}}
            , mIsDirty(true) {
        }

        Core::Status Pca9685::Initialize() {
            this->Reset();
            LOG_COMPONENT_DEBUG("ServosController", "address 0x%02X Initialized.", this->mAddress);
            return (Core::Status::CORE_OK);
        }

        void Pca9685::Update(const uint64_t currentTime) {
            (void) currentTime;
            if (!this->mIsDirty) {
                return;
            }
            this->mI2c.WriteRegisters(this->mAddress,
                                      static_cast<uint8_t>(ERegister::LED0_ON_L),
                                      reinterpret_cast<uint8_t *>(this->mPwm),
                                      4 * EConstant::NB_LEDS);
            this->mIsDirty = false;
        }

        void Pca9685::Reset() {
            this->mI2c.WriteRegister(this->mAddress,
                                     static_cast<uint8_t>(ERegister::MODE1),
                                     static_cast<uint8_t>(ERegisterMode1::RESTART));

            for (size_t idPwm = 0U; idPwm < EConstant::NB_LEDS; idPwm++) {
                this->SetPwm(idPwm, EConstant::LED_OFF);
            }

            this->SetFrequency(PCA9685_FREQUENCY);
        }


        void Pca9685::Sleep() {
            uint8_t awake = 0U;

            this->mI2c.ReadRegister(this->mAddress, static_cast<uint8_t>(ERegister::MODE1), awake);
            awake |= ERegisterMode1::SLEEP;
            this->mI2c.WriteRegister(this->mAddress, static_cast<uint8_t>(ERegister::MODE1), awake);
        }

        void Pca9685::WakeUp() {
            uint8_t wakeUp = 0U;

            this->mI2c.ReadRegister(this->mAddress, static_cast<uint8_t>(ERegister::MODE1), wakeUp);
            wakeUp &= ~ERegisterMode1::SLEEP;
            this->mI2c.WriteRegister(
                    this->mAddress, static_cast<uint8_t>(ERegister::MODE1), wakeUp);
        }

        void Pca9685::setOscillatorFrequency(const uint32_t frequency) {
            this->mInternalOscillatorFrequency = frequency;
        }

        void Pca9685::SetFrequency(const uint16_t frequency) {
            float prescaleval =
                    ((mInternalOscillatorFrequency / (frequency * 4096.0F)) + 0.5F) - 1.0F;

            if (prescaleval < EConstant::PRESCALE_MIN) {
                prescaleval = EConstant::PRESCALE_MIN;
            }

            if (prescaleval > EConstant::PRESCALE_MAX) {
                prescaleval = EConstant::PRESCALE_MAX;
            }
            const uint8_t prescale = static_cast<uint8_t>(prescaleval);

            uint8_t       oldmode = 0U;
            this->mI2c.ReadRegister(
                    this->mAddress, static_cast<uint8_t>(ERegister::MODE1), oldmode);
            const uint8_t newmode = (oldmode & ~ERegisterMode1::RESTART) | ERegisterMode1::SLEEP;
            this->mI2c.WriteRegister(
                    this->mAddress, static_cast<uint8_t>(ERegister::MODE1), newmode);
            this->mI2c.WriteRegister(
                    this->mAddress, static_cast<uint8_t>(ERegister::PRESCALE), prescale);
            this->mI2c.WriteRegister(
                    this->mAddress, static_cast<uint8_t>(ERegister::MODE1), oldmode);
            this->mI2c.WriteRegister(
                    this->mAddress,
                    static_cast<uint8_t>(ERegister::MODE1),
                    static_cast<uint8_t>(oldmode | ERegisterMode1::RESTART | ERegisterMode1::AI));
        }

        void Pca9685::SetPwm(const uint8_t num, const uint16_t off) {
            const uint16_t newOn = 0U;
            uint16_t       newOff = EConstant::LED_OFF;

            if (off > 500U && off < 3000U) {
                newOff = off;
            }

            if (this->mPwm[num].on != newOn || this->mPwm[num].off != newOff) {
                this->mPwm[num].on = newOn;
                this->mPwm[num].off = newOff;
                this->mIsDirty = true;
            }
        }

        uint8_t Pca9685::GetAddress() {
            return this->mAddress;
        }
    } // namespace ServosController
} // namespace Component
