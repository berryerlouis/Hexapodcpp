#pragma once

#include "../clu/Constants.h"
#include "../clu/Frame.h"
#include "../drv/Twi.h"

class Mpu9150 {
public:
	struct Vector3
	{
		int16_t x;
		int16_t y;
		int16_t z;
	};
	static const uint8_t MPU9150_I2C_ADDRESS = 0x69U;
	static const uint8_t AK8963_I2C_ADDRESS  = 0x0CU;

	Mpu9150(Twi &i2c, const uint8_t address = MPU9150_I2C_ADDRESS);
	~Mpu9150() = default;

	void Initialize(void);
	void Update(const uint32_t currentTime);

	bool BuildFrameAll(Frame &response);
	bool BuildFrameAcc(Frame &response);
	bool BuildFrameGyr(Frame &response);
	bool BuildFrameMag(Frame &response);
	bool BuildFrameTmp(Frame &response);

private:
	Vector3 ReadAcc(void);
	Vector3 ReadGyr(void);
	Vector3 ReadMag(void);
	int16_t ReadTemp(void);



	struct ERegister
	{
		static const uint8_t SELF_TEST_X        = 0x0D;
		static const uint8_t SELF_TEST_Y        = 0x0E;
		static const uint8_t SELF_TEST_Z        = 0x0F;
		static const uint8_t SELF_TEST_A        = 0x10;
		static const uint8_t SMPLRT_DIV         = 0x19;
		static const uint8_t CONFIG             = 0x1A;
		static const uint8_t GYRO_CONFIG        = 0x1B;
		static const uint8_t ACCEL_CONFIG       = 0x1C;
		static const uint8_t FF_THR             = 0x1D;
		static const uint8_t FF_DUR             = 0x1E;
		static const uint8_t MOT_THR            = 0x1F;
		static const uint8_t MOT_DUR            = 0x20;
		static const uint8_t ZRMOT_THR          = 0x21;
		static const uint8_t ZRMOT_DUR          = 0x22;
		static const uint8_t FIFO_EN            = 0x23;
		static const uint8_t I2C_MST_CTRL       = 0x24;
		static const uint8_t I2C_SLV0_ADDR      = 0x25;
		static const uint8_t I2C_SLV0_REG       = 0x26;
		static const uint8_t I2C_SLV0_CTRL      = 0x27;
		static const uint8_t I2C_SLV1_ADDR      = 0x28;
		static const uint8_t I2C_SLV1_REG       = 0x29;
		static const uint8_t I2C_SLV1_CTRL      = 0x2A;
		static const uint8_t I2C_SLV2_ADDR      = 0x2B;
		static const uint8_t I2C_SLV2_REG       = 0x2C;
		static const uint8_t I2C_SLV2_CTRL      = 0x2D;
		static const uint8_t I2C_SLV3_ADDR      = 0x2E;
		static const uint8_t I2C_SLV3_REG       = 0x2F;
		static const uint8_t I2C_SLV3_CTRL      = 0x30;
		static const uint8_t I2C_SLV4_ADDR      = 0x31;
		static const uint8_t I2C_SLV4_REG       = 0x32;
		static const uint8_t I2C_SLV4_DO        = 0x33;
		static const uint8_t I2C_SLV4_CTRL      = 0x34;
		static const uint8_t I2C_SLV4_DI        = 0x35;
		static const uint8_t I2C_MST_STATUS     = 0x36;
		static const uint8_t INT_PIN_CFG        = 0x37;
		static const uint8_t INT_ENABLE         = 0x38;
		static const uint8_t INT_STATUS         = 0x3A;
		static const uint8_t ACCEL_XOUT_H       = 0x3B;
		static const uint8_t ACCEL_XOUT_L       = 0x3C;
		static const uint8_t ACCEL_YOUT_H       = 0x3D;
		static const uint8_t ACCEL_YOUT_L       = 0x3E;
		static const uint8_t ACCEL_ZOUT_H       = 0x3F;
		static const uint8_t ACCEL_ZOUT_L       = 0x40;
		static const uint8_t TEMP_OUT_H         = 0x41;
		static const uint8_t TEMP_OUT_L         = 0x42;
		static const uint8_t GYRO_XOUT_H        = 0x43;
		static const uint8_t GYRO_XOUT_L        = 0x44;
		static const uint8_t GYRO_YOUT_H        = 0x45;
		static const uint8_t GYRO_YOUT_L        = 0x46;
		static const uint8_t GYRO_ZOUT_H        = 0x47;
		static const uint8_t GYRO_ZOUT_L        = 0x48;
		static const uint8_t EXT_SENS_DATA_00   = 0x49;
		static const uint8_t EXT_SENS_DATA_01   = 0x4A;
		static const uint8_t EXT_SENS_DATA_02   = 0x4B;
		static const uint8_t EXT_SENS_DATA_03   = 0x4C;
		static const uint8_t EXT_SENS_DATA_04   = 0x4D;
		static const uint8_t EXT_SENS_DATA_05   = 0x4E;
		static const uint8_t EXT_SENS_DATA_06   = 0x4F;
		static const uint8_t EXT_SENS_DATA_07   = 0x50;
		static const uint8_t EXT_SENS_DATA_08   = 0x51;
		static const uint8_t EXT_SENS_DATA_09   = 0x52;
		static const uint8_t EXT_SENS_DATA_10   = 0x53;
		static const uint8_t EXT_SENS_DATA_11   = 0x54;
		static const uint8_t EXT_SENS_DATA_12   = 0x55;
		static const uint8_t EXT_SENS_DATA_13   = 0x56;
		static const uint8_t EXT_SENS_DATA_14   = 0x57;
		static const uint8_t EXT_SENS_DATA_15   = 0x58;
		static const uint8_t EXT_SENS_DATA_16   = 0x59;
		static const uint8_t EXT_SENS_DATA_17   = 0x5A;
		static const uint8_t EXT_SENS_DATA_18   = 0x5B;
		static const uint8_t EXT_SENS_DATA_19   = 0x5C;
		static const uint8_t EXT_SENS_DATA_20   = 0x5D;
		static const uint8_t EXT_SENS_DATA_21   = 0x5E;
		static const uint8_t EXT_SENS_DATA_22   = 0x5F;
		static const uint8_t EXT_SENS_DATA_23   = 0x60;
		static const uint8_t MOT_DETECT_STATUS  = 0x61;
		static const uint8_t I2C_SLV0_DO        = 0x63;
		static const uint8_t I2C_SLV1_DO        = 0x64;
		static const uint8_t I2C_SLV2_DO        = 0x65;
		static const uint8_t I2C_SLV3_DO        = 0x66;
		static const uint8_t I2C_MST_DELAY_CTRL = 0x67;
		static const uint8_t SIGNAL_PATH_RESET  = 0x68;
		static const uint8_t MOT_DETECT_CTRL    = 0x69;
		static const uint8_t USER_CTRL          = 0x6A;
		static const uint8_t PWR_MGMT_1         = 0x6B;
		static const uint8_t PWR_MGMT_2         = 0x6C;
		static const uint8_t FIFO_COUNTH        = 0x72;
		static const uint8_t FIFO_COUNTL        = 0x73;
		static const uint8_t FIFO_R_W           = 0x74;
		static const uint8_t WHO_AM_I           = 0x75;
		static const uint8_t PWR1_CLKSEL_BIT    = 2;
		static const uint8_t PWR1_SLEEP_BIT     = 6;
	};

