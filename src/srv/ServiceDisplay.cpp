#include "ServiceDisplay.h"
#include "../cor/Bitmaps.h"
#include <stdlib.h>

ServiceDisplay::ServiceDisplay( Ssd1306Interface &ssd1306 )
	: Service( 5U )
	, mSsd1306( ssd1306 )
	, mBmpBatteryLevel{.bmp  = (uint8_t *) Bitmaps::bitmapBattery0, .width = 16U, .height = 7U }
	, mBmpCommunication{.bmp = (uint8_t *) Bitmaps::bitmapCommunication, .width = 16U, .height = 8U }
	, mBmpProximity{.bmp     = (uint8_t *) Bitmaps::bitmapArrowCenter, .width = 16U, .height = 6U }
	, mPreviousTime( 0UL )
	, mToggleCommunicationBmp( false )
{
}

Core::CoreStatus ServiceDisplay::Initialize ( void )
{
	this->mSsd1306.Initialize();
	this->DisplayBackground();
	return ( Core::CoreStatus::CORE_OK );
}

void ServiceDisplay::Update ( const uint64_t currentTime )
{
	if ( ( currentTime - this->mPreviousTime ) >= 500UL )
	{
		this->DisplayCommunicationBmp();
		mPreviousTime = currentTime;
	}
	this->mSsd1306.Update( currentTime );
}

void ServiceDisplay::DisplayBackground ( void )
{
	this->mSsd1306.DrawLine( 0, 10U, SCREEN_WIDTH, 10U, Ssd1306Interface::Color::COLOR_WHITE );
}

void ServiceDisplay::DisplayCommunicationBmp ( void )
{
	if ( this->mToggleCommunicationBmp == true )
	{
		this->mSsd1306.DrawBitmap(
			&this->mBmpCommunication,
			SCREEN_WIDTH - this->mBmpCommunication.width,
			0U,
			Ssd1306Interface::Color::COLOR_WHITE );
		this->mToggleCommunicationBmp = false;
	}
	else
	{
		this->mSsd1306.EraseArea(
			20U,
			0U,
			SCREEN_WIDTH,
			8U );
		this->mToggleCommunicationBmp = true;
	}
}

void ServiceDisplay::DisplayBatteryLevel ( Component::BatteryInterface::BatteryState state )
{
	if ( state == Component::BatteryInterface::BatteryState::WARNING )
	{
		this->mBmpBatteryLevel.bmp = (uint8_t *) Bitmaps::bitmapBattery50;
	}
	else if ( state == Component::BatteryInterface::BatteryState::NOMINAL )
	{
		this->mBmpBatteryLevel.bmp = (uint8_t *) Bitmaps::bitmapBattery100;
	}
	else
	{
		this->mBmpBatteryLevel.bmp = (uint8_t *) Bitmaps::bitmapBattery0;
	}
	this->mSsd1306.DrawBitmap( &this->mBmpBatteryLevel, 0, 0, Ssd1306Interface::Color::COLOR_WHITE );
}

void ServiceDisplay::DisplayProximitySensor ( Component::SensorProximityInterface::SensorsId sensor )
{
	if ( sensor == Component::SensorProximityInterface::SensorsId::SRF_LEFT )
	{
		this->mBmpProximity.bmp = (uint8_t *) Bitmaps::bitmapArrowLeft;

		this->mSsd1306.DrawBitmap(
			&this->mBmpProximity,
			( SCREEN_WIDTH / 2U ) - ( this->mBmpProximity.width / 2U ) - this->mBmpProximity.width,
			0U,
			Ssd1306Interface::Color::COLOR_WHITE );
	}
	if ( sensor == Component::SensorProximityInterface::SensorsId::VLX )
	{
		this->mBmpProximity.bmp = (uint8_t *) Bitmaps::bitmapArrowUp;
		this->mSsd1306.DrawBitmap(
			&this->mBmpProximity,
			( SCREEN_WIDTH / 2U ) - ( this->mBmpProximity.width / 2U ),
			0U,
			Ssd1306Interface::Color::COLOR_WHITE );
	}
	if ( sensor == Component::SensorProximityInterface::SensorsId::SRF_RIGHT )
	{
		this->mBmpProximity.bmp = (uint8_t *) Bitmaps::bitmapArrowRight;
		this->mSsd1306.DrawBitmap(
			&this->mBmpProximity,
			( SCREEN_WIDTH / 2U ) + ( this->mBmpProximity.width / 2U ),
			0U,
			Ssd1306Interface::Color::COLOR_WHITE );
	}
}
