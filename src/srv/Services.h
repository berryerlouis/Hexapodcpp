#pragma once

#include "../srv/Service.h"
#include "../srv/ServiceControl.h"
#include "../srv/ServiceProximity.h"
#include "../srv/ServiceOrientation.h"
#include "../srv/ServiceBattery.h"

class Services
{
public:
  static const uint8_t NB_SERVICES = 4U;
  Services(ServiceControl &serviceControl, ServiceProximity &serviceProximity, ServiceOrientation &serviceOrientation, ServiceBattery &serviceBattery) : mServices{
                                                                                                                                                             &serviceControl,
                                                                                                                                                             &serviceProximity,
                                                                                                                                                             &serviceOrientation,
                                                                                                                                                             &serviceBattery}
  {
  }

  ~Services() = default;

  void Initialize(void)
  {
    for (size_t i = 0; i < NB_SERVICES; i++)
    {
      mServices[i]->Initialize();
    }
  }

  void Update(const uint32_t currentTime)
  {
    for (size_t i = 0; i < NB_SERVICES; i++)
    {
      if (mServices[i]->IsTimeToUpdate(currentTime))
      {
        mServices[i]->Update(currentTime);
        mServices[i]->SetNewUpdateTime(currentTime);
      }
    }
  }

  Service *GetService(const uint8_t serviceId) const
  {
    return mServices[serviceId];
  }

private:
  Service *mServices[NB_SERVICES];
};