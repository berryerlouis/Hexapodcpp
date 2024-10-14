#pragma once

#include "ClusterInterface.h"

namespace Cluster
{
    struct ClusterItem {
        ClusterItem() = default;

        ~ClusterItem() = default;

        uint8_t commandId;
        uint8_t expectedSize;
    };

    class StrategyCluster {
#define NB_COMMANDS_MAX 20U

    public:
        StrategyCluster(const uint8_t size)
            : mClusterCommands{}
              , mClusterCommandSize(0U)
              , mSize(size) {
        }

        ~StrategyCluster() = default;

        bool AddClusterItem(const Cluster::ClusterItem clusterItem) {
            if (this->mClusterCommandSize < this->mSize) {
                mClusterCommands[this->mClusterCommandSize++] = clusterItem;
                return true;
            }
            return false;
        }

        virtual Core::CoreStatus ExecuteFrame(const Frame &request, Frame &response) = 0;

        Core::CoreStatus Execute(const Frame &request, Frame &response) {
            Core::CoreStatus success = Core::CoreStatus::CORE_ERROR;
			const uint8_t cmdId = request.commandId;
			if(cmdId < this->mClusterCommandSize)
			{
				if(this->mClusterCommands[cmdId].expectedSize == request.nbParams)
				{
					success = this->ExecuteFrame(request, response);
				}
			}
            return success;
        }

    private:
        Cluster::ClusterItem mClusterCommands[NB_COMMANDS_MAX];
        uint8_t mClusterCommandSize;
        const uint8_t mSize;
    };


    class ClusterBase : public ClusterInterface {
    public:
        ClusterBase(const EClusters clusterId, StrategyCluster *strategyCluster)
            : mClusterId(clusterId)
              , mStrategyCluster(strategyCluster) {
        }

        virtual Core::CoreStatus Execute(Frame &request, Frame &response) final override {
            Core::CoreStatus success = Core::CoreStatus::CORE_ERROR;
            if (this->mStrategyCluster != nullptr) {
                if (request.clusterId != this->GetId()) {
                    return (success);
                }
                success = this->mStrategyCluster->Execute(request, response);
            }
            return (success);
        }

        virtual EClusters GetId(void) {
            return (this->mClusterId);
        }

        virtual Core::CoreStatus BuildFrameNack(Frame &response) {
            response.clusterId = this->mClusterId;
            response.commandId = static_cast<uint8_t>(GENERIC);
            response.Set1ByteParam(false);
            return (Core::CoreStatus::CORE_OK);
        }

        virtual Core::CoreStatus BuildFrameNack(Frame &response, const Core::CoreStatus error) {
            response.clusterId = this->mClusterId;
            response.commandId = static_cast<uint8_t>(GENERIC);
            response.Set1ByteParam(error);
            return (Core::CoreStatus::CORE_OK);
        }

    protected:
        const EClusters mClusterId;
        StrategyCluster *mStrategyCluster;
    };
}
