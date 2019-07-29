#include "DX12View.h"

namespace z {

// descriptor heap

DX12DescriptorHeapAllocator* DX12DescriptorHeapAllocator::gDescHeapRTV = nullptr;
DX12DescriptorHeapAllocator* DX12DescriptorHeapAllocator::gDescHeapDSV = nullptr;
DX12DescriptorHeapAllocator* DX12DescriptorHeapAllocator::gDescHeapSRV = nullptr;
DX12DescriptorHeapAllocator* DX12DescriptorHeapAllocator::gDescHeapCBV = nullptr;
DX12DescriptorHeapAllocator* DX12DescriptorHeapAllocator::gDescHeapUAV = nullptr;
DX12DescriptorHeapAllocator* DX12DescriptorHeapAllocator::gDescHeapSampler = nullptr;

void DX12DescriptorHeapAllocator::CreateHeapAllocators() {
	gDescHeapRTV = new DX12DescriptorHeapAllocator(D3D12_DESCRIPTOR_HEAP_TYPE_RTV, 64);
	gDescHeapDSV = new DX12DescriptorHeapAllocator(D3D12_DESCRIPTOR_HEAP_TYPE_DSV, 64);
	gDescHeapSRV = new DX12DescriptorHeapAllocator(D3D12_DESCRIPTOR_HEAP_TYPE_CBV_SRV_UAV, 1024);
	gDescHeapSampler = new DX12DescriptorHeapAllocator(D3D12_DESCRIPTOR_HEAP_TYPE_SAMPLER, 1024);
	gDescHeapCBV = gDescHeapSRV;
	gDescHeapUAV = gDescHeapSRV;
}

void DX12DescriptorHeapAllocator::DestroyHeapAllocators() {
	SAFE_DELETE(gDescHeapRTV);
	SAFE_DELETE(gDescHeapDSV);
	SAFE_DELETE(gDescHeapSRV);
	SAFE_DELETE(gDescHeapSampler)
	gDescHeapCBV = nullptr;
	gDescHeapUAV = nullptr;
}

DX12DescriptorHeapAllocator::DX12DescriptorHeapAllocator(D3D12_DESCRIPTOR_HEAP_TYPE type, const uint32_t bucketSize) :
	mDescSize(GDX12Device->GetIDevice()->GetDescriptorHandleIncrementSize(type)) {
	mDesc.Type = type;
	mDesc.NumDescriptors = bucketSize;
	mDesc.NodeMask = 0;
	mDesc.Flags = type <= D3D12_DESCRIPTOR_HEAP_TYPE_SAMPLER ?
		D3D12_DESCRIPTOR_HEAP_FLAG_SHADER_VISIBLE : D3D12_DESCRIPTOR_HEAP_FLAG_NONE;
}

DX12DescriptorHeapAllocator::~DX12DescriptorHeapAllocator() {
	mHeaps.clear();
	mFreeDescriptors.clear();
}

DX12DescriptorHeapPos DX12DescriptorHeapAllocator::Allocate() {
	if (mFreeDescriptors.empty()) {
		AllocateNewHeap();
	}
	DX12DescriptorHeapPos last = mFreeDescriptors.back();
	mFreeDescriptors.pop_back();
	Log<LDEBUG>("Allocate descriptor", mDesc.Type, last.heap, last.cpuHandle.ptr);
	return last;
}

void DX12DescriptorHeapAllocator::Free(DX12DescriptorHeapPos const& pos) {
	mFreeDescriptors.emplace_back(pos);
}

void DX12DescriptorHeapAllocator::AllocateNewHeap() {
	Log<LDEBUG>("Allocate descriptor heap for type", mDesc.Type);

	RefCountPtr<ID3D12DescriptorHeap> newHeap;
	GDX12Device->GetIDevice()->CreateDescriptorHeap(&mDesc, IID_PPV_ARGS(newHeap.GetComRef()));
	mHeaps.push_back(newHeap);

	D3D12_CPU_DESCRIPTOR_HANDLE cpuAddr = newHeap->GetCPUDescriptorHandleForHeapStart();
	D3D12_GPU_DESCRIPTOR_HANDLE gpuAddr = newHeap->GetGPUDescriptorHandleForHeapStart();
	for (uint32_t i = 0; i < mDesc.NumDescriptors; i++) {
		mFreeDescriptors.emplace_back(
			DX12DescriptorHeapPos{
				newHeap.GetRef(),
				cpuAddr.ptr + i * mDescSize,
				gpuAddr.ptr + i * mDescSize,
			}
		);
	}

	return;
}



}