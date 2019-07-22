#include "DX12Device.h"
#include "DX12Command.h"
#include "DX12Descriptor.h"

namespace z {

DX12Device* GDX12Device = nullptr;

DX12Device::DX12Device(HWND hwnd) : 
	RHIDevice(),
	mHWND(hwnd) {
	InitializeSingleton<DX12Device>(GDX12Device, this);

	InitDevice(hwnd);
}

DX12Device::~DX12Device() {
	DX12DescriptorHeapAllocator::DestroyHeapAllocators();
	SAFE_DELETE(mCommandContext);

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

	// enumrate hardware adapters and  create device
	Microsoft::WRL::ComPtr<IDXGIAdapter1> adapter;
	for (UINT idx = 0; DXGI_ERROR_NOT_FOUND != mDxgiFactory->EnumAdapters1(idx, &adapter); idx++) {
		DXGI_ADAPTER_DESC1 desc = {};
		adapter->GetDesc1(&desc);

		if (desc.Flags & DXGI_ADAPTER_FLAG_SOFTWARE) {
			continue;
		}
		// just for test create, use nullptr as paramters
		if (SUCCEEDED(D3D12CreateDevice(adapter.Get(), D3D_FEATURE_LEVEL_12_1, IID_PPV_ARGS(mDevice.GetComRef())))) {
			Log<LINFO>("Find Adapter: ", desc.Description);
	
			D3D12_FEATURE_DATA_ARCHITECTURE arch{};
			mDevice->CheckFeatureSupport(D3D12_FEATURE_ARCHITECTURE, &arch, sizeof(D3D12_FEATURE_DATA_ARCHITECTURE));
			/*if (arch.UMA) {
				Log<LINFO>("Skip this adapter.....");
				mDevice.Reset();
				continue;
			}*/
			break;
		}
	}
	CHECK(mDevice.GetRef() != nullptr, "Device not found!");

	mCommandContext = new DX12CommandContext();

	DX12DescriptorHeapAllocator::CreateHeapAllocators();

	Log<LINFO>("Device init done.");
}


}