#include "DX12Device.h"

#include <tchar.h>
#include <Windows.h>
#include <windowsx.h>
#include "Helper/Helper.h"

#include <Core/Common/Common.h>

namespace z {

DX12Device* GDX12Device = nullptr;

DX12Device::DX12Device(HWND hwnd, int width, int height) : RHIDevice() {
	InitializeSingleton<DX12Device>(GDX12Device, this);

	InitDevice(hwnd, width, height);
}

DX12Device::~DX12Device() {
	FinalizeSingleton<DX12Device>(GDX12Device, this);
}


void DX12Device::InitDevice(HWND hwnd, int width, int height) {
	UINT createFactoryFlag = 0;
	{
		Microsoft::WRL::ComPtr<ID3D12Debug> debugController;
		if (SUCCEEDED(D3D12GetDebugInterface(IID_PPV_ARGS(&debugController)))) {
			debugController->EnableDebugLayer();
			createFactoryFlag |= DXGI_CREATE_FACTORY_DEBUG;
		}
	}

	// create dxgi factory
	DX12_CHECK(CreateDXGIFactory2(createFactoryFlag, IID_PPV_ARGS(&mDxgiFactory)));

	// enumrate hardware adapters and  create device
	Microsoft::WRL::ComPtr<IDXGIAdapter1> adapter;
	for (UINT idx = 0; DXGI_ERROR_NOT_FOUND != mDxgiFactory->EnumAdapters1(idx, &adapter); idx++) {
		DXGI_ADAPTER_DESC1 desc = {};
		adapter->GetDesc1(&desc);

		if (desc.Flags & DXGI_ADAPTER_FLAG_SOFTWARE) {
			continue;
		}
		// just for test create, use nullptr as paramters
		if (SUCCEEDED(D3D12CreateDevice(adapter.Get(), D3D_FEATURE_LEVEL_12_1, __uuidof(mDevice), nullptr))) {
			Log<LINFO>("Use Adapter: ", desc.Description);
			break;
		}
	}

	// really create device here
	DX12_CHECK(D3D12CreateDevice(adapter.Get(), D3D_FEATURE_LEVEL_12_1, IID_PPV_ARGS(&mDevice)));

	// create fence object
	DX12_CHECK(mDevice->CreateFence(0, D3D12_FENCE_FLAG_NONE, IID_PPV_ARGS(&mFence)));
	mFenceValue = 0;

	//// create command objects (queue/allocator/list)
	D3D12_COMMAND_QUEUE_DESC queueDesc{};
	queueDesc.Type = D3D12_COMMAND_LIST_TYPE_DIRECT;
	DX12_CHECK(mDevice->CreateCommandQueue(&queueDesc, IID_PPV_ARGS(&mCommandQueue)));
	DX12_CHECK(mDevice->CreateCommandAllocator(D3D12_COMMAND_LIST_TYPE_DIRECT, IID_PPV_ARGS(&mCommandAllocator)));
	DX12_CHECK(mDevice->CreateCommandList(0, D3D12_COMMAND_LIST_TYPE_DIRECT, mCommandAllocator.Get(),
		nullptr,                        // initial pipeline is none
		IID_PPV_ARGS(&mCommandList)));
	mCommandList->Close();

	// create swap chain
	DXGI_SWAP_CHAIN_DESC1 chainDesc{};
	chainDesc.Width = width;
	chainDesc.Height = height;
	chainDesc.Format = DXGI_FORMAT_R8G8B8A8_UNORM;
	chainDesc.SampleDesc.Count = 1;
	chainDesc.SampleDesc.Quality = 0;
	chainDesc.BufferUsage = DXGI_USAGE_RENDER_TARGET_OUTPUT;
	chainDesc.BufferCount = kBackBufferCount;
	chainDesc.SwapEffect = DXGI_SWAP_EFFECT_FLIP_DISCARD;
	chainDesc.Scaling = DXGI_SCALING_NONE;
	chainDesc.Flags = DXGI_SWAP_CHAIN_FLAG_ALLOW_MODE_SWITCH;
	DX12_CHECK(mDxgiFactory->CreateSwapChainForHwnd(mCommandQueue.Get(), hwnd, &chainDesc, nullptr, nullptr, &mSwapChain));

}


}