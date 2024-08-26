#include "Buffer.h"
#include <math.h>

namespace Misc {
namespace Buffer {
Buffer::Buffer()
	: mBuffer{ 0U }
	, mHead( 0U )
	, mTail( 0U )
	, mIsEmpty( true )
{
}

uint8_t Buffer::GetLength () const
{
	if ( this->mIsEmpty )
	{
		return ( 0U );
	}
	if ( this->mHead >= this->mTail )
	{
		return ( this->mHead - this->mTail );
	}
	else
	{
		return ( BUFFER_MAX_SIZE - ( this->mTail - this->mHead ) );
	}
}

void Buffer::Push ( const volatile uint8_t &item )
{
	this->mBuffer[this->mHead] = item;
	if ( ++this->mHead >= BUFFER_MAX_SIZE )
	{
		this->mHead = 0U;
	}
	this->mIsEmpty = false;
}

uint8_t Buffer::Pop ()
{
	uint8_t datum = 0xFFU;

	if ( !this->mIsEmpty )
	{
		datum = this->mBuffer[this->mTail];
		if ( ++this->mTail >= BUFFER_MAX_SIZE )
		{
			this->mTail = 0U;
		}
		if ( this->mTail == this->mHead )
		{
			this->mIsEmpty = true;
		}
	}
	return ( datum );
}
}
}
