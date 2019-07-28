#pragma once
#include <Core/CoreHeader.h>
#include <RHI/RHIResource.h>
#include "DX12Resource.h"
#include "DX12Descriptor.h"

namespace z {


class DX12IndexBuffer : public DX12ShaderResource, public RHIIndexBuffer {
public:
	DX12IndexBuffer(uint32_t num, uint32_t stride, const void* data);

	D3D12_INDEX_BUFFER_VIEW GetView() {
		D3D12_INDEX_BUFFER_VIEW view;
		view.BufferLocation = GetResourceOwner()->GetResource()->GetIResource()->GetGPUVirtualAddress();;
		view.SizeInBytes = mNum * sizeof(std::uint16_t);
		view.Format = DXGI_FORMAT_R16_UINT;
		return view;
	}

	uint32_t GetSize() {
		return mNum;
	}

private:
	//DXGI_FORMAT format;
	uint32_t mNum;

	DX12ResourceOwner mUploader;
};

class DX12VertexBuffer : public DX12ShaderResource, public RHIVertexBuffer {
public:
	DX12VertexBuffer(uint32_t num, uint32_t stride, const void* data);

	D3D12_VERTEX_BUFFER_VIEW GetView() {
		D3D12_VERTEX_BUFFER_VIEW view;
		view.BufferLocation = GetResourceOwner()->GetResource()->GetIResource()->GetGPUVirtualAddress();
		view.SizeInBytes = mNum * mStride;
		view.StrideInBytes = mStride;
		return view;
	}

private:
	uint32_t mStride;
	uint32_t mNum;

	DX12ResourceOwner mUploader;
};

class DX12ConstantBuffer : public DX12ShaderResource, public RHIConstantBuffer {
public:
	DX12ConstantBuffer(uint32_t size);
	void CopyData(void *data, uint32_t size) override;

	DX12ConstantBufferView* GetView() {
		return mView.GetRef();
	}

private:
	RefCountPtr<DX12ConstantBufferView> mView;
	BYTE* mMappedBuffer;
	uint32_t mSize;
};



}