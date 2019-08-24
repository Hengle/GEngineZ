#pragma once
#include <Core/CoreHeader.h>

namespace z {

class Renderer;

enum ERenderStep {
	RENDER_STEP_IMGUI = 0,
	RENDER_STEP_HDR = 1,

};

class RenderStep : public RefCounter {
public:
	virtual void Init() = 0;
	virtual void Resize(uint32_t width, uint32_t height) = 0;
	virtual void Render(Renderer* r) = 0;
};

}
