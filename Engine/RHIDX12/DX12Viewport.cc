#include "DX12Viewport.h"
#include "DX12Device.h"
#include "DX12Command.h"
#include "DX12Descriptor.h"

namespace z {


DX12Viewport::DX12Viewport(uint32_t width, uint32_t height) :
	RHIViewport(),
	mWidth(width),
	mHeight(height) {
}

void DX12Viewport::Init() {
	auto hwnd = GDX12Device->GetHWND();
	auto dxgiFactory = GDX12Device->GetIDXGIFactory();
	auto commandQueue = GDX12Device->GetCommandContext()->List().GetCommandQueue();

	// create swap chain
	DXGI_SWAP_CHAIN_DESC1 chainDesc{};
	chainDesc.Width = mWidth;
	chainDesc.Height = mHeight;
	chainDesc.Stereo = false;				// what meaning?
	chainDesc.Format = DXGI_FORMAT_R8G8B8A8_UNORM;	// TODO, more pixel format
	chainDesc.SampleDesc.Count = 1;
	chainDesc.SampleDesc.Quality = 0;
	chainDesc.BufferUsage = DXGI_USAGE_RENDER_TARGET_OUTPUT | DXGI_USAGE_SHADER_INPUT;
	chainDesc.BufferCount = kBackBufferCount;
	chainDesc.SwapEffect = DXGI_SWAP_EFFECT_FLIP_DISCARD;
	chainDesc.Scaling = DXGI_SCALING_NONE;
	chainDesc.Flags = DXGI_SWAP_CHAIN_FLAG_ALLOW_MODE_SWITCH;
	DX12_CHECK(dxgiFactory->CreateSwapChainForHwnd(commandQueue, hwnd, &chainDesc, nullptr, nullptr, mSwapChain.GetComRef()));

	// deny alt+enter to switch between window and full screen
	dxgiFactory->MakeWindowAssociation(hwnd, DXGI_MWA_NO_WINDOW_CHANGES);

	Resize(mWidth, mHeight);

	// tell the window to redraw then
	::PostMessage(hwnd, WM_PAINT, 0, 0);
}


void DX12Viewport::Resize(uint32_t width, uint32_t height) {
	mWidth = width;
	mHeight = height;

	// flush commands first
	GDX12Device->GetCommandContext()->Execute(true);
	GDX12Device->GetCommandContext()->Reset();

	// reset back buffer
	for (uint32_t i = 0; i < kBackBufferCount; i++) {
		mBackBuffers[i] = nullptr;
	}

	DX12_CHECK(mSwapChain->ResizeBuffers(kBackBufferCount,
		width, height, DXGI_FORMAT_R8G8B8A8_UNORM, DXGI_SWAP_CHAIN_FLAG_ALLOW_MODE_SWITCH));

	// GetSwapChain Buffers
	for (uint32_t i = 0; i < kBackBufferCount; i++) {
		// create texture, manage memory of (DX12Resource, DX12RenderTargetView, DX12ShaderResourceView)
		RHITextureDesc desc;
		desc.sizeX = width;
		desc.sizeY = height;
		desc.sizeZ = 1;
		desc.numMips = 1;
		desc.numSamples = 1;
		desc.pixelFormat = DXGI_FORMAT_R8G8B8A8_UNORM;
		desc.flags = 0;
		desc.clearv = TexClearValue(0.5f, 0.3f, 0.2f, 1.0f);
		DX12Texture2D* tex = new DX12Texture2D(desc);
		
		// get resource from swapchain and bind it to texture
		RefCountPtr<ID3D12Resource> backBufferRes;
		DX12_CHECK(mSwapChain->GetBuffer(i, IID_PPV_ARGS(backBufferRes.GetComRef())));
		D3D12_RESOURCE_DESC backBufferResDesc = backBufferRes->GetDesc();
		DX12Resource *resource = new DX12Resource(backBufferRes, D3D12_RESOURCE_STATE_COMMON, backBufferResDesc);
		tex->GetResourceOwner()->OwnResource(EResourceOwn_Exclusive, resource);

		// create render target
		D3D12_RENDER_TARGET_VIEW_DESC rtvDesc = {};
		rtvDesc.Format = backBufferResDesc.Format;
		rtvDesc.ViewDimension = D3D12_RTV_DIMENSION_TEXTURE2D;
		rtvDesc.Texture2D.MipSlice = 0;

		DX12RenderTargetView* rtvView = new DX12RenderTargetView(rtvDesc, resource);
		tex->SetRenderTargetView(rtvView);

		// create shader resource
		D3D12_SHADER_RESOURCE_VIEW_DESC srvDesc = {};
		srvDesc.Format = backBufferResDesc.Format;
		srvDesc.ViewDimension = D3D12_SRV_DIMENSION_TEXTURE2D;
		srvDesc.Shader4ComponentMapping = D3D12_DEFAULT_SHADER_4_COMPONENT_MAPPING;
		srvDesc.Texture2D.MostDetailedMip = 0;
		srvDesc.Texture2D.MipLevels = 1;

		DX12ShaderResourceView* srvView = new DX12ShaderResourceView(srvDesc, resource);
		tex->SetShaderResourceView(srvView);

		mBackBuffers[i] = tex;
	}
	mCurBackBufferIndex = 0;


	GDX12Device->GetCommandContext()->Execute(true);
}

void DX12Viewport::Present() {

	GDX12Device->GetCommandContext()->Reset();

	// transition back buffer to state present
	DX12Texture2D* backBuffer = GetCurBackBuffer();
	GDX12Device->GetCommandContext()->ResourceTransition(backBuffer->GetResourceOwner()->GetResource(), D3D12_RESOURCE_STATE_PRESENT);

	// execute commandlist and flush
	GDX12Device->GetCommandContext()->Execute(true);
	
	// present and swap buffer
	DX12_CHECK(mSwapChain->Present(1, 0));
	mCurBackBufferIndex = (mCurBackBufferIndex + 1) % kBackBufferCount;
}



}