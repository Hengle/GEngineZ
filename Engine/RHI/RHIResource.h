#pragma once;

#include "RHIConst.h"
#include <Core/CoreHeader.h>

namespace z {

// rhi resource must be interface, and create by device
class RHIResource : public RefCounter {
public:
	RHIResource() : RefCounter(), mImmedDel(false){
		//Log<LDEBUG>("create rhi resource", this);
	}

	void MarkImmedDel() {
		mImmedDel = true;
	}

	int32_t Release() const {
		// override release. 
		// free in next frame, because command list may use resource in this frame... 
		int old = --mCntRef;
		if (mCntRef == 0) {
			if (mImmedDel) {
				delete this;
			} else {
				gWaitDelReousrce.push_back(const_cast<RHIResource*>(this));
			}
		}
		return old;
	}

	virtual ~RHIResource() {
		// Log<LDEBUG>("destroy rhi resource", this);
	}

	static void FreeWaitDelResource() {
		// gWaitDelReousrce may changed when delete, so copy it first...
		std::vector<RHIResource*> backup = gWaitDelReousrce;
		gWaitDelReousrce.clear();
		for (RHIResource* res : backup) {
			delete res;
		}
	}
private:
	bool mImmedDel;

	static std::vector<RHIResource*> gWaitDelReousrce;
};


// texture
class RHITexture : public RHIResource {
public:
	virtual void Clear(const RHIClearValue& v) = 0;
	
	// render target
	virtual void SetBlendState(const RHIBlendState&) {}
};

// Viewport 
class RHIViewport : public RHIResource {
public:
	virtual void Resize(uint32_t width, uint32_t height) = 0;
	virtual void BeginDraw() = 0;
	virtual void EndDraw() = 0;
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
};

class RHIShaderInstance : public RHIResource {
public:
	virtual void SetParameter(const std::string& key, const void* value, int size) = 0;
	virtual void SetParameter(const std::string& key, RHITexture*, uint32_t sampler=SAMPLER_FILTER_LINEAR|SAMPLER_ADDRESS_WRAP) = 0;

	// clone current parameter to another shader inst
	virtual void CloneParametersTo(RHIShaderInstance*) = 0;
};

// buffer
class RHIVertexBuffer : public RHIResource {
public:
	virtual void* MapBuffer() = 0;
	virtual void UnMapBuffer() = 0;
	virtual uint32_t GetBufferSize() = 0;
};

class RHIIndexBuffer : public RHIResource {
public:
	virtual void* MapBuffer() = 0;
	virtual void UnMapBuffer() = 0;
	virtual uint32_t GetBufferSize() = 0;
};








}