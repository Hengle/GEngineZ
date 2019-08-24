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
	RHIShader* CreateShader() override;
	RHIShaderStage* CreateShaderStage(const char* data, size_t dataLen, ERHIShaderStage stype) override;
	RHIShaderInstance* CreateShaderInstance(RHIShader*) override;
	RHIIndexBuffer* CreateIndexBuffer(uint32_t num, uint32_t stride, const void* data, bool dynamic = false) override;
	RHIVertexBuffer* CreateVertexBuffer(uint32_t num, const std::vector<ERHIInputSemantic>&, const void* data, bool dynamic = false) override;
	RHITexture* CreateTexture(const RHITextureDesc& desc, const uint8_t* data) override;
	
	RHITexture* CreateDepthStencil(uint32_t width, uint32_t height, ERHIPixelFormat format) override;
	RHITexture* CreateRenderTarget(uint32_t width, uint32_t height, ERHIPixelFormat format) override;

	void SetRenderRect(const RHIRenderRect& rect) override;
	void SetScissorRect(const RHIScissorRect& rect) override;
	void SetOutputs(const std::vector<RHITexture*>& rts, RHITexture *ds=nullptr) override;
	void DrawIndexed(RHIShaderInstance* si, RHIVertexBuffer* vb, RHIIndexBuffer* ib, RHIRenderState state, uint32_t numIndex, uint32_t baseIndex, uint32_t baseVertex) override;

	void ReloadShaders() override;
	// ==== end device method ====

	
private:
	HWND mHWND;
	RefCountPtr<IDXGIFactory5> mDxgiFactory{ nullptr };
	RefCountPtr<ID3D12Device3> mDevice{ nullptr };

	DX12Executor* mExecutor;
};

extern DX12Device* GDX12Device;

}