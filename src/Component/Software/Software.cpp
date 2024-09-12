#include "Software.h"

namespace Component
{
    namespace Software
    {
        Software::Software() {
        }

        Core::CoreStatus Software::Initialize(void) {
            return (Core::CoreStatus::CORE_OK);
        }

        void Software::Update(const uint64_t currentTime) {
            (void) currentTime;
        }

        SoftwareInterface::Version Software::GetVersion(void) {
            constexpr Version version = {.major = 0U, .minor = 1U};
            return (version);
        }
    }
}
