#pragma once

#include <RHI/RHIDevice.h>

#include "DX12Header.h"
namespace z {

class DX12DescriptorHeapAllocator;
class DX12CommandContext;

class DX12Device: public RHIDevice {
public:
	DX12Device(HWND hwnd);
	~DX12Device();

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

	DX12CommandContext* GetCommandContext() {
		return mCommandContext;
	}

	

private:
	HWND mHWND;
	RefCountPtr<IDXGIFactory5> mDxgiFactory{ nullptr };
	RefCountPtr<ID3D12Device3> mDevice{ nullptr };
	
	DX12CommandContext* mCommandContext{ nullptr };


};

extern DX12Device* GDX12Device;

}