#pragma once

#include <stdint.h>

namespace Misc {
namespace Buffer {
const uint8_t BUFFER_MAX_SIZE = 100U;

class Buffer
{
public:
    Buffer( void );
    ~Buffer() = default;
    uint8_t GetLength( void ) const;
    void Push( const volatile uint8_t &item );
    uint8_t Pop( void );

private:
    volatile uint8_t mBuffer[BUFFER_MAX_SIZE];
    volatile uint8_t mHead;
    volatile uint8_t mTail;
    volatile bool mIsEmpty;
};
}
}
