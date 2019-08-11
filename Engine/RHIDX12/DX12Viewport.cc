#include "DX12Viewport.h"
#include "DX12Device.h"
#include "DX12Executor.h"
#include "DX12View.h"
#include "DX12Buffer.h"

namespace z {


DX12Viewport::DX12Viewport(uint32_t width, uint32_t height, DXGI_FORMAT format) :
	RHIViewport(),
	mWidth(width),
	mHeight(height),
	mFormat(format),
	mCurBackBufferIndex(0) {

	auto hwnd = GDX12Device->GetHWND();
	auto dxgiFactory = GDX12Device->GetIDXGIFactory();
	auto commandQueue = GDX12Device->GetExecutor()->GetCommandQueue();

	// create swap chain
	DXGI_SWAP_CHAIN_DESC1 chainDesc{};
	chainDesc.Width              = mWidth;
	chainDesc.Height             = mHeight;
	chainDesc.Stereo             = false;
	chainDesc.Format             = mFormat;
	chainDesc.SampleDesc.Count   = 1;
	chainDesc.SampleDesc.Quality = 0;
	chainDesc.BufferUsage        = DXGI_USAGE_RENDER_TARGET_OUTPUT | DXGI_USAGE_SHADER_INPUT;
	chainDesc.BufferCount        = BACK_BUFFER_COUNT;
	chainDesc.SwapEffect         = DXGI_SWAP_EFFECT_FLIP_DISCARD;
	chainDesc.Scaling            = DXGI_SCALING_NONE;
	chainDesc.Flags              = DXGI_SWAP_CHAIN_FLAG_ALLOW_MODE_SWITCH;
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
	GDX12Device->GetExecutor()->FlushAndReset();

	// reset back buffers
	for (uint32_t i = 0; i < BACK_BUFFER_COUNT; i++) {
		if (mBackBuffers[i]) {
			mBackBuffers[i]->MarkImmedDel();
		}
		mBackBuffers[i] = nullptr;
	}

	DX12_CHECK(mSwapChain->ResizeBuffers(BACK_BUFFER_COUNT, width, height, mFormat, DXGI_SWAP_CHAIN_FLAG_ALLOW_MODE_SWITCH));

	// GetSwapChain Buffers
	for (uint32_t i = 0; i < BACK_BUFFER_COUNT; i++) {
		// create resource from swapchain
		RefCountPtr<ID3D12Resource> backBufferRes;
		DX12_CHECK(mSwapChain->GetBuffer(i, IID_PPV_ARGS(backBufferRes.GetComRef())));
		D3D12_RESOURCE_DESC backBufferResDesc = backBufferRes->GetDesc();
		DX12Resource *resource = new DX12Resource(backBufferRes, D3D12_RESOURCE_STATE_COMMON, backBufferResDesc);
		
		mBackBuffers[i] = new DX12RenderTarget(resource);
	}
	mCurBackBufferIndex = 0;

	GDX12Device->GetExecutor()->FlushAndReset();
}

void DX12Viewport::Present() {
	// transition back buffer to state present
	GetCurBackBuffer()->SetPresent();
	
	// execute commandlist and flush
	GDX12Device->GetExecutor()->Flush();

	// present and swap buffer
	DX12_CHECK(mSwapChain->Present(1, 0));
	mCurBackBufferIndex = (mCurBackBufferIndex + 1) % BACK_BUFFER_COUNT;
}


void DX12Viewport::BeginDraw(const RHIClearValue& clearValue) {
	// TODO, move set viewport
	mRenderRect.TopLeftX = 0;
	mRenderRect.TopLeftY = 0;
	mRenderRect.Width = (float)mWidth;
	mRenderRect.Height = (float)mHeight;
	mRenderRect.MinDepth = 0.0f;
	mRenderRect.MaxDepth = 1.0f;

	D3D12_RECT scissorRect;
	scissorRect = { 0, 0, (long)mWidth, (long)mHeight };
	GDX12Device->GetExecutor()->GetCommandList()->RSSetViewports(1, &mRenderRect);
	GDX12Device->GetExecutor()->GetCommandList()->RSSetScissorRects(1, &scissorRect);

	// transition state to render target
	DX12RenderTarget* backBuffer = GetCurBackBuffer();
	backBuffer->SetWritable();

	backBuffer->Clear(FromRHIClearVale(clearValue));
	GDX12Device->GetExecutor()->SetRenderTargets({ backBuffer });
	GDX12Device->GetExecutor()->ApplyState();
}

void DX12Viewport::EndDraw() {
	Present();
	DX12BufferUploader::Clear();
	GDX12Device->GetExecutor()->Reset();
}


void DX12Viewport::SetRenderRect(const ScreenRenderRect& rect) {
	mRenderRect.TopLeftX = rect.topLeftX;
	mRenderRect.TopLeftY = rect.topLeftY;
	mRenderRect.Width = rect.width;
	mRenderRect.Height = rect.height;

	GDX12Device->GetExecutor()->GetCommandList()->RSSetViewports(1, &mRenderRect);
}

}