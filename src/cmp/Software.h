#pragma once

#include "SoftwareInterface.h"

namespace Component {
class Software : public SoftwareInterface {
public:

	Software();
	~Software() = default;

	virtual Core::CoreStatus Initialize( void ) final override;
	virtual void Update( const uint32_t currentTime ) final override;

	virtual Version GetVersion( void ) final override;
	virtual void CheckTimingLoop( const uint32_t time ) final override;

	virtual uint32_t GetMinTime( void ) final override;
	virtual uint32_t GetMaxTime( void ) final override;
	virtual void ResetTiming( void ) final override;

private:
	uint32_t mMinTime;
	uint32_t mMaxTime;
	void SetMinTime( const uint32_t time );
	void SetMaxTime( const uint32_t time );
};
}
