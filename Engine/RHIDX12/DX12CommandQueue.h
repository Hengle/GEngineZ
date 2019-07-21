#pragma once

#include "DX12Header.h"

namespace z {

using Microsoft::WRL::ComPtr;

class DX12CommandQueue {
public:
	void Create();
	void Destroy();
	void Execute();
	void Flush();
	void Reset();

	ID3D12CommandQueue* GetICommandQueue() {
		return mCommandQueue.Get();
	}
private:
	ComPtr<ID3D12CommandQueue> mCommandQueue;
	ComPtr<ID3D12CommandAllocator> mCommandAllocator;
	ComPtr<ID3D12GraphicsCommandList> mCommandList;

	// fence
	ComPtr<ID3D12Fence> mFence;
	UINT64 mFenceValue;
};

}