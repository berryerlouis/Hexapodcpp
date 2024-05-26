#include "Adc.h"

Adc::Adc(const uint8_t gpio)
    : mIo(gpio)
{
}

void Adc::Initialize(void)
{
    pinMode(this->mIo, INPUT);
}

void Adc::Update(const uint32_t currentTime)
{
}

uint16_t Adc::Read()
{
    return (uint16_t)(100.0 * ADC_VOLT(analogRead(this->mIo) / BRIDGE_DIVIDER));
}