#pragma once

#include "BodyInterface.h"

namespace Bot {
namespace Body {
class Body : public BodyInterface {
public:
    struct SBodyIk
    {
        float                      totalY;
        float                      totalX;
        float                      distBodyCenterFeet;
        float                      angleBodyCenterX;
        float                      rollZ;
        float                      pitchZ;
        Misc::Geometry::Position3d bodyIk;
    };

    Body( Legs::Legs &legs );
    ~Body() = default;

    virtual Core::CoreStatus Initialize( void ) final override;
    virtual void Update( const uint64_t currentTime ) final override;

    virtual void SetPositionRotation( const Misc::Geometry::Position3d &position, const Misc::Geometry::Rotation3d &rotation, const uint16_t travelTime ) final override;

private:
    SBodyIk mBodyIk;
    Legs::Legs &mLegs;

    void SetBodyIk( const Misc::Geometry::Position3d &position, const Misc::Geometry::Rotation3d &rotation, const uint16_t travelTime );
};
}
}
