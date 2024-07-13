#include "Frame.h"

namespace Clusters {
Frame::Frame( void )
{
	Reset();
}

Core::CoreStatus Frame::Build ( uint8_t clusterId, uint8_t commandId )
{
	if ( nbParams > FRAME_MAX_PARAMS )
	{
		return ( Core::CoreStatus::CORE_ERROR );
	}
	this->clusterId = clusterId;
	this->commandId = commandId;
	this->nbParams  = 0U;

	return ( Core::CoreStatus::CORE_OK );
}

Core::CoreStatus Frame::Build ( uint8_t clusterId, uint8_t commandId, uint8_t *params, uint8_t nbParams )
{
	Core::CoreStatus success = Core::CoreStatus::CORE_ERROR;
	if ( nbParams < FRAME_MAX_PARAMS )
	{
		if ( ( params == nullptr ) && ( nbParams == 0U ) )
		{
			success = this->Build( clusterId, commandId );
		}
		else
		{
			this->clusterId = clusterId;
			this->commandId = commandId;
			this->nbParams  = nbParams;

			memset( (void *) this->params, 0U, FRAME_MAX_PARAMS );
			memcpy( (void *) this->params, params, nbParams );
			success = Core::CoreStatus::CORE_OK;
		}
	}
	return ( success );
}

void Frame::Reset ( void )
{
	this->clusterId = 0U;
	this->commandId = 0U;
	this->nbParams  = 0U;
	memset( (void *) this->params, 0U, FRAME_MAX_PARAMS );
}

void Frame::Set1ByteParam ( const uint8_t value )
{
	this->SetnBytesParam( 1U, (uint8_t *) &value );
}

void Frame::Set2BytesParam ( const uint16_t value )
{
	this->SetnBytesParam( 2U, (uint8_t *) &value );
}

void Frame::Set3BytesParam ( const uint32_t value )
{
	this->SetnBytesParam( 3U, (uint8_t *) &value );
}

void Frame::Set4BytesParam ( const uint32_t value )
{
	this->SetnBytesParam( 4U, (uint8_t *) &value );
}

void Frame::Set6BytesParam ( const uint64_t value )
{
	this->SetnBytesParam( 6U, (uint8_t *) &value );
}

void Frame::Set8BytesParam ( const uint64_t value )
{
	this->SetnBytesParam( 8U, (uint8_t *) &value );
}

void Frame::SetnBytesParam ( const size_t size, const uint8_t *value )
{
	memcpy( (void *) &this->params[this->nbParams], value, size );
	this->nbParams += size;
}

uint8_t Frame::Get1ByteParam ( const uint8_t index )
{
	return ( (uint8_t) this->params[index] );
}

uint16_t Frame::Get2BytesParam ( const uint8_t index )
{
	return ( (uint16_t) ( ( (uint16_t) this->params[index + 1U] << 8U ) \
								 | this->params[index] ) );
}

uint32_t Frame::Get3BytesParam ( const uint8_t index )
{
	return ( (uint32_t) ( ( (uint32_t) this->params[index + 2U] << 16U )					\
								 | (uint16_t) ( (uint16_t) this->params[index + 1U] << 8U )	\
								 | this->params[index] ) );
}

uint32_t Frame::Get4BytesParam ( const uint8_t index )
{
	return ( (uint32_t) ( ( (uint32_t) this->params[index + 3U] << 24U )	  \
								 | ( (uint32_t) this->params[index + 2U] << 16U ) \
								 | ( (uint16_t) this->params[index + 1U] << 8U )  \
								 | this->params[index] ) );
}

uint64_t Frame::Get6BytesParam ( const uint8_t index )
{
	return ( (uint64_t) ( ( (uint64_t) this->params[index + 5U] << 40U )	  \
								 | ( (uint64_t) this->params[index + 4U] << 32U ) \
								 | ( (uint32_t) this->params[index + 3U] << 24U ) \
								 | ( (uint32_t) this->params[index + 2U] << 16U ) \
								 | ( (uint16_t) this->params[index + 1U] << 8U )  \
								 | this->params[index] ) );
}

uint64_t Frame::Get8BytesParam ( const uint8_t index )
{
	return ( (uint64_t) ( ( (uint64_t) this->params[index + 7U] << 56U )	  \
								 | ( (uint64_t) this->params[index + 6U] << 48U ) \
								 | ( (uint64_t) this->params[index + 5U] << 40U ) \
								 | ( (uint64_t) this->params[index + 4U] << 32U ) \
								 | ( (uint32_t) this->params[index + 3U] << 24U ) \
								 | ( (uint32_t) this->params[index + 2U] << 16U ) \
								 | ( (uint16_t) this->params[index + 1U] << 8U )  \
								 | this->params[index] ) );
}
}
