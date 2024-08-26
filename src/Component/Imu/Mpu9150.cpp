#include "Mpu9150.h"

namespace Component {
namespace Imu {
#define NB_SAMPLES    20

Mpu9150::Mpu9150( Twi::TwiInterface &i2c, const uint8_t address )
	: mI2c( i2c )
	, mAddress( address )
	, mAddressMag( AK8963_I2C_ADDRESS )
	, mAccOffset{ 0, 0, 0 }
	, mGyrOffset{ 0, 0, 0 }
	, mMagOffset{ 0, 0, 0 }
	, mAccOffsetIndex( 0U )
	, mGyrOffsetIndex( 0U )
	, mMagOffsetIndex( 0U )
	, mAccOffsetMoy{ 0, 0, 0 }
	, mGyrOffsetMoy{ 0, 0, 0 }
	, mMagOffsetMoy{ 0, 0, 0 }
	, mAcc{ 0, 0, 0 }
	, mGyr{ 0, 0, 0 }
	, mMag{ 0, 0, 0 }
	, mTmp( 0U )
{
}

Core::CoreStatus Mpu9150::Initialize ( void )
{
	uint8_t          whoAmI  = 0x00U;
	uint8_t          reg     = 0x00U;
	Core::CoreStatus success = Core::CoreStatus::CORE_ERROR;

	// check device
	this->mI2c.ReadRegister( this->mAddress, (uint8_t) ERegister::WHO_AM_I, whoAmI );

	if ( whoAmI == this->mAddress - 1 )
	{
		this->mI2c.ReadRegister( this->mAddressMag, (uint8_t) ERegisterMag::WHO_AM_I, whoAmI );
		// if(whoAmI == 0x48)
		//{
		this->mI2c.WriteRegister( this->mAddressMag, (uint8_t) ERegisterMag::CNTL, 0x00 );
		this->mI2c.WriteRegister( this->mAddressMag, (uint8_t) ERegisterMag::CNTL, 0x0F );
		this->mI2c.WriteRegister( this->mAddressMag, (uint8_t) ERegisterMag::CNTL, 0x00 );
		// }
		// setSleepEnabled
		this->mI2c.ReadRegister( this->mAddress, (uint8_t) ERegister::PWR_MGMT_1, reg );
		reg &= ~( 1 << (uint8_t) ERegister::PWR1_SLEEP_BIT );
		this->mI2c.WriteRegister( this->mAddress, (uint8_t) ERegister::PWR_MGMT_1, reg );

		this->mI2c.WriteRegister( this->mAddress, (uint8_t) ERegister::I2C_MST_CTRL, 0x40 );
		this->mI2c.WriteRegister( this->mAddress, (uint8_t) ERegister::I2C_SLV0_ADDR, 0x8C );
		this->mI2c.WriteRegister( this->mAddress, (uint8_t) ERegister::I2C_SLV0_REG, 0x02 );
		this->mI2c.WriteRegister( this->mAddress, (uint8_t) ERegister::I2C_SLV0_CTRL, 0x88 );
		this->mI2c.WriteRegister( this->mAddress, (uint8_t) ERegister::I2C_SLV1_ADDR, 0x0C );
		this->mI2c.WriteRegister( this->mAddress, (uint8_t) ERegister::I2C_SLV1_REG, 0x0A );
		this->mI2c.WriteRegister( this->mAddress, (uint8_t) ERegister::I2C_SLV1_CTRL, 0x81 );
		this->mI2c.WriteRegister( this->mAddress, (uint8_t) ERegister::I2C_SLV1_DO, 0x01 );
		this->mI2c.WriteRegister( this->mAddress, (uint8_t) ERegister::I2C_MST_DELAY_CTRL, 0x03 );

		this->mI2c.WriteRegister( this->mAddress, (uint8_t) ERegister::I2C_SLV4_CTRL, 0x04 );
		this->mI2c.WriteRegister( this->mAddress, (uint8_t) ERegister::I2C_SLV1_DO, 0x00 );
		this->mI2c.WriteRegister( this->mAddress, (uint8_t) ERegister::USER_CTRL, 0x00 );
		this->mI2c.WriteRegister( this->mAddress, (uint8_t) ERegister::I2C_SLV1_DO, 0x01 );
		this->mI2c.WriteRegister( this->mAddress, (uint8_t) ERegister::USER_CTRL, 0x20 );
		this->mI2c.WriteRegister( this->mAddress, (uint8_t) ERegister::I2C_SLV4_CTRL, 0x13 );

		// setClockSource
		this->mI2c.ReadRegister( this->mAddress, (uint8_t) ERegister::PWR_MGMT_1, reg );
		reg |= ERegisterGyro::CLOCK_PLL_XGYRO;
		this->mI2c.WriteRegister( this->mAddress, (uint8_t) ERegister::PWR_MGMT_1, reg );

		// setFullScaleGyroRange
		this->mI2c.ReadRegister( this->mAddress, (uint8_t) ERegister::GYRO_CONFIG, reg );
		reg |= ( ERegisterGyro::GYRO_FS_2000 << 3U );
		this->mI2c.WriteRegister( this->mAddress, (uint8_t) ERegister::GYRO_CONFIG, reg );

		// setFullScaleAccelRange
		this->mI2c.ReadRegister( this->mAddress, (uint8_t) ERegister::ACCEL_CONFIG, reg );
		reg |= ( ERegisterAccel::ACCEL_FS_2 << 3U );
		this->mI2c.WriteRegister( this->mAddress, (uint8_t) ERegister::ACCEL_CONFIG, reg );

		// setBandWidthLowPassFilter
		this->mI2c.ReadRegister( this->mAddress, (uint8_t) ERegister::CONFIG, reg );
		reg |= ERegisterAccel::DLPF_BW_10;
		this->mI2c.WriteRegister( this->mAddress, (uint8_t) ERegister::CONFIG, reg );
		success = Core::CoreStatus::CORE_OK;
	}
	return ( success );
}

void Mpu9150::Update ( const uint64_t currentTime )
{
	(void) currentTime;
	UpdateAcc();
	UpdateGyr();
	UpdateMag();
	UpdateTemp();
}

Vector3 Mpu9150::UpdateAcc ( void )
{
	if ( this->mI2c.ReadRegisters( this->mAddress, (uint8_t) ERegister::ACCEL_XOUT_H, (uint8_t *) &this->mAcc, 6U ) )
	{
		this->mAcc.x = (int16_t) ( ( this->mAcc.x << 8 ) | ( ( this->mAcc.x >> 8 ) & 0xFF ) ) - mAccOffset.x;
		this->mAcc.y = (int16_t) ( ( this->mAcc.y << 8 ) | ( ( this->mAcc.y >> 8 ) & 0xFF ) ) - mAccOffset.y;
		this->mAcc.z = (int16_t) ( ( this->mAcc.z << 8 ) | ( ( this->mAcc.z >> 8 ) & 0xFF ) ) - mAccOffset.z + ERegisterAccel::ACCEL_1G_16G;
	}

	if ( mAccOffsetIndex < NB_SAMPLES )
	{
		mAccOffsetMoy.x += this->mAcc.x;
		mAccOffsetMoy.y += this->mAcc.y;
		mAccOffsetMoy.z += this->mAcc.z;

		if ( mAccOffsetIndex == NB_SAMPLES - 1U )
		{
			mAccOffset.x = mAccOffsetMoy.x / NB_SAMPLES;
			mAccOffset.y = mAccOffsetMoy.y / NB_SAMPLES;
			mAccOffset.z = mAccOffsetMoy.z / NB_SAMPLES;
		}
		mAccOffsetIndex++;
	}
	return ( this->mAcc );
}

Vector3 Mpu9150::UpdateGyr ( void )
{
	if ( this->mI2c.ReadRegisters( this->mAddress, (uint8_t) ERegister::GYRO_XOUT_H, (uint8_t *) &this->mGyr, 6U ) )
	{
		this->mGyr.x = (int16_t) ( ( this->mGyr.x << 8 ) | ( ( this->mGyr.x >> 8 ) & 0xFF ) ) - mGyrOffset.x;
		this->mGyr.y = (int16_t) ( ( this->mGyr.y << 8 ) | ( ( this->mGyr.y >> 8 ) & 0xFF ) ) - mGyrOffset.y;
		this->mGyr.z = (int16_t) ( ( this->mGyr.z << 8 ) | ( ( this->mGyr.z >> 8 ) & 0xFF ) ) - mGyrOffset.z;
	}

	if ( mGyrOffsetIndex < NB_SAMPLES )
	{
		mGyrOffsetMoy.x += this->mGyr.x;
		mGyrOffsetMoy.y += this->mGyr.y;
		mGyrOffsetMoy.z += this->mGyr.z;

		if ( mGyrOffsetIndex == NB_SAMPLES - 1U )
		{
			mGyrOffset.x = mGyrOffsetMoy.x / NB_SAMPLES;
			mGyrOffset.y = mGyrOffsetMoy.y / NB_SAMPLES;
			mGyrOffset.z = mGyrOffsetMoy.z / NB_SAMPLES;
		}
		mGyrOffsetIndex++;
	}
	return ( this->mGyr );
}

Vector3 Mpu9150::UpdateMag ( void )
{
	if ( this->mI2c.ReadRegisters( this->mAddress, (uint8_t) ERegisterMag::XOUT_L, (uint8_t *) &this->mMag, 6U ) )
	{
		this->mMag.x = -mMagOffset.x;
		this->mMag.y = -mMagOffset.y;
		this->mMag.z = -mMagOffset.z;
	}

	if ( mMagOffsetIndex < NB_SAMPLES )
	{
		mMagOffsetMoy.x += this->mMag.x;
		mMagOffsetMoy.y += this->mMag.y;
		mMagOffsetMoy.z += this->mMag.z;

		if ( mMagOffsetIndex == NB_SAMPLES - 1U )
		{
			mMagOffset.x = mMagOffsetMoy.x / NB_SAMPLES;
			mMagOffset.y = mMagOffsetMoy.y / NB_SAMPLES;
			mMagOffset.z = mMagOffsetMoy.z / NB_SAMPLES;
		}
		mMagOffsetIndex++;
	}
	return ( this->mMag );
}

int16_t Mpu9150::UpdateTemp ( void )
{
	uint8_t tmpL = 0U;
	uint8_t tmpH = 0U;

	this->mI2c.ReadRegister( this->mAddress, (uint8_t) ERegister::TEMP_OUT_H, tmpH );
	this->mI2c.ReadRegister( this->mAddress, (uint8_t) ERegister::TEMP_OUT_L, tmpL );
	this->mTmp = (int16_t) ( ( tmpH << 8 ) | ( ( tmpL >> 8 ) & 0xFF ) );
	this->mTmp = ( this->mTmp / 340.0 ) + 35U;
	return ( this->mTmp );
}
}
}
