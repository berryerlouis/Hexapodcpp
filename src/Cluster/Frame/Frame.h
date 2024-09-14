#pragma once

#include <stdio.h>
#include <string.h>
#include "../../Core/Status.h"
#include "../../Misc/Maths/Utils.h"

namespace Cluster
{
#define FRAME_MAX_PARAMS    50U

    struct Frame {
    public:
        Frame(void);

        ~Frame(void) = default;

        void Reset(void);

        Core::CoreStatus Build(const uint8_t clusterId, const uint8_t commandId);

        Core::CoreStatus Build(const uint8_t clusterId, const uint8_t commandId, const uint8_t *params,
                               const uint8_t nbParams);

        void Set1ByteParam(const uint8_t value);

        void Set2BytesParam(const uint16_t value);

        void Set3BytesParam(const uint32_t value);

        void Set4BytesParam(const uint32_t value);

        void Set6BytesParam(const uint64_t value);

        void Set8BytesParam(const uint64_t value);

        void SetxBytesParam(const size_t size, const uint8_t *value);

        uint8_t Get1ByteParam(const uint8_t index) const;

        uint16_t Get2BytesParam(const uint8_t index) const;

        uint32_t Get3BytesParam(const uint8_t index) const;

        uint32_t Get4BytesParam(const uint8_t index) const;

        uint64_t Get6BytesParam(const uint8_t index) const;

        uint64_t Get8BytesParam(const uint8_t index) const;

        uint8_t clusterId;
        uint8_t commandId;
        uint8_t nbParams;
        uint8_t params[FRAME_MAX_PARAMS];

    private:
    };
}
