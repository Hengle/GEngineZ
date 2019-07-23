#pragma once

#include "DX12Header.h"

namespace z {

class DX12Resource;
class DX12Viewport;

class DX12CommandList : public NonCopyable {
public:
	DX12CommandList();
	~DX12CommandList();

	void Execute(bool waitComplete=false);
	void Flush();
	void Reset();

	void Close();

	ID3D12GraphicsCommandList* operator->() const {
		return mCommandList;
	}

	ID3D12GraphicsCommandList* GetCommandList() {
		return mCommandList.GetRef();
	}

	ID3D12CommandQueue* GetCommandQueue() {
		return mCommandQueue.GetRef();
	}

private:
	RefCountPtr<ID3D12CommandQueue> mCommandQueue;
	RefCountPtr<ID3D12CommandAllocator> mCommandAllocator;
	RefCountPtr<ID3D12GraphicsCommandList> mCommandList;

	// fence
	RefCountPtr<ID3D12Fence> mFence;
	UINT64 mFenceValue;
	bool mClosed;
};


class DX12CommandContext {
public:
	DX12CommandContext() {
	}

	void Execute(bool waitComplete = false) {
		mList.Execute(waitComplete);
	}

	void Flush() {
		mList.Flush();
	}

	void Reset() {
		mList.Reset();
	}

	DX12CommandList& List() {
		return mList;
	}
	


private:
	DX12CommandList mList;

};

}