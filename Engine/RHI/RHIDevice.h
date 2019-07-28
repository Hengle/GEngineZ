#pragma once

#include "RHIConst.h"
#include "RHIResource.h"

namespace z {

class RHIDevice {
public:
	RHIDevice();
	virtual ~RHIDevice();

	// create
	virtual RHIViewport* CreateViewport(uint32_t width, uint32_t height, ERHIPixelFormat format) = 0;
	virtual RHIDepthStencil* CreateDepthStencil(uint32_t width, uint32_t height, ERHIPixelFormat format) = 0;
	virtual RHIShader* CreateShader(const char* data, size_t dataLen, ERHIShaderType stype) = 0;
	virtual RHIVertexLayout* CreateVertexLayout() = 0;
	virtual RHIUniformLayout* CreateUniformLayout() = 0;
	virtual RHIPipelineState* CreatePipelineState(const RHIPipelineStateDesc&) = 0;
	virtual RHIConstantBuffer* CreateConstantBuffer(uint32_t size) = 0;
	virtual RHIIndexBuffer* CreateIndexBuffer(uint32_t num, uint32_t stride, const void* data) = 0;
	virtual RHIVertexBuffer* CreateVertexBuffer(uint32_t num, uint32_t stride, const void* data) = 0;
	virtual RHITexture2D* CreateTexture2D(const RHITextureDesc& desc, const uint8_t* data) = 0;

	// command
	virtual void SetPipelineState(RHIPipelineState*) = 0;
	virtual void SetRenderTargets(const std::vector<RHIRenderTarget*>&) = 0;
	virtual void SetDepthStencil(RHIDepthStencil*) = 0;
	virtual void SetVertexBuffer(RHIVertexBuffer*) = 0;
	virtual void SetIndexBuffer(RHIIndexBuffer*) = 0;
	virtual void SetConstantBuffer(int, RHIConstantBuffer*) = 0;
	virtual void SetTexture(int, RHITexture*) = 0;
	virtual void DrawIndexed() = 0;
};

extern RHIDevice* GDevice;


}