#pragma once

#include <stdint.h>
#include <stddef.h>
#include "SEvent.h"

namespace Service
{
    namespace Event
    {
        constexpr size_t BUFFER_MAX_SIZE = 30U;

        class BufferEvent {
        public:
            BufferEvent(const size_t size = BUFFER_MAX_SIZE);

            ~BufferEvent() = default;

            void Push(const SEvent &item);

            bool Pop(SEvent &item);

            uint8_t GetLength(void) const;

            bool IsEmpty() const;

            bool IsFull() const;

        private:
            SEvent mBuffer[BUFFER_MAX_SIZE];
            const size_t mMaxSize;
            size_t mHead;
            size_t mTail;
            bool mIsFull;
        };
    }
}
