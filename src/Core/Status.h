#pragma once

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
}
