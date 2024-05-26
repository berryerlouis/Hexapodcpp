#pragma once

#include "Constants.h"
#include "Cluster.h"
#include "../cmp/Servos.h"

class ClusterServo : public Cluster
{

public:
  ClusterServo(Servos &servos)
      : Cluster(SERVO),
        mServos(servos)
  {
  }
  ~ClusterServo() = default;

  bool Execute(Frame &request, Frame &response) final override
  {
    bool success = false;
    uint8_t servoId = 0xFFU;
    EServoCommands command = (EServoCommands)request.commandId;

    if (command != EServoCommands::GET_ALL)
    {
      if (request.nbParams == 0U)
      {
        return false;
      }
      servoId = request.params[0U];
      if (servoId > NB_SERVOS)
      {
        return false;
      }
    }

    switch (command)
    {
    case EServoCommands::GET_ALL:
      if (request.nbParams == 0U)
      {
        return mServos.BuildFrameAllAngle(response);
      }

      break;
    case EServoCommands::GET_ANGLE:
      if (request.nbParams == 1U)
      {
        return mServos.BuildFrameAngle(servoId, response);
      }

      break;

    case EServoCommands::SET_ANGLE:
      if (request.nbParams == 2U)
      {
        uint8_t angle = request.params[1U];
        this->mServos.GetServo(servoId).SetAngle(angle);
        return mServos.BuildFrameAngle(servoId, response);
      }
      break;

    case EServoCommands::GET_MIN:
      if (request.nbParams == 1U)
      {
        return mServos.BuildFrameMinAngle(servoId, response);
      }
      break;

    case EServoCommands::SET_MIN:
      if (request.nbParams == 2U)
      {
        uint8_t angle = request.params[1U];
        this->mServos.GetServo(servoId).SetMin(angle);
        return mServos.BuildFrameMinAngle(servoId, response);
      }
      break;

    case EServoCommands::GET_MAX:
      if (request.nbParams == 1U)
      {
        return mServos.BuildFrameMaxAngle(servoId, response);
      }
      break;

    case EServoCommands::SET_MAX:
      if (request.nbParams == 2U)
      {
        uint8_t angle = request.params[1U];
        this->mServos.GetServo(servoId).SetMax(angle);
        return mServos.BuildFrameMaxAngle(servoId, response);
      }
      break;

    case EServoCommands::GET_OFFSET:
      if (request.nbParams == 1U)
      {
        return mServos.BuildFrameOffset(servoId, response);
      }
      break;

    case EServoCommands::SET_OFFSET:
      if (request.nbParams == 2U)
      {
        int8_t angle = (int8_t)request.params[1U];
        this->mServos.GetServo(servoId).SetOffset(angle);
        return mServos.BuildFrameOffset(servoId, response);
      }
      break;

    case EServoCommands::GET_STATE:
      if (request.nbParams == 1U)
      {
        return mServos.BuildFrameState(servoId, response);
      }
      break;

    case EServoCommands::SET_STATE:
      if (request.nbParams == 2U)
      {
        if (request.params[1U] == 0x00U || request.params[1U] == 0x01U)
        {
          this->mServos.GetServo(servoId).SetEnable(request.params[1U]);
          return mServos.BuildFrameState(servoId, response);
        }
      }
      break;

    case EServoCommands::GET_REVERSE:
      if (request.nbParams == 1U)
      {
        return mServos.BuildFrameReverse(servoId, response);
      }
      break;

    case EServoCommands::SET_REVERSE:
      if (request.nbParams == 2U)
      {
        if (request.params[1U] == 0x00U || request.params[1U] == 0x01U)
        {
          this->mServos.GetServo(servoId).SetReverse((bool)request.params[1U]);
          return mServos.BuildFrameReverse(servoId, response);
        }
      }
      break;

    default:
      break;
    }
    return success;
  }

private:
  Servos &mServos;
  
};