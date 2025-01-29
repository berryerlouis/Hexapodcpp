#pragma once
#include <gmock/gmock.h>
#include "../../../src/Component/Sound/SoundInterface.h"

namespace Component
{
	namespace Sound
	{
		class MockSound : public SoundInterface {
		public:
			MOCK_METHOD0(Initialize, Core::Status( void ));
			MOCK_METHOD1(Update, void( const uint64_t ));

			MOCK_CONST_METHOD0(Get, SoundState (void));
			MOCK_CONST_METHOD0(GetLastStartTimeHit, uint64_t (void));

			MOCK_METHOD1(Attach, Core::Status( SoundObserverInterface * ));
			MOCK_METHOD3(Notify, void( const SoundId &, const SoundState &, const uint16_t ));
		};
	}
}