	struct ERegisterAccel
	{
		static const uint8_t  ACCEL_FS_2  = 0x00;
		static const uint8_t  ACCEL_FS_4  = 0x01;
		static const uint8_t  ACCEL_FS_8  = 0x02;
		static const uint8_t  ACCEL_FS_16 = 0x03;

		static const uint8_t  DLPF_BW_256 = 0x00;
		static const uint8_t  DLPF_BW_188 = 0x01;
		static const uint8_t  DLPF_BW_98  = 0x02;
		static const uint8_t  DLPF_BW_42  = 0x03;
		static const uint8_t  DLPF_BW_20  = 0x04;
		static const uint8_t  DLPF_BW_10  = 0x05;
		static const uint8_t  DLPF_BW_5   = 0x06;

		static const uint16_t ACCEL_1G_2G  = 0x3FFF;
		static const uint16_t ACCEL_1G_4G  = 0x1FFF;
		static const uint16_t ACCEL_1G_8G  = 0x0FFF;
		static const uint16_t ACCEL_1G_16G = 0x07FF;
	};

	struct ERegisterMag
	{
		static const uint8_t WHO_AM_I = 0x00;
		static const uint8_t INFO     = 0x01;
		static const uint8_t ST1      = 0x02;
		static const uint8_t XOUT_L   = 0x03;
		static const uint8_t XOUT_H   = 0x04;
		static const uint8_t YOUT_L   = 0x05;
		static const uint8_t YOUT_H   = 0x06;
		static const uint8_t ZOUT_L   = 0x07;
		static const uint8_t ZOUT_H   = 0x08;
		static const uint8_t ST2      = 0x09;
		static const uint8_t CNTL     = 0x0A;
		static const uint8_t ASTC     = 0x0C;
		static const uint8_t I2CDIS   = 0x0F;
		static const uint8_t ASAX     = 0x10;
		static const uint8_t ASAY     = 0x11;
		static const uint8_t ASAZ     = 0x12;
	};

	struct ERegisterGyro
	{
		static const uint8_t CLOCK_PLL_XGYRO = 0x01;
		static const uint8_t GYRO_FS_250     = 0x00;
		static const uint8_t GYRO_FS_500     = 0x01;
		static const uint8_t GYRO_FS_1000    = 0x02;
		static const uint8_t GYRO_FS_2000    = 0x03;

		static const uint8_t SELF_TEST_X_GYRO = 0x00;
		static const uint8_t SELF_TEST_Y_GYRO = 0x01;
		static const uint8_t SELF_TEST_Z_GYRO = 0x02;
	};

	Twi &mI2c;
	const uint8_t mAddress;
	const uint8_t mAddressMag;
	Vector3 mAccOffset;
	Vector3 mGyrOffset;
	Vector3 mMagOffset;
	uint8_t mAccOffsetIndex;
	uint8_t mGyrOffsetIndex;
	uint8_t mMagOffsetIndex;
	Vector3 mAccOffsetMoy;
	Vector3 mGyrOffsetMoy;
	Vector3 mMagOffsetMoy;
	Vector3 mAcc;
	Vector3 mGyr;
	Vector3 mMag;
	uint8_t mTmp;
};
