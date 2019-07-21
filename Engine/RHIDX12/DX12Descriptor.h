#pragma once

#include "DX12Header.h"
#include "DX12Device.h"
#include "DX12Resource.h"
#include <RHI/RHIResource.h>

namespace z {

/*
Manager D3D12 Descriptors

DX12DescriptorHeap
DX12DescriptorView

*/

/////////// Descriptor Heap

struct DX12DescriptorHeapPos {
	ID3D12DescriptorHeap* heap;
	D3D12_CPU_DESCRIPTOR_HANDLE cpuHandle;
};

class DX12DescriptorHeapAllocator {
public:
	DX12DescriptorHeapAllocator(D3D12_DESCRIPTOR_HEAP_TYPE type, const uint32_t bucketSize = 64);
	~DX12DescriptorHeapAllocator();

	DX12DescriptorHeapPos Allocate();
	void Free(DX12DescriptorHeapPos const& pos);

	static void CreateHeapAllocators();
	static void DestroyHeapAllocators();

	template<typename Desc> static DX12DescriptorHeapAllocator* GetAllocator() {}
#define DEF_GETALLOC(desc, heap) template<> static DX12DescriptorHeapAllocator* GetAllocator<desc>() { return heap; }
	DEF_GETALLOC(D3D12_RENDER_TARGET_VIEW_DESC, gDescHeapRTV)
	DEF_GETALLOC(D3D12_DEPTH_STENCIL_VIEW_DESC, gDescHeapDSV)
	DEF_GETALLOC(D3D12_SHADER_RESOURCE_VIEW_DESC, gDescHeapSRV)
	DEF_GETALLOC(D3D12_CONSTANT_BUFFER_VIEW_DESC, gDescHeapCBV)
	DEF_GETALLOC(D3D12_UNORDERED_ACCESS_VIEW_DESC, gDescHeapUAV)
#undef DEF_GETALLOC

private:
	void AllocateNewHeap();

	D3D12_DESCRIPTOR_HEAP_DESC mDesc;
	uint32_t mDescSize;

	std::vector<RefCountPtr<ID3D12DescriptorHeap>> mHeaps;
	std::vector<DX12DescriptorHeapPos> mFreeDescriptors;

	static DX12DescriptorHeapAllocator* gDescHeapRTV;
	static DX12DescriptorHeapAllocator* gDescHeapDSV;
	static DX12DescriptorHeapAllocator* gDescHeapSRV;
	static DX12DescriptorHeapAllocator* gDescHeapCBV;
	static DX12DescriptorHeapAllocator* gDescHeapUAV;
};


/////////// Descriptor View

template<typename VIEW_DESC>
class DX12DescriptorViewHandle {
public:
	DX12DescriptorViewHandle() {
		mHeapPos = DX12DescriptorHeapAllocator::GetAllocator<VIEW_DESC>()->Allocate();
	}

	~DX12DescriptorViewHandle() {
		DX12DescriptorHeapAllocator::GetAllocator<VIEW_DESC>()->Free(mHeapPos);
	}

	void Create(const VIEW_DESC& desc, ID3D12Resource* resource, ID3D12Resource* counter = nullptr) {
		auto device = GDX12Device->GetIDevice();
		if constexpr (std::is_same_v<VIEW_DESC, D3D12_RENDER_TARGET_VIEW_DESC>) {
			device->CreateRenderTargetView(resource, &desc, mHeapPos.cpuHandle);
		} else if constexpr (std::is_same_v<VIEW_DESC, D3D12_DEPTH_STENCIL_VIEW_DESC>) {
			device->CreateDepthStencilView(resource, &desc, mHeapPos.cpuHandle);
		} else if constexpr (std::is_same_v<VIEW_DESC, D3D12_SHADER_RESOURCE_VIEW_DESC>) {
			device->CreateShaderResourceView(resource, &desc, mHeapPos.cpuHandle);
		} else if constexpr (std::is_same_v<VIEW_DESC, D3D12_CONSTANT_BUFFER_VIEW_DESC>) {
			device->CreateConstantBufferView(resource, &desc, mHeapPos.cpuHandle);
		} else if constexpr (std::is_same_v<VIEW_DESC, D3D12_UNORDERED_ACCESS_VIEW_DESC>) {
			device->CreateUnorderedAccessView(resource, counter, &desc, mHeapPos.cpuHandle);
		}
	}

	const D3D12_CPU_DESCRIPTOR_HANDLE& GetCPUHandle() {
		return mHeapPos.cpuHandle;
	}

private:
	DX12DescriptorHeapPos mHeapPos;
};


template<typename VIEW_DESC>
class DX12DescriptorView : public RHIResource {
public:

protected:
	void CreateView(const VIEW_DESC& desc, DX12Resource* res, DX12Resource* counter = nullptr) {
		mResouce = res;
		mDesc = desc;

		mDescriptor.Create(desc, res->GetIResource(), counter ? counter->GetIResource() : nullptr);
	}

	DX12DescriptorViewHandle<VIEW_DESC> mDescriptor;
	DX12Resource* mResouce;
	VIEW_DESC mDesc;
};


class DX12RenderTargetView : public DX12DescriptorView<D3D12_RENDER_TARGET_VIEW_DESC> {
public:
	DX12RenderTargetView(D3D12_RENDER_TARGET_VIEW_DESC& desc, DX12Resource* res) {
		CreateView(desc, res);
	}
};


class DX12DepthStencilView : public DX12DescriptorView<D3D12_DEPTH_STENCIL_VIEW_DESC> {
public:
	DX12DepthStencilView(D3D12_DEPTH_STENCIL_VIEW_DESC& desc, DX12Resource* res) {
		CreateView(desc, res);
	}
};

class DX12ShaderResourceView : public DX12DescriptorView<D3D12_SHADER_RESOURCE_VIEW_DESC> {
public:
	DX12ShaderResourceView(D3D12_SHADER_RESOURCE_VIEW_DESC& desc, DX12Resource* res) {
		CreateView(desc, res);
	}

};

}