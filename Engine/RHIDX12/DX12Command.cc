#include "DX12Command.h"
#include "DX12Device.h"
#include "DX12Header.h"
#include "DX12Resource.h"

namespace z {

DX12CommandList::DX12CommandList() : 
	mClosed(false)
{
	auto device = GDX12Device->GetIDevice();
	
	// create command objects (queue/allocator/list)
	D3D12_COMMAND_QUEUE_DESC queueDesc{};
	queueDesc.Type = D3D12_COMMAND_LIST_TYPE_DIRECT;
	queueDesc.Priority = 0;
	queueDesc.NodeMask = 0;
	queueDesc.Flags = D3D12_COMMAND_QUEUE_FLAG_NONE;
	DX12_CHECK(device->CreateCommandQueue(&queueDesc, IID_PPV_ARGS(mCommandQueue.GetComRef())));
	DX12_CHECK(device->CreateCommandAllocator(D3D12_COMMAND_LIST_TYPE_DIRECT, IID_PPV_ARGS(mCommandAllocator.GetComRef())));
	DX12_CHECK(device->CreateCommandList(0, D3D12_COMMAND_LIST_TYPE_DIRECT, mCommandAllocator.GetRef(),
		nullptr,                        // initial pipeline is none
		IID_PPV_ARGS(mCommandList.GetComRef())));

	Close();
	DX12_CHECK(device->CreateFence(0, D3D12_FENCE_FLAG_NONE, IID_PPV_ARGS(mFence.GetComRef())));
	mFenceValue = 0;

	Log<LINFO>("Command queue create done.");
}

DX12CommandList::~DX12CommandList() {
	// todo
}


void DX12CommandList::Close() {
	if (!mClosed) {
		DX12_CHECK(mCommandList->Close());
		Log<LDEBUG>("...close");
		mClosed = true;
	}
}


void DX12CommandList::Reset() {
	Log<LDEBUG>("...reset");

	mCommandAllocator->Reset();
	mCommandList->Reset(mCommandAllocator.GetRef(), nullptr);
	mClosed = false;
}


void DX12CommandList::Execute(bool waitComplete) {
	Close();
	Log<LDEBUG>("...execute");


	ID3D12CommandList* cmdLists[] = { mCommandList.GetRef() };
	mCommandQueue->ExecuteCommandLists(_countof(cmdLists), cmdLists);
	if (waitComplete) {
		Flush();
	}
}


void DX12CommandList::Flush() {
	// flush command and wait compeleted.
	++mFenceValue;
	DX12_CHECK(mCommandQueue->Signal(mFence.GetRef(), mFenceValue));
	if (mFence->GetCompletedValue() < mFenceValue) {
		HANDLE event = CreateEventEx(nullptr, false, false, EVENT_ALL_ACCESS);
		DX12_CHECK(mFence->SetEventOnCompletion(mFenceValue, event));
		if (event) {
			WaitForSingleObject(event, INFINITE);
			CloseHandle(event);
		}
	}
}




// CommandContext

void DX12CommandContext::ResourceTransition(DX12Resource* resource, D3D12_RESOURCE_STATES toState) {
	if (toState == resource->GetState()) {
		Log<LWARN>("resource transition with same state.");
		return;
	}
	D3D12_RESOURCE_BARRIER barrier;
	barrier.Type = D3D12_RESOURCE_BARRIER_TYPE_TRANSITION;
	barrier.Flags = D3D12_RESOURCE_BARRIER_FLAG_NONE;
	barrier.Transition.pResource = resource->GetIResource();
	barrier.Transition.StateBefore = resource->GetState();
	barrier.Transition.StateAfter = toState;
	barrier.Transition.Subresource = D3D12_RESOURCE_BARRIER_ALL_SUBRESOURCES;

	resource->SetState(toState);
	mList->ResourceBarrier(1, &barrier);
}

}
