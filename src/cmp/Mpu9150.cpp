#include "Mpu9150.h"

#define NB_SAMPLES    20

Mpu9150::Mpu9150(Twi&i2c, const uint8_t address)
	: mI2c(i2c)
	, mAddress(address)
	, mAddressMag(AK8963_I2C_ADDRESS)
	, mAccOffset{0}
	, mGyrOffset{0}
	, mMagOffset{0}
	, mAccOffsetIndex(0)
	, mGyrOffsetIndex(0)
	, mMagOffsetIndex(0)
	, mAccOffsetMoy{0U}
	, mGyrOffsetMoy{0U}
	, mMagOffsetMoy{0U}
	, mAcc{0U}
	, mGyr{0U}
	, mMag{0U}
	, mTmp(0) {
}

void Mpu9150::Initialize (void) {
	uint8_t whoAmI = 0x00U;
	uint8_t reg    = 0x00U;

	// check device
	this->mI2c.ReadRegister(this->mAddress, (uint8_t)ERegister::WHO_AM_I, whoAmI);

	if (whoAmI == this->mAddress - 1) {
		this->mI2c.ReadRegister(this->mAddressMag, (uint8_t)ERegisterMag::WHO_AM_I, whoAmI);
		// if(whoAmI == 0x48)
		//{
		this->mI2c.WriteRegister(this->mAddressMag, (uint8_t)ERegisterMag::CNTL, 0x00);                                        // PowerDownMode
		this->mI2c.WriteRegister(this->mAddressMag, (uint8_t)ERegisterMag::CNTL, 0x0F);                                        // SelfTest
		this->mI2c.WriteRegister(this->mAddressMag, (uint8_t)ERegisterMag::CNTL, 0x00);                                        // PowerDownMode
		// }
		// setSleepEnabled
		this->mI2c.ReadRegister(this->mAddress, (uint8_t)ERegister::PWR_MGMT_1, reg);
		reg &= ~(1 << (uint8_t)ERegister::PWR1_SLEEP_BIT);
		this->mI2c.WriteRegister(this->mAddress, (uint8_t)ERegister::PWR_MGMT_1, reg);

		this->mI2c.WriteRegister(this->mAddress, (uint8_t)ERegister::I2C_MST_CTRL, 0x40);                                     // Wait for Data at Slave0
		this->mI2c.WriteRegister(this->mAddress, (uint8_t)ERegister::I2C_SLV0_ADDR, 0x8C);                                    // Set i2c address at slave0 at 0x0C
		this->mI2c.WriteRegister(this->mAddress, (uint8_t)ERegister::I2C_SLV0_REG, 0x02);                                     // Set where reading at slave 0 starts
		this->mI2c.WriteRegister(this->mAddress, (uint8_t)ERegister::I2C_SLV0_CTRL, 0x88);                                    // set offset at start reading and enable
		this->mI2c.WriteRegister(this->mAddress, (uint8_t)ERegister::I2C_SLV1_ADDR, 0x0C);                                    // set i2c address at slv1 at 0x0C
		this->mI2c.WriteRegister(this->mAddress, (uint8_t)ERegister::I2C_SLV1_REG, 0x0A);                                     // Set where reading at slave 1 starts
		this->mI2c.WriteRegister(this->mAddress, (uint8_t)ERegister::I2C_SLV1_CTRL, 0x81);                                    // Enable at set length to 1
		this->mI2c.WriteRegister(this->mAddress, (uint8_t)ERegister::I2C_SLV1_DO, 0x01);                                      // overrvride register
		this->mI2c.WriteRegister(this->mAddress, (uint8_t)ERegister::I2C_MST_DELAY_CTRL, 0x03);                               // set delay rate

		this->mI2c.WriteRegister(this->mAddress, (uint8_t)ERegister::I2C_SLV4_CTRL, 0x04);                                    // set i2c slv4 delay
		this->mI2c.WriteRegister(this->mAddress, (uint8_t)ERegister::I2C_SLV1_DO, 0x00);                                      // override register
		this->mI2c.WriteRegister(this->mAddress, (uint8_t)ERegister::USER_CTRL, 0x00);                                        // clear user setting
		this->mI2c.WriteRegister(this->mAddress, (uint8_t)ERegister::I2C_SLV1_DO, 0x01);                                      // override register
		this->mI2c.WriteRegister(this->mAddress, (uint8_t)ERegister::USER_CTRL, 0x20);                                        // enable master i2c mode
		this->mI2c.WriteRegister(this->mAddress, (uint8_t)ERegister::I2C_SLV4_CTRL, 0x13);                                    // disable slv4

		// setClockSource
		this->mI2c.ReadRegister(this->mAddress, (uint8_t)ERegister::PWR_MGMT_1, reg);
		reg |= ERegisterGyro::CLOCK_PLL_XGYRO;
		this->mI2c.WriteRegister(this->mAddress, (uint8_t)ERegister::PWR_MGMT_1, reg);

		// setFullScaleGyroRange
		this->mI2c.ReadRegister(this->mAddress, (uint8_t)ERegister::GYRO_CONFIG, reg);
		reg |= (ERegisterGyro::GYRO_FS_2000 << 3U);
		this->mI2c.WriteRegister(this->mAddress, (uint8_t)ERegister::GYRO_CONFIG, reg);

		// setFullScaleAccelRange
		this->mI2c.ReadRegister(this->mAddress, (uint8_t)ERegister::ACCEL_CONFIG, reg);
		reg |= (ERegisterAccel::ACCEL_FS_2 << 3U);
		this->mI2c.WriteRegister(this->mAddress, (uint8_t)ERegister::ACCEL_CONFIG, reg);

		// setBandWidthLowPassFilter
		this->mI2c.ReadRegister(this->mAddress, (uint8_t)ERegister::CONFIG, reg);
		reg |= ERegisterAccel::DLPF_BW_10;
		this->mI2c.WriteRegister(this->mAddress, (uint8_t)ERegister::CONFIG, reg);
	}
} // Mpu9150::Initialize

