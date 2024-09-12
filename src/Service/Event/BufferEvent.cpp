#include "BufferEvent.h"

namespace Service
{
    namespace Event
    {
        BufferEvent::BufferEvent(const size_t size)
            : mBuffer{}
              , mMaxSize(size)
              , mHead(0U)
              , mTail(0U)
              , mIsFull(false) {
        }

        void BufferEvent::Push(const SEvent &item) {
            this->mBuffer[this->mHead] = item;
            if (this->mIsFull) {
                this->mTail = (this->mTail + 1U) % this->mMaxSize;
            }
            this->mHead = (this->mHead + 1U) % this->mMaxSize;
            this->mIsFull = this->mHead == this->mTail;
        }

        bool BufferEvent::Pop(SEvent &item) {
            if (this->IsEmpty()) {
                return false;
            }
            item = this->mBuffer[this->mTail];
            this->mIsFull = false;
            this->mTail = (this->mTail + 1U) % this->mMaxSize;
            return true;
        }

        uint8_t BufferEvent::GetLength() const {
            if (this->IsEmpty()) {
                return (0U);
            }
            if (this->mHead >= this->mTail) {
                return (this->mHead - this->mTail);
            }
            return (BUFFER_MAX_SIZE - (this->mTail - this->mHead));
        }

        bool BufferEvent::IsEmpty() const {
            return (!this->mIsFull && (this->mHead == this->mTail));
        }

        bool BufferEvent::IsFull() const {
            return this->mIsFull;
        }
    }
}
