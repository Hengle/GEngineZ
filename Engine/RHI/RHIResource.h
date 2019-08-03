#pragma once

#include "RHIConst.h"
#include <Core/CoreHeader.h>

namespace z {

// rhi resource must be interface, and create by device
class RHIResource : public RefCounter {
public:
	RHIResource() : RefCounter() {
		//Log<LDEBUG>("create rhi resource", this);
	}

	virtual ~RHIResource() {
		//Log<LDEBUG>("destroy rhi resource", this);
	}
};


// texture
class RHITexture : public RHIResource {
public:
	virtual void Clear(const RHIClearValue& v) = 0;
};

// Viewport 
class RHIViewport : public RHIResource {
public:
	virtual void Resize(uint32_t width, uint32_t height) = 0;
	virtual void BeginDraw(const RHIClearValue& clearValue) = 0;
	virtual void EndDraw() = 0;
	virtual void SetRenderRect(const ScreenRenderRect& rect) = 0;
	virtual RHITexture* GetBackBuffer() = 0;
};

// shader
class RHIShaderStage : public RHIResource {
public:
	virtual ERHIShaderStage GetStage() const = 0;
};

class RHIShader : public RHIResource {
public:
	virtual void CombineStage(RHIShaderStage* stage) = 0;
	virtual bool Complete() = 0;
	virtual std::vector<RHIInputDesc> GetRHIInputsDesc() = 0;
};

class RHIShaderInstance : public RHIResource {
public:
	virtual void SetParameter(const std::string& key, const float* value, int size) = 0;
	virtual void SetParameter(const std::string& key, RHITexture*, uint32_t sampler=SAMPLER_FILTER_LINEAR|SAMPLER_ADDRESS_WRAP) = 0;
};

// buffer
class RHIVertexBuffer : public RHIResource {
};

class RHIIndexBuffer : public RHIResource {

};





}