#include "DX12Device.h"
#include "DX12Executor.h"
#include "DX12View.h"
#include "DX12Viewport.h"
#include "DX12Shader.h"
#include "DX12PipelineState.h"

namespace z {

DX12Device* GDX12Device = nullptr;

DX12Device::DX12Device(HWND hwnd) :
	RHIDevice(),
	mHWND(hwnd),
	mExecutor(nullptr) {
	InitializeSingleton<DX12Device>(GDX12Device, this);

	InitDevice(hwnd);
}

DX12Device::~DX12Device() {
	DX12DescriptorHeapAllocator::DestroyHeapAllocators();
	SAFE_DELETE(mExecutor);

	FinalizeSingleton<DX12Device>(GDX12Device, this);
}


void DX12Device::InitDevice(HWND hwnd) {
	UINT createFactoryFlag = 0;
	{
		Microsoft::WRL::ComPtr<ID3D12Debug> debugController;
		if (SUCCEEDED(D3D12GetDebugInterface(IID_PPV_ARGS(&debugController)))) {
			debugController->EnableDebugLayer();
			createFactoryFlag |= DXGI_CREATE_FACTORY_DEBUG;
		}
	}

	// create dxgi factory
	DX12_CHECK(CreateDXGIFactory2(createFactoryFlag, IID_PPV_ARGS(mDxgiFactory.GetComRef())));

	// enumrate hardware adapters and create device
	Microsoft::WRL::ComPtr<IDXGIAdapter1> adapter;
	for (UINT idx = 0; DXGI_ERROR_NOT_FOUND != mDxgiFactory->EnumAdapters1(idx, &adapter); idx++) {
		DXGI_ADAPTER_DESC1 desc = {};
		adapter->GetDesc1(&desc);

		if (desc.Flags & DXGI_ADAPTER_FLAG_SOFTWARE) {
			continue;
		}
		if (SUCCEEDED(D3D12CreateDevice(adapter.Get(), D3D_FEATURE_LEVEL_12_1, IID_PPV_ARGS(mDevice.GetComRef())))) {
			Log<LINFO>("Find Adapter: ", desc.Description);

			D3D12_FEATURE_DATA_ARCHITECTURE arch{};
			mDevice->CheckFeatureSupport(D3D12_FEATURE_ARCHITECTURE, &arch, sizeof(D3D12_FEATURE_DATA_ARCHITECTURE));
			if (arch.UMA) {
				Log<LINFO>("Skip this adapter.....");
				mDevice.Reset();
				continue;
			}
			break;
		}
	}
	CHECK(mDevice.GetRef() != nullptr, "Device not found!");

	// craete executor
	mExecutor = new DX12Executor(this);

	// create descriptor heap
	DX12DescriptorHeapAllocator::CreateHeapAllocators();

	Log<LINFO>("Device init done.");
}



RHIViewport* DX12Device::CreateViewport(uint32_t width, uint32_t height, ERHIPixelFormat format) {
	return new DX12Viewport(width, height, FromRHIFormat(format));
}

RHIShaderStage* DX12Device::CreateShaderStage(const char* data, size_t dataLen, ERHIShaderStage stage) {
	return DX12ShaderStage::FromCompile(data, dataLen, stage);
}

RHIShader* DX12Device::CreateShader() {
	return new DX12Shader();
}

RHIVertexLayout* DX12Device::CreateVertexLayout() {
	return new DX12VertexLayout();
}

RHIUniformLayout* DX12Device::CreateUniformLayout() {
	return new DX12UniformLayout();
}

RHIPipelineState* DX12Device::CreatePipelineState(const RHIPipelineStateDesc& desc) {
	DX12PipelineState* state = new DX12PipelineState();
	state->SetShaderVS(static_cast<DX12ShaderStage*>(desc.vs));
	state->SetShaderPS(static_cast<DX12ShaderStage*>(desc.ps));
	state->SetUniformLayout(static_cast<DX12UniformLayout*>(desc.ulayout));
	state->SetVertexLayout(static_cast<DX12VertexLayout*>(desc.vlayout));
	state->SetDepthStencilFormat(FromRHIFormat(desc.dsFormat));
	std::vector<DXGI_FORMAT> rtsFormat;
	for (int i = 0; i < desc.rtsFormat.size(); i++) {
		rtsFormat.emplace_back(FromRHIFormat(desc.rtsFormat[i]));
	}
	state->SetRenderTargetsFormat(rtsFormat);

	if (FromRHIFormat(desc.dsFormat) == DXGI_FORMAT_UNKNOWN) {
		state->GetDepthStencilState().DepthEnable = false;
	}
	state->GetRasterizerState().CullMode = FromRHICullMode(desc.cullMode);
	state->GetRasterizerState().FillMode = FromRHIFillMode(desc.fillMode);

	return state;
}

RHIConstantBuffer* DX12Device::CreateConstantBuffer(uint32_t size) {
	return new DX12ConstantBuffer(size);
}

RHIIndexBuffer* DX12Device::CreateIndexBuffer(uint32_t num, uint32_t stride, const void* data) {
	return new DX12IndexBuffer(num, stride, data);
}

RHIVertexBuffer* DX12Device::CreateVertexBuffer(uint32_t num, uint32_t stride, const void* data) {
	return new DX12VertexBuffer(num, stride, data);
}

RHITexture* DX12Device::CreateTexture(const RHITextureDesc& desc, const uint8_t* data) {
	return new DX12Texture2D(desc, data);
}

RHITexture* DX12Device::CreateRenderTarget(uint32_t width, uint32_t height, ERHIPixelFormat format) {
	return new DX12RenderTarget(width, height, FromRHIFormat(format));
};

RHITexture* DX12Device::CreateDepthStencil(uint32_t width, uint32_t height, ERHIPixelFormat format) {
	return new DX12DepthStencil(width, height, FromRHIFormat(format));
}


void DX12Device::SetPipelineState(RHIPipelineState* state) {
	mExecutor->SetPipelineState(static_cast<DX12PipelineState*>(state));
}

void DX12Device::SetRenderTargets(const std::vector<RHITexture*>& res) {
	std::vector<DX12RenderTarget*> rts(res.size());
	memcpy(rts.data(), res.data(), res.size() * sizeof(RHITexture*));
	mExecutor->SetRenderTargets(rts);
}

void DX12Device::SetDepthStencil(RHITexture* res) {
	mExecutor->SetDepthStencil(static_cast<DX12DepthStencil*>(res));
}

void DX12Device::SetVertexBuffer(RHIVertexBuffer* res) {
	mExecutor->SetVertexBuffer(static_cast<DX12VertexBuffer*>(res));
}

void DX12Device::SetIndexBuffer(RHIIndexBuffer* res) {
	mExecutor->SetIndexBuffer(static_cast<DX12IndexBuffer*>(res));
}

void DX12Device::SetConstantBuffer(int idx, RHIConstantBuffer* res) {
	mExecutor->SetConstantBuffer(idx, static_cast<DX12ConstantBuffer*>(res));
}

void DX12Device::SetTexture(int idx, RHITexture* res) {
	mExecutor->SetTexture(idx, dynamic_cast<DX12Texture*>(res));
}

void DX12Device::DrawIndexed() {
	mExecutor->Draw();
}

}