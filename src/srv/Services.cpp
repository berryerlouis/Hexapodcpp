#include "../srv/Services.h"

Services::Services(
	CommunicationInterface &communication,
	ServiceControl &serviceControl,
	ServiceProximity &serviceProximity,
	ServiceOrientation &serviceOrientation,
	ServiceBattery &serviceBattery)
	: mCommunication(communication)
	, mServices{
					&serviceControl,
					&serviceProximity,
					&serviceOrientation,
					&serviceBattery
					}
{
}

Core::CoreStatus Services::Initialize (void)
{
	Core::CoreStatus success = Core::CoreStatus::CORE_ERROR;

	for (size_t i = 0U; i < NB_SERVICES; i++)
	{
		mServices[i]->SetComComponent(this);
		success = mServices[i]->Initialize();
		if (success == false)
		{
			break;
		}
	}
	return (success);
}

void Services::Update (const uint32_t currentTime)
{
	for (size_t i = 0U; i < NB_SERVICES; i++)
	{
		if (mServices[i]->IsTimeToUpdate(currentTime) )
		{
			mServices[i]->Update(currentTime);
			mServices[i]->SetNewUpdateTime(currentTime);
		}
	}
}

Service *Services::GetService (const uint8_t serviceId) const
{
	return (mServices[serviceId]);
}

void Services::SendFrame (Frame &message) const
{
	this->mCommunication.Send(message);
}
