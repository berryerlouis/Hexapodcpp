#pragma once

#include "Constants.h"
#include "Frame.h"

class ClusterInterface
{
public:
    ClusterInterface(void) = default;
    ~ClusterInterface() = default;

    virtual bool Execute(Frame &request, Frame &response) = 0;
};