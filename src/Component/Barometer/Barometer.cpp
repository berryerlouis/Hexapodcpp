#include "Barometer.h"

namespace Component
{
	namespace Barometer
	{
		Barometer::Barometer(Driver::Twi::TwiInterface &i2c, const uint8_t address,
		                     const BarometerResolution resolution)
			: mI2c(i2c)
			  , mAddress(address)
			  , mResolution(resolution)
			  , mProm{0U}
			  , mRawTemp(0U)
			  , mRawPressure(0U)
			  , mTemp(0U)
			  , mPressure(0U)
			  , mAltitude(0U) {
		}

		Core::CoreStatus Barometer::Initialize(void) {
			this->ReadProm();
			return Core::CORE_OK;
		}

		void Barometer::Update(const uint64_t currentTime) {
			(void) currentTime;
			int32_t deltaTime = 0;
			static uint64_t lastTime = false;
			static bool toggleReadTempPresure = false;

			if(currentTime > lastTime + 10000U)
			{
				lastTime = currentTime;
				if (toggleReadTempPresure == false) {
					toggleReadTempPresure = true;
					this->mRawTemp = this->ReadRawTemp();
					this->mI2c.SendCommand(this->mAddress, MS5611_CMD_CONV_D1 + this->mResolution);
				} else {
					toggleReadTempPresure = false;
					this->mRawPressure = this->ReadRawPressure();
					this->mI2c.SendCommand(this->mAddress, MS5611_CMD_CONV_D2 + this->mResolution);
				}
				deltaTime = this->mRawTemp - ((uint32_t) this->mProm[4U] * 256);
				const int32_t temp = 2000 + (deltaTime * static_cast<int64_t>(this->mProm[5U])) / 8388608.0F;
				this->mTemp = temp;

				int64_t offset = (static_cast<int64_t>(this->mProm[1U]) * 65536) + (
										   static_cast<int64_t>(this->mProm[3U]) * deltaTime) / 128.0F;
				int64_t sensibility = (static_cast<int64_t>(this->mProm[0U]) * 32768) + (
												static_cast<int64_t>(this->mProm[2U]) * deltaTime) / 256.0F;
				int64_t offset2 = 0;
				int64_t sensibility2 = 0;
				if (temp < 2000)
				{
					offset2 = 5 * ((temp - 2000) * (temp - 2000)) / 2.0F;
					sensibility2 = 5 * ((temp - 2000) * (temp - 2000)) / 4.0F;
				}

				if (temp < -1500)
				{
					offset2 = offset2 + 7 * ((temp + 1500) * (temp + 1500));
					sensibility2 = sensibility2 + 11 * ((temp + 1500) * (temp + 1500)) / 2.0F;
				}

				offset -= offset2;
				sensibility -= sensibility2;
				this->mPressure = (this->mRawPressure * sensibility / 2097152.0F - offset) / 32768.0F;
			}
		}

		void Barometer::Reset(void) const {
			this->mI2c.WriteRegister(this->mAddress, MS5611_CMD_RESET, 0U);
		}

		void Barometer::ReadProm(void) {
			for (size_t offset = 0; offset < 6; offset++) {
				uint16_t data;
				this->mI2c.ReadRegister16Bits(this->mAddress, MS5611_CMD_READ_PROM + (offset * 2U), data);
				this->mProm[offset] = data;
			}
		}

		uint32_t Barometer::ReadRawTemp(void) const {
			uint32_t rawTemp = 0U;
			this->mI2c.ReadRegister24Bits(this->mAddress, MS5611_CMD_ADC_READ, rawTemp);
			return rawTemp;
		}

		uint32_t Barometer::ReadRawPressure(void) const {
			uint32_t rawPressure = 0U;
			this->mI2c.ReadRegister24Bits(this->mAddress, MS5611_CMD_ADC_READ, rawPressure);
			return rawPressure;
		}
	}
}
