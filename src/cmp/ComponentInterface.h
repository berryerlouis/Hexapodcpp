#pragma once
#include <stdint.h>
#include <stddef.h>

namespace Component {
class ComponentInterface {
public:
	ComponentInterface(void) = default;
	~ComponentInterface()    = default;

	virtual bool Initialize(void) = 0;
	virtual void Update(const uint32_t currentTime) = 0;
};
}
