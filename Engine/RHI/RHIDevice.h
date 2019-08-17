#pragma once

#include "RHIConst.h"
#include "RHIResource.h"

namespace z {



struct RHIDrawBatch {
	struct DrawCall {
		uint32_t Num;
		uint32_t BaseIndex;
		uint32_t BaseVertex;
	};
	struct DrawData {
		RHIShaderInstance* ShaderInst;
		RHIRenderState RenderState;
		std::vector<DrawCall> DPs;
	};
	RHIVertexBuffer* VBuffer;
	RHIIndexBuffer* IBuffer;
	std::vector<DrawData> Draws;
};


class RHIDevice {
public:
	RHIDevice();
	virtual ~RHIDevice();

	// create
	virtual RHIViewport* CreateViewport(uint32_t width, uint32_t height, ERHIPixelFormat format) = 0;

	virtual RHIShader* CreateShader() = 0;
	virtual RHIShaderStage* CreateShaderStage(const char* data, size_t dataLen, ERHIShaderStage stype) = 0;
	virtual RHIShaderInstance* CreateShaderInstance(RHIShader*) = 0;
	virtual RHIIndexBuffer* CreateIndexBuffer(uint32_t num, uint32_t stride, const void* data, bool dynamic=false) = 0;
	virtual RHIVertexBuffer* CreateVertexBuffer(uint32_t num, const std::vector<ERHIInputSemantic>&, const void* data, bool dynamic=false) = 0;
	virtual RHITexture* CreateDepthStencil(uint32_t width, uint32_t height, ERHIPixelFormat format) = 0;
	virtual RHITexture* CreateRenderTarget(uint32_t width, uint32_t height, ERHIPixelFormat format) = 0;
	virtual RHITexture* CreateTexture(const RHITextureDesc& desc, const uint8_t* data) = 0;

	// command
	virtual void SetVertexBuffer(RHIVertexBuffer*) = 0;
	virtual void SetIndexBuffer(RHIIndexBuffer*) = 0;
	virtual void SetOutputs(const std::vector<RHITexture*>& rts, RHITexture* ds = nullptr) = 0;
	virtual void DrawIndexed(RHIShaderInstance* si, RHIVertexBuffer* vb, RHIIndexBuffer* ib, RHIRenderState state) = 0;
	virtual void DrawBatch(const RHIDrawBatch& batch) = 0;
	//
	void EndDraw() {
		RHIResource::FreeWaitDelResource();
	}
};

extern RHIDevice* GDevice;


}