void Mpu9150::Update (const uint32_t currentTime) {
	ReadAcc();
	ReadGyr();
	ReadMag();
	ReadTemp();
}

Mpu9150::Vector3 Mpu9150::ReadAcc (void) {
	if (this->mI2c.ReadRegisters(this->mAddress, (uint8_t)ERegister::ACCEL_XOUT_H, (uint8_t *)&this->mAcc, 6U)) {
		this->mAcc.x = (int16_t)((this->mAcc.x << 8) | ((this->mAcc.x >> 8) & 0xFF)) - mAccOffset.x;
		this->mAcc.y = (int16_t)((this->mAcc.y << 8) | ((this->mAcc.y >> 8) & 0xFF)) - mAccOffset.y;
		this->mAcc.z = (int16_t)((this->mAcc.z << 8) | ((this->mAcc.z >> 8) & 0xFF)) - mAccOffset.z + ERegisterAccel::ACCEL_1G_16G;
	}

	if (mAccOffsetIndex < NB_SAMPLES) {
		mAccOffsetMoy.x += this->mAcc.x;
		mAccOffsetMoy.y += this->mAcc.y;
		mAccOffsetMoy.z += this->mAcc.z;

		if (mAccOffsetIndex == NB_SAMPLES - 1U) {
			mAccOffset.x = mAccOffsetMoy.x / NB_SAMPLES;
			mAccOffset.y = mAccOffsetMoy.y / NB_SAMPLES;
			mAccOffset.z = mAccOffsetMoy.z / NB_SAMPLES;
		}
		mAccOffsetIndex++;
	}
	return(this->mAcc);
}

