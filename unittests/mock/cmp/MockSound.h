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

			MOCK_CONST_METHOD0(GetStatus, SoundState (void));
			MOCK_CONST_METHOD0(GetLastStartTimeHit, uint64_t (void));
			MOCK_CONST_METHOD0(GetIntervalSoundHit, uint64_t (void));
		};
	}
}
