#pragma once
#include <string>

namespace Core
{
    enum Status {
        CORE_OK = (0),
        CORE_ERROR = (-1),
        CORE_ERROR_ARGUMENT = (-2),
        CORE_ERROR_NULLPTR = (-3),
        CORE_ERROR_OVERLOAD = (-4),
        CORE_ERROR_SIZE = (-5),
        CORE_ERROR_MIN = (-6),
        CORE_ERROR_MAX = (-7),
        CORE_ERROR_DISABLE = (-8),
        CORE_ERROR_UNKNOWN_CLUSTER = (-9),
    };

    static inline bool IsSuccess(const Status status) {
        return (status == CORE_OK);
    }

    static inline std::string StatusToString(const Status status) {
        switch (status) {
            case CORE_OK:
                return "CORE_OK";
            case CORE_ERROR:
                return "CORE_ERROR";
            case CORE_ERROR_ARGUMENT:
                return "CORE_ERROR_ARGUMENT";
            case CORE_ERROR_NULLPTR:
                return "CORE_ERROR_NULLPTR";
            case CORE_ERROR_OVERLOAD:
                return "CORE_ERROR_OVERLOAD";
            case CORE_ERROR_SIZE:
                return "CORE_ERROR_SIZE";
            case CORE_ERROR_MIN:
                return "CORE_ERROR_MIN";
            case CORE_ERROR_MAX:
                return "CORE_ERROR_MAX";
            case CORE_ERROR_DISABLE:
                return "CORE_ERROR_DISABLE";
            case CORE_ERROR_UNKNOWN_CLUSTER:
                return "CORE_ERROR_UNKNOWN_CLUSTER";
            default:
                return "UNKNOWN_STATUS";
        }
    }
}
