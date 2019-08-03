#pragma once
#include <Core/CoreHeader.h>
#include <RHI/RHIResource.h>
#include "DX12Resource.h"
#include "DX12View.h"

namespace z {

class DX12BufferUploader {
public:
	static void UploadBuffer(DX12Resource *dest, const void* data, uint32_t bufSize);
	static void UploadTexture(DX12Resource* dest, const void* data, uint32_t width, uint32_t hieght, uint32_t pixelsize);

	// clear when frame end
	static void Clear();
private:
	static std::vector<RefCountPtr<DX12Resource>> mUploadingBufers;
};


class DX12IndexBuffer : public DX12ShaderResource, public RHIIndexBuffer {
public:
	DX12IndexBuffer(uint32_t num, uint32_t stride, const void* data);

	D3D12_INDEX_BUFFER_VIEW GetView() {
		D3D12_INDEX_BUFFER_VIEW view;
		view.BufferLocation = mResource->GetIResource()->GetGPUVirtualAddress();;
		view.SizeInBytes = mNum * sizeof(std::uint16_t);
		view.Format = DXGI_FORMAT_R16_UINT;
		return view;
	}

	uint32_t GetSize() {
		return mNum;
	}

private:
	uint32_t mNum;
};

class DX12VertexBuffer : public DX12ShaderResource, public RHIVertexBuffer {
public:
	DX12VertexBuffer(uint32_t num, uint32_t stride, const void* data);

	D3D12_VERTEX_BUFFER_VIEW GetView() {
		D3D12_VERTEX_BUFFER_VIEW view;
		view.BufferLocation = mResource->GetIResource()->GetGPUVirtualAddress();
		view.SizeInBytes = mNum * mStride;
		view.StrideInBytes = mStride;
		return view;
	}

private:
	uint32_t mStride;
	uint32_t mNum;
};

class DX12ConstantBuffer : public DX12ShaderResource, public RefCounter {
public:
	DX12ConstantBuffer(uint32_t size);
	void CopyData(const void *data, uint32_t offset, uint32_t size);

	DX12ConstantBufferView* GetView() {
		return mView;
	}

private:
	RefCountPtr<DX12ConstantBufferView> mView;

	BYTE* mMappedBuffer;
	uint32_t mSize;
};



}