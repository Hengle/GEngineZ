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

// pipeline state
class RHIShader : public RHIResource {
public:
	virtual ERHIShaderType GetShaderType() = 0;
};

class RHIVertexLayout : public RHIResource {
public:
	virtual void PushLayout(const std::string& name, uint32_t index, ERHIPixelFormat format, EVertexLaytoutFlag flag) = 0;
};

class RHIUniformLayout : public RHIResource {
public:
	virtual void PushLayout(std::string name, uint32_t registerNo, EUniformLayoutFlag flag) = 0;
};

class RHIPipelineState : public RHIResource {

};

// buffer
class RHIVertexBuffer : public RHIResource {
};

class RHIIndexBuffer : public RHIResource {

};

class RHIConstantBuffer : public RHIResource {
public:
	virtual void CopyData(void* data, uint32_t size) = 0;
};








class RHIPipeline : public RHIResource {

};






}