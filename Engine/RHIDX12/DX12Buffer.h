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
	DX12IndexBuffer(uint32_t num, uint8_t stride, const void* data, bool dynamic);

	D3D12_INDEX_BUFFER_VIEW GetView() {
		D3D12_INDEX_BUFFER_VIEW view;
		view.BufferLocation = mResource->GetIResource()->GetGPUVirtualAddress();
		view.SizeInBytes = mNum * mStride;
		view.Format = mStride == 2 ? DXGI_FORMAT_R16_UINT : DXGI_FORMAT_R32_UINT;
		return view;
	}

	void* MapBuffer() override {
		return MapResource();
	}

	void UnMapBuffer() override {
		UnMapResource();
	}

	uint32_t GetBufferSize() override {
		return mNum * mStride;
	}

	uint32_t GetSize() {
		return mNum;
	}

private:
	uint8_t mStride;
	uint32_t mNum;
};

class DX12VertexBuffer : public DX12ShaderResource, public RHIVertexBuffer {
public:
	friend class DX12Device;

	DX12VertexBuffer(uint32_t num, const std::vector<ERHIInputSemantic>& semantic, const void* data, bool dynamic);

	virtual ~DX12VertexBuffer() {
		mSemantics.clear();
	}

	void* MapBuffer() override {
		return MapResource();
	}

	void UnMapBuffer() override {
		UnMapResource();
	}

	uint32_t GetBufferSize() override {
		return mNum * mStride;
	}


	D3D12_VERTEX_BUFFER_VIEW GetView() {
		D3D12_VERTEX_BUFFER_VIEW view;
		view.BufferLocation = mResource->GetIResource()->GetGPUVirtualAddress();
		view.SizeInBytes = mNum * mStride;
		view.StrideInBytes = mStride;
		return view;
	}

private:
	uint8_t mSemanticsOffset[SEMANTIC_MAX];
	uint8_t mStride;
	std::vector<ERHIInputSemantic> mSemantics;
	uint32_t mNum;

	bool mLocked;
};

class DX12ConstantBuffer : public DX12ShaderResource, public RefCounter {
public:
	DX12ConstantBuffer(uint32_t size);
	void CopyData(const void *data, uint32_t offset, uint32_t size);
	const void* GetData(uint32_t offset);

	DX12ConstantBufferView* GetView() {
		return mView;
	}

private:
	RefCountPtr<DX12ConstantBufferView> mView;

	BYTE* mMappedBuffer;
	uint32_t mSize;
};



}