#pragma once

#include "Constants.h"
#include "Cluster.h"


class ClusterGeneral : public Cluster
{

public:
    ClusterGeneral() : Cluster(GENERAL)
    {
    }
    ~ClusterGeneral() = default;

    virtual bool Execute(Frame &request, Frame &response) final override
    {
        bool success = false;
        switch ((EGeneralCommands)request.commandId)
        {
        case EGeneralCommands::VERSION:
            success = ResponseGetVersion(response);
            break;
        case EGeneralCommands::INSTANT_EXECUTION_TIME:
            /*uint32_t params = App::Builder::GetInstance().mUpdateTime - Builder::GetInstance().mPreviousTime;
            response = Response{GENERAL, INSTANT_EXECUTION_TIME, (uint8_t *)&params, 8U};*/
            success = true;
            break;
        case EGeneralCommands::MAX_EXECUTION_TIME:
            /*uint32_t params = App::Builder::GetInstance().mMaxTime;
            response = Response{GENERAL, INSTANT_EXECUTION_TIME, (uint8_t *)&params, 8U};*/
            success = true;
            break;
        case EGeneralCommands::RESET_EXECUTION_TIME:
            /*Builder::GetInstance().mMaxTime = 0;
            uint32_t params = App::Builder::GetInstance().mMaxTime;
            response = Response{GENERAL, INSTANT_EXECUTION_TIME, (uint8_t *)&params, 8U};*/
            success = true;
            break;
        default:
            break;
        }
        return success;
    }

private:
    bool ResponseGetVersion(Frame &response)
    {
        uint8_t params[] = {1, 0};
        response.Build(GENERAL, VERSION, params, 2U);
        return true;
    }
};