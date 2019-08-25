#pragma once
#include <Core/CoreHeader.h>

namespace z {

class Renderer;
class RenderTarget;

enum ERenderStep {
	RENDER_STEP_IMGUI = 0,
	RENDER_STEP_HDR = 1,
	RENDER_STEP_FORWARD_MAIN

};

class RenderStep : public RefCounter {
public:
	virtual void Init() = 0;
	virtual void Resize(uint32_t width, uint32_t height) = 0;
	virtual RenderTarget* Render(Renderer *r, RenderTarget* src=nullptr, RenderTarget* dst=nullptr) = 0;
};

}
