#pragma once

#include <RHI/RHIDevice.h>

#include <wrl.h>
#include <dxgi1_6.h>
#include <d3d12.h>
#include <D3Dcompiler.h>
#include <DirectXMath.h>
#include <DirectXPackedVector.h>
#include <DirectXColors.h>
#include <DirectXCollision.h>

#pragma comment(lib,"d3dcompiler.lib")
#pragma comment(lib, "D3D12.lib")
#pragma comment(lib, "dxgi.lib")

namespace z {

using Microsoft::WRL::ComPtr;

class DX12Device: public RHIDevice {
public:
	DX12Device(HWND hwnd, int width, int height);
	~DX12Device();

	void InitDevice(HWND hwnd, int width, int height);

private:
	const static int kBackBufferCount = 2;

	// dxgi
	ComPtr<IDXGIFactory6> mDxgiFactory{ nullptr };
	// device
	ComPtr<ID3D12Device5> mDevice{ nullptr };
	// commonad queue
	ComPtr<ID3D12CommandQueue> mCommandQueue;
	ComPtr<ID3D12CommandAllocator> mCommandAllocator;
	ComPtr<ID3D12GraphicsCommandList> mCommandList;
	// fence
	ComPtr<ID3D12Fence> mFence;
	UINT64 mFenceValue;
	// swapchain
	ComPtr<IDXGISwapChain1> mSwapChain;

};

extern DX12Device* GDX12Device;

}