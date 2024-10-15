#pragma once

#include <stdint.h>
#include <string.h>
#include "../Constants.h"

namespace Service
{
#define EVENT_MAX_PARAMS    10U

    struct SEvent {
        volatile EServices id;
        volatile uint8_t value;
        volatile uint8_t nbParams;
        volatile uint8_t params[EVENT_MAX_PARAMS];

        SEvent() = default;

        SEvent(const EServices id, const uint8_t value)
            : id(id)
              , value(value)
              , nbParams(0U)
              , params{} {
        }

        SEvent(const EServices id, const uint8_t value, const uint8_t *params, const uint8_t nbParams)
            : id(id)
              , value(value)
              , nbParams(nbParams) {
            if (nbParams < EVENT_MAX_PARAMS) {
                memset((void*)this->params, 0U, EVENT_MAX_PARAMS);
                memcpy((void*)this->params, params, nbParams);
            }
        }

        ~SEvent() = default;

        bool operator==(const SEvent &other) const {
            return (this->id == other.id && this->value == other.value && this->nbParams == other.nbParams);
        }
    };
}
