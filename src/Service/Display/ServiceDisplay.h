#pragma once

#include "../../Component/Display/Ssd1306Interface.h"
#include "../../Component/Proximity/SensorProximityInterface.h"
#include "../../Component/Battery/BatteryInterface.h"
#include "../../Misc/Bitmap/Bitmaps.h"
#include "../Service.h"

namespace Service {
namespace Display {
using namespace Component::Display;
using namespace Component::Proximity;
using namespace Component::Battery;
using namespace Misc::Bitmap;

using namespace Component;
class ServiceDisplay : public Service, BatteryObserverInterface, SensorProximityObserverInterface {
public:
    ServiceDisplay( Ssd1306Interface &ssd1306, BatteryInterface &batteryInterface, SensorProximityMultipleInterface &proximity );
    ~ServiceDisplay() = default;

    virtual Core::CoreStatus Initialize( void ) final override;
    virtual void Update( const uint64_t currentTime ) final override;

    void DisplayBackground( void );
    void DisplayCommunicationBmp( void );
    void DisplayBatteryLevel( BatteryState state );
    void DisplayProximitySensor( SensorsId sensor );

    virtual void UpdatedBatteryState( const BatteryState &batteryState ) final override;
    virtual void Detect( const SensorsId &sensorId ) final override;
    virtual void NoDetect( const SensorsId &sensorId ) final override;

protected:
    Ssd1306Interface & mSsd1306;
    BatteryInterface & mBatteryInterface;
    SensorProximityMultipleInterface & mProximity;

    Bitmaps::SBitmap mBmpBatteryLevel;
    Bitmaps::SBitmap mBmpCommunication;
    Bitmaps::SBitmap mBmpProximity;
    uint64_t mPreviousTime;
    uint32_t mToggleCommunicationBmp;
};
}
}
