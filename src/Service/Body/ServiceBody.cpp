#include "ServiceBody.h"

namespace Service
{
    namespace Body
    {
        ServiceBody::ServiceBody(BodyInterface &body, Event::MessageInterface &messageListener) :
            Service(BODY, 100U, messageListener)
            , mBody(body) {
        }

        Core::Status ServiceBody::Initialize(void) {
            const Core::Status success = this->mBody.Initialize();
            if (Core::Status::CORE_OK == success) {
                this->mInitialized = true;
            }
            return (success);
        }

        void ServiceBody::Update(const uint64_t currentTime) {
            this->mBody.Update(currentTime);
        }

    } // namespace Body
} // namespace Service
