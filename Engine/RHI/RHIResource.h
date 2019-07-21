#pragma once

#include "RHIConst.h"
#include <Core/CoreHeader.h>

namespace z {

class RHIResource : public RefCounter {
public:
	RHIResource() : RefCounter() {
		Log<LINFO>("create rhi resource");
	}

	virtual ~RHIResource() {
		Log<LINFO>("destroy rhi resource");
	}


};

class RHIViewport : public RHIResource {
public:



};

class RHIShader : public RHIResource {
public:

};


class RHIUniformBuffer : public RHIResource {

};

class RHIVertexBuffer : public RHIResource {

};

class RHIIndexBuffer : public RHIResource {

};

class RHITexture : public RHIResource {

};

class RHITexture2D : public RHITexture {

};

class RHIRenderTarget : public RHITexture2D {

};

class RHIGPUFence : public RHIResource {

};


}