#pragma once

#include "Pca9685Interface.h"
#include "../../Driver/Twi/TwiInterface.h"

namespace Component
{
    namespace ServosController
    {
        using namespace Driver;

        class Pca9685 : public Pca9685Interface {
        public:
            static const uint8_t PCA9685_I2C_ADDRESS = 0x40U;
            static const uint8_t SERVO_FREQUENCY = 50U;

            struct ERegister {
                static const uint8_t MODE1 = 0x00;
                static const uint8_t MODE2 = 0x01;
                static const uint8_t SUBADR1 = 0x02;
                static const uint8_t SUBADR2 = 0x03;
                static const uint8_t SUBADR3 = 0x04;
                static const uint8_t ALLCALLADR = 0x05;
                static const uint8_t LED0_ON_L = 0x06;
                static const uint8_t LED0_ON_H = 0x07;
                static const uint8_t LED0_OFF_L = 0x08;
                static const uint8_t LED0_OFF_H = 0x09;
                static const uint8_t ALLLED_ON_L = 0xFA;
                static const uint8_t ALLLED_ON_H = 0xFB;
                static const uint8_t ALLLED_OFF_L = 0xFC;
                static const uint8_t ALLLED_OFF_H = 0xFD;
                static const uint8_t PRESCALE = 0xFE;
                static const uint8_t TESTMODE = 0xFF;
            };

            struct ERegisterMode1 {
                static const uint8_t ALLCAL = 0x01;
                static const uint8_t SUB3 = 0x02;
                static const uint8_t SUB2 = 0x04;
                static const uint8_t SUB1 = 0x08;
                static const uint8_t SLEEP = 0x10;
                static const uint8_t AI = 0x20;
                static const uint8_t EXTCLK = 0x40;
                static const uint8_t RESTART = 0x80;
            };

            struct ERegisterMode2 {
                static const uint8_t OUTNE_0 = 0x01;
                static const uint8_t OUTNE_1 = 0x02;
                static const uint8_t OUTDRV = 0x04;
                static const uint8_t OCH = 0x08;
                static const uint8_t INVRT = 0x10;
            };

            struct EConstant {
                static const uint32_t FREQUENCY_OSCILLATOR = 27000000;
                static const uint8_t PRESCALE_MIN = 3;
                static const uint8_t PRESCALE_MAX = 255;
                static const uint8_t NB_LEDS = 10U;
                static const uint16_t LED_OFF = 4096U;
            };

            struct SPwm {
                uint16_t on;
                uint16_t off;
            };

            Pca9685(Twi::TwiInterface &i2c, const uint8_t address = PCA9685_I2C_ADDRESS);

            ~Pca9685() = default;

            virtual void Reset(void) final override;

            virtual void Sleep(void) final override;

            virtual void WakeUp(void) final override;

            virtual void
            setOscillatorFrequency(const uint32_t frequency = EConstant::FREQUENCY_OSCILLATOR) final override;

            virtual void SetFrequency(const uint32_t frequency) final override;

            virtual void SetPwm(uint8_t num, uint16_t off) final override;

            Core::CoreStatus Initialize(void);

            void Update(const uint64_t currentTime);

        private:
            Twi::TwiInterface &mI2c;
            const uint8_t mAddress;
            uint32_t mInternalOscillatorFrequency;
            SPwm mPwm[EConstant::NB_LEDS];
        };
    }
}
