#include "../srv/Services.h"

Services::Services(ServiceControl &serviceControl,
						 ServiceProximity &serviceProximity,
						 ServiceOrientation &serviceOrientation,
						 ServiceBattery &serviceBattery)
	: mServices{
					&serviceControl,
					&serviceProximity,
					&serviceOrientation,
					&serviceBattery
					}
{
}

bool Services::Initialize (void)
{
	bool success = false;
	for (size_t i = 0U; i < NB_SERVICES; i++)
	{
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
