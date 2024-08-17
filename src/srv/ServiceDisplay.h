#pragma once

#include "../cmp/Ssd1306Interface.h"
#include "../cmp/Ssd1306.h"
#include "../cmp/SensorProximityInterface.h"
#include "Service.h"

using namespace Component;
class ServiceDisplay : public Service {
public:
	ServiceDisplay( Ssd1306Interface &ssd1306 );
	~ServiceDisplay() = default;

	virtual Core::CoreStatus Initialize( void ) final override;
	virtual void Update( const uint64_t currentTime ) final override;

	void DisplayBackground( void );
	void DisplayCommunicationBmp( void );
	void DisplayBatteryLevel( Component::BatteryState state );
	void DisplayProximitySensor( Component::SensorProximityInterface::SensorsId sensor );

protected:
	Ssd1306Interface & mSsd1306;

	Ssd1306Interface::SBitmap mBmpBatteryLevel;
	Ssd1306Interface::SBitmap mBmpCommunication;
	Ssd1306Interface::SBitmap mBmpProximity;
	uint64_t mPreviousTime;
	uint32_t mToggleCommunicationBmp;
};