Mpu9150::Vector3 Mpu9150::ReadGyr (void) {
	if (this->mI2c.ReadRegisters(this->mAddress, (uint8_t)ERegister::GYRO_XOUT_H, (uint8_t *)&this->mGyr, 6U)) {
		this->mGyr.x = (int16_t)((this->mGyr.x << 8) | ((this->mGyr.x >> 8) & 0xFF)) - mGyrOffset.x;
		this->mGyr.y = (int16_t)((this->mGyr.y << 8) | ((this->mGyr.y >> 8) & 0xFF)) - mGyrOffset.y;
		this->mGyr.z = (int16_t)((this->mGyr.z << 8) | ((this->mGyr.z >> 8) & 0xFF)) - mGyrOffset.z;
	}

	if (mGyrOffsetIndex < NB_SAMPLES) {
		mGyrOffsetMoy.x += this->mGyr.x;
		mGyrOffsetMoy.y += this->mGyr.y;
		mGyrOffsetMoy.z += this->mGyr.z;

		if (mGyrOffsetIndex == NB_SAMPLES - 1U) {
			mGyrOffset.x = mGyrOffsetMoy.x / NB_SAMPLES;
			mGyrOffset.y = mGyrOffsetMoy.y / NB_SAMPLES;
			mGyrOffset.z = mGyrOffsetMoy.z / NB_SAMPLES;
		}
		mGyrOffsetIndex++;
	}
	return(this->mGyr);
}

Mpu9150::Vector3 Mpu9150::ReadMag (void) {
	if (this->mI2c.ReadRegisters(this->mAddress, (uint8_t)ERegisterMag::XOUT_L, (uint8_t *)&this->mMag, 6U)) {
		this->mMag.x = -mMagOffset.x;
		this->mMag.y = -mMagOffset.y;
		this->mMag.z = -mMagOffset.z;
	}

	if (mMagOffsetIndex < NB_SAMPLES) {
		mMagOffsetMoy.x += this->mMag.x;
		mMagOffsetMoy.y += this->mMag.y;
		mMagOffsetMoy.z += this->mMag.z;

		if (mMagOffsetIndex == NB_SAMPLES - 1U) {
			mMagOffset.x = mMagOffsetMoy.x / NB_SAMPLES;
			mMagOffset.y = mMagOffsetMoy.y / NB_SAMPLES;
			mMagOffset.z = mMagOffsetMoy.z / NB_SAMPLES;
		}
		mMagOffsetIndex++;
	}
	return(this->mMag);
}

int16_t Mpu9150::ReadTemp (void) {
	uint8_t tmpL = 0U;
	uint8_t tmpH = 0U;

	this->mI2c.ReadRegister(this->mAddress, (uint8_t)ERegister::TEMP_OUT_H, tmpH);
	this->mI2c.ReadRegister(this->mAddress, (uint8_t)ERegister::TEMP_OUT_L, tmpL);
	this->mTmp = (int16_t)((tmpH << 8) | ((tmpL >> 8) & 0xFF));
	this->mTmp = (this->mTmp / 340.0) + 35U;
	return(this->mTmp);
}

bool Mpu9150::BuildFrameAll (Frame&response) {
	uint8_t params[] = { 0, 0, 0 };

	response.Build(EClusters::IMU, EImuCommands::ALL, (uint8_t *)&params, 3U);
	return(true);
}

bool Mpu9150::BuildFrameAcc (Frame&response) {
	Vector3 params = this->mAcc;

	response.Build(EClusters::IMU, EImuCommands::ACC, (uint8_t *)&params, 6U);
	return(true);
}

bool Mpu9150::BuildFrameGyr (Frame&response) {
	Vector3 params = this->mGyr;

	response.Build(EClusters::IMU, EImuCommands::GYR, (uint8_t *)&params, 6U);
	return(true);
}

bool Mpu9150::BuildFrameMag (Frame&response) {
	Vector3 params = this->mMag;

	response.Build(EClusters::IMU, EImuCommands::MAG, (uint8_t *)&params, 6U);
	return(true);
}

bool Mpu9150::BuildFrameTmp (Frame&response) {
	int16_t params = this->mTemp;

	response.Build(EClusters::IMU, EImuCommands::TMP, (uint8_t *)&params, 2U);
	return(true);
}
