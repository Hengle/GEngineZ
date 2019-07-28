#pragma once

#include <RHI/RHIDevice.h>

#include "DX12Header.h"
namespace z {

class DX12DescriptorHeapAllocator;
class DX12Executor;

class DX12Device: public RHIDevice {
public:
	DX12Device(HWND hwnd);
	~DX12Device();


	// DX12 Method
	void InitDevice(HWND hwnd);

	IDXGIFactory5* GetIDXGIFactory() {
		return mDxgiFactory.GetRef();
	}

	HWND GetHWND() {
		return mHWND;
	}

	ID3D12Device3* GetIDevice() {
		return mDevice.GetRef();
	}

	DX12Executor* GetExecutor() {
		return mExecutor;
	}

	// ====  begin device method ====
	RHIViewport* CreateViewport(uint32_t width, uint32_t height, ERHIPixelFormat format) override;
	RHIShader* CreateShader(const char* data, size_t dataLen, ERHIShaderType stype) override;
	RHIVertexLayout* CreateVertexLayout() override;
	RHIUniformLayout* CreateUniformLayout() override;
	RHIPipelineState* CreatePipelineState(const RHIPipelineStateDesc&) override;
	RHIConstantBuffer* CreateConstantBuffer(uint32_t size) override;
	RHIIndexBuffer* CreateIndexBuffer(uint32_t num, uint32_t stride, const void* data) override;
	RHIVertexBuffer* CreateVertexBuffer(uint32_t num, uint32_t stride, const void* data) override;
	RHITexture* CreateTexture(const RHITextureDesc& desc, const uint8_t* data) override;
	RHITexture* CreateDepthStencil(uint32_t width, uint32_t height, ERHIPixelFormat format) override;
	RHITexture* CreateRenderTarget(uint32_t width, uint32_t height, ERHIPixelFormat format) override {
		return nullptr;
	};

	void SetPipelineState(RHIPipelineState*) override;
	void SetRenderTargets(const std::vector<RHITexture*>&) override;
	void SetDepthStencil(RHITexture* res) override;
	void SetVertexBuffer(RHIVertexBuffer*) override;
	void SetIndexBuffer(RHIIndexBuffer*) override;
	void SetConstantBuffer(int, RHIConstantBuffer*) override;
	void SetTexture(int, RHITexture*) override;
	void DrawIndexed() override;
	// ==== end device method ====


private:
	HWND mHWND;
	RefCountPtr<IDXGIFactory5> mDxgiFactory{ nullptr };
	RefCountPtr<ID3D12Device3> mDevice{ nullptr };

	DX12Executor* mExecutor;
};

extern DX12Device* GDX12Device;

}