#pragma once
#include <Core/CoreHeader.h>
#include <RHI/RHIResource.h>
namespace z {

class RenderStage {
public:
	virtual void OnResize(uint32_t width, uint32_t height);
	virtual void InitStage() = 0;
	virtual void DrawStage() = 0;
	virtual void SetRT() = 0;
	virtual void GetRT() = 0;
};


}