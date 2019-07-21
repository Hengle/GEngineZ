#include "DX12CommandQueue.h"
#include "DX12Device.h"
#include "DX12Header.h"

namespace z {

void DX12CommandQueue::Create() {
	auto device = GDX12Device->GetIDevice();
	
	// create command objects (queue/allocator/list)
	D3D12_COMMAND_QUEUE_DESC queueDesc{};
	queueDesc.Type = D3D12_COMMAND_LIST_TYPE_DIRECT;
	queueDesc.Priority = 0;
	queueDesc.NodeMask = 0;
	queueDesc.Flags = D3D12_COMMAND_QUEUE_FLAG_NONE;
	DX12_CHECK(device->CreateCommandQueue(&queueDesc, IID_PPV_ARGS(&mCommandQueue)));
	DX12_CHECK(device->CreateCommandAllocator(D3D12_COMMAND_LIST_TYPE_DIRECT, IID_PPV_ARGS(&mCommandAllocator)));
	DX12_CHECK(device->CreateCommandList(0, D3D12_COMMAND_LIST_TYPE_DIRECT, mCommandAllocator.Get(),
		nullptr,                        // initial pipeline is none
		IID_PPV_ARGS(&mCommandList)));
	mCommandList->Close();

	Log<LINFO>("Command queue create done.");
}

void DX12CommandQueue::Destroy() {
	// todo
}

void DX12CommandQueue::Execute() {

}

void DX12CommandQueue::Flush() {
	// flush command and wait compeleted.


}


void DX12CommandQueue::Reset() {

}

}
