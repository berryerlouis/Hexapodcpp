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
    public:
        StrategyCluster()
            : mClusterItems{},
              mClusterItemSize(0U) {
        }

        ~StrategyCluster() = default;

        bool AddClusterItem(const Cluster::ClusterItem clusterItem) {
            if (this->mClusterItemSize < 15U) {
                mClusterItems[this->mClusterItemSize++] = clusterItem;
                return true;
            }
            return false;
        }

        virtual Core::CoreStatus ExecuteFrame(const Frame &request, Frame &response) = 0;

        Core::CoreStatus Execute(const Frame &request, Frame &response) {
            Core::CoreStatus success = Core::CoreStatus::CORE_ERROR;
            for (size_t i = 0; i < this->mClusterItemSize; i++) {
                if (this->mClusterItems[i].commandId == request.commandId
                    && this->mClusterItems[i].expectedSize == request.nbParams) {
                    success = this->ExecuteFrame(request, response);
                }
            }
            return success;
        }

    private:
        Cluster::ClusterItem mClusterItems[15U];
        uint8_t mClusterItemSize = 0U;
    };


    class  ClusterBase : public ClusterInterface {
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
            return (mClusterId);
        }

    protected:
        const EClusters mClusterId;
        StrategyCluster *mStrategyCluster;
    };
}
