#pragma once
#include <gmock/gmock.h>
#include "../../../src/Driver/Twi/TwiInterface.h"

namespace Driver
{
	namespace Twi
	{
		class MockTwi : public TwiInterface {
		public:
			MOCK_METHOD0(Initialize, Core::CoreStatus( void ));
			MOCK_METHOD1(Update, void( const uint64_t ));

			MOCK_METHOD2(SendCommand, bool( const uint8_t, const uint8_t ));
			MOCK_METHOD3(ReadRegister, bool(const uint8_t address, const uint8_t reg, uint8_t & data));
			MOCK_METHOD3(ReadRegister16Bits, bool(const uint8_t address, const uint8_t reg, uint16_t & data));
			MOCK_METHOD3(ReadRegister24Bits, bool(const uint8_t address, const uint8_t reg, uint32_t & data));
			MOCK_METHOD3(ReadRegister32Bits, bool(const uint8_t address, const uint8_t reg, uint32_t & data));
			MOCK_METHOD4(ReadRegisters,
			             bool(const uint8_t address, const uint8_t reg, uint8_t * data, const uint8_t length));
			MOCK_METHOD3(WriteRegister, bool(const uint8_t address, const uint8_t reg, uint8_t data));
			MOCK_METHOD3(WriteRegister16Bits, bool(const uint8_t address, const uint8_t reg, uint16_t & data));
			MOCK_METHOD3(WriteRegister32Bits, bool(const uint8_t address, const uint8_t reg, uint32_t & data));
			MOCK_METHOD4(WriteRegisters,
			             bool(const uint8_t address, const uint8_t reg, uint8_t * data, const uint8_t length));
		};
	}
}
