#pragma once

#include <RHI/RHIDevice.h>

#include "DX12Header.h"
namespace z {

class DX12DescriptorHeapAllocator;
class DX12CommandQueue;

class DX12Device: public RHIDevice {
public:
	DX12Device(HWND hwnd);
	~DX12Device();

	void InitDevice(HWND hwnd);

	IDXGIFactory6* GetIDXGIFactory() {
		return mDxgiFactory.GetRef();
	}

	HWND GetHWND() {
		return mHWND;
	}

	ID3D12Device5* GetIDevice() {
		return mDevice.GetRef();
	}

	DX12CommandQueue* GetCommandQueue() {
		return mCommandQueue;
	}

	

private:
	HWND mHWND;
	RefCountPtr<IDXGIFactory6> mDxgiFactory{ nullptr };
	RefCountPtr<ID3D12Device5> mDevice{ nullptr };
	
	DX12CommandQueue* mCommandQueue{ nullptr };


};

extern DX12Device* GDX12Device;

}