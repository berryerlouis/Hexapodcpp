#pragma once

#include "SoftwareInterface.h"

namespace Component {
namespace Software {
class Software : public SoftwareInterface {
public:

    Software();
    ~Software() = default;

    virtual Core::CoreStatus Initialize( void ) final override;
    virtual void Update( const uint64_t currentTime ) final override;

    virtual Version GetVersion( void ) final override;

    virtual uint64_t GetMinTime( void ) final override;
    virtual uint64_t GetMaxTime( void ) final override;
    virtual void ResetTiming( void ) final override;
    virtual void SetMinTime( const uint64_t time ) final override;
    virtual void SetMaxTime( const uint64_t time ) final override;

private:
    uint64_t mMinTime;
    uint64_t mMaxTime;
};
}
}
