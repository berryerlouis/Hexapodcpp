#include "Frame.h"

namespace Cluster
{
    Frame::Frame(void) {
        Reset();
    }

    Frame::Frame(const uint8_t clusterId, const uint8_t commandId)
        : clusterId(clusterId)
          , commandId(commandId)
          , nbParams(0U)
          , params{} {
    }

    Core::CoreStatus Frame::Build(const uint8_t clusterId, const uint8_t commandId) {
        if (nbParams > FRAME_MAX_PARAMS) {
            return (Core::CoreStatus::CORE_ERROR);
        }
        this->clusterId = clusterId;
        this->commandId = commandId;
        this->nbParams = 0U;

        return (Core::CoreStatus::CORE_OK);
    }

    Core::CoreStatus Frame::Build(const uint8_t clusterId, const uint8_t commandId, const uint8_t *params,
                                  const uint8_t nbParams) {
        Core::CoreStatus success = Core::CoreStatus::CORE_ERROR;
        if (nbParams < FRAME_MAX_PARAMS) {
            if ((params == nullptr) && (nbParams == 0U)) {
                success = this->Build(clusterId, commandId);
            } else {
                this->clusterId = clusterId;
                this->commandId = commandId;
                this->nbParams = nbParams;

                memset((void *) this->params, 0U, FRAME_MAX_PARAMS);
                memcpy((void *) this->params, params, nbParams);
                success = Core::CoreStatus::CORE_OK;
            }
        }
        return (success);
    }

    void Frame::Reset(void) {
        this->clusterId = 0U;
        this->commandId = 0U;
        this->nbParams = 0U;
        memset(this->params, 0U, FRAME_MAX_PARAMS);
    }

    void Frame::Set1ByteParam(const uint8_t value) {
        this->SetxBytesParam(1U, &value);
    }

    void Frame::Set2BytesParam(const uint16_t value) {
        this->SetxBytesParam(2U, (uint8_t *) &value);
    }

    void Frame::Set3BytesParam(const uint32_t value) {
        this->SetxBytesParam(3U, (uint8_t *) &value);
    }

    void Frame::Set4BytesParam(const uint32_t value) {
        this->SetxBytesParam(4U, (uint8_t *) &value);
    }

    void Frame::Set6BytesParam(const uint64_t value) {
        this->SetxBytesParam(6U, (uint8_t *) &value);
    }

    void Frame::Set8BytesParam(const uint64_t value) {
        this->SetxBytesParam(8U, (uint8_t *) &value);
    }

    void Frame::SetxBytesParam(const size_t size, const uint8_t *value) {
        memcpy((void *) &this->params[this->nbParams], value, size);
        this->nbParams += size;
    }

    uint8_t Frame::Get1ByteParam(const uint8_t index) const {
        return this->params[index];
    }

    uint16_t Frame::Get2BytesParam(const uint8_t index) const {
        return PTR_TO_UINT16(&this->params[index]);
    }

    uint32_t Frame::Get3BytesParam(const uint8_t index) const {
        return PTR_TO_UINT24(&this->params[index]);
    }

    uint32_t Frame::Get4BytesParam(const uint8_t index) const {
        return PTR_TO_UINT32(&this->params[index]);
    }

    uint64_t Frame::Get6BytesParam(const uint8_t index) const {
        return PTR_TO_UINT56(&this->params[index]);
    }

    uint64_t Frame::Get8BytesParam(const uint8_t index) const {
        return PTR_TO_UINT64(&this->params[index]);
    }
}
