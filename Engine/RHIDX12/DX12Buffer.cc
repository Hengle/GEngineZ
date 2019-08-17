#include "DX12Buffer.h"
#include "DX12Device.h"
#include "DX12Executor.h"
#include "DX12View.h"

namespace z {

std::vector<RefCountPtr<DX12Resource>> DX12BufferUploader::mUploadingBufers;

void DX12BufferUploader::UploadBuffer(DX12Resource* dest, const void* data, uint32_t bufSize) {
	auto exec = GDX12Device->GetExecutor();

	D3D12_RESOURCE_DESC uploaderDesc = CD3DX12_RESOURCE_DESC::Buffer(bufSize);
	DX12Resource* uploader = new DX12Resource(D3D12_HEAP_TYPE_UPLOAD, D3D12_RESOURCE_STATE_GENERIC_READ, uploaderDesc);

	D3D12_SUBRESOURCE_DATA subResourceData = {};
	subResourceData.pData      = data;
	subResourceData.RowPitch   = bufSize;
	subResourceData.SlicePitch = subResourceData.RowPitch;

	dest->Transition(D3D12_RESOURCE_STATE_COPY_DEST);
	UpdateSubresources<1>(exec->GetCommandList(), dest->GetIResource(), uploader->GetIResource(), 0, 0, 1, &subResourceData);
	dest->Transition(D3D12_RESOURCE_STATE_GENERIC_READ);
	mUploadingBufers.push_back(uploader);
}


void DX12BufferUploader::UploadTexture(DX12Resource* dest, const void* data, uint32_t width, uint32_t height, uint32_t pixelsize) {
	auto exec = GDX12Device->GetExecutor();

	// create uploader resource
	const uint64_t size = GetRequiredIntermediateSize(dest->GetIResource(), 0, 1);
	D3D12_RESOURCE_DESC uploaderDesc = CD3DX12_RESOURCE_DESC::Buffer(size);
	DX12Resource* uploader = new DX12Resource(D3D12_HEAP_TYPE_UPLOAD, D3D12_RESOURCE_STATE_GENERIC_READ, uploaderDesc);

	D3D12_SUBRESOURCE_DATA subResourceData = {};
	subResourceData.pData = data;
	subResourceData.RowPitch = (uint64_t)width* pixelsize;
	subResourceData.SlicePitch = (uint64_t)height * width * pixelsize;

	dest->Transition(D3D12_RESOURCE_STATE_COPY_DEST);
	UpdateSubresources<1>(exec->GetCommandList(), dest->GetIResource(), uploader->GetIResource(), 0, 0, 1, &subResourceData);
	dest->Transition(D3D12_RESOURCE_STATE_GENERIC_READ);
	mUploadingBufers.push_back(uploader);
}

void DX12BufferUploader::Clear() {
	mUploadingBufers.clear();
}


// DX12 Index Buffer
DX12IndexBuffer::DX12IndexBuffer(uint32_t num, uint8_t stride, const void* data, bool dynamic) :
	mNum(num),
	mStride(stride) {
	CHECK(stride == 4|| stride == 2);

	uint32_t totolSize = num * stride;
	D3D12_RESOURCE_DESC destDesc = CD3DX12_RESOURCE_DESC::Buffer(totolSize);
	if (dynamic) {
		mResource = new DX12Resource(D3D12_HEAP_TYPE_UPLOAD, D3D12_RESOURCE_STATE_GENERIC_READ, destDesc);
		if (data) {
			// upload data
			void* addr = MapBuffer();
			memcpy(addr, data, totolSize);
			UnMapBuffer();
		}
	} else {
		CHECK(data);
		mResource = new DX12Resource(D3D12_HEAP_TYPE_DEFAULT, D3D12_RESOURCE_STATE_COMMON, destDesc);
		DX12BufferUploader::UploadBuffer(mResource, data, totolSize);
	}
}


// DX12 Vertex Buffer
DX12VertexBuffer::DX12VertexBuffer(uint32_t num, const std::vector<ERHIInputSemantic>& semantic, const void* data, bool dynamic) :
	mNum(num) {
	mSemantics = semantic;
	memset(mSemanticsOffset, 0, sizeof(uint8_t) * SEMANTIC_MAX);

	// calculate stride from semantic
	mStride = 0;
	for (size_t i = 0; i < semantic.size(); i++) {
		mSemanticsOffset[semantic[i]] = mStride;
		mStride += GetSemanticSize(semantic[i]);
	}

	uint32_t totolSize = num * mStride;
	D3D12_RESOURCE_DESC destDesc = CD3DX12_RESOURCE_DESC::Buffer(totolSize);
	if (dynamic) {
		mResource = new DX12Resource(D3D12_HEAP_TYPE_UPLOAD, D3D12_RESOURCE_STATE_GENERIC_READ, destDesc);
		if (data) {
			// upload data
			void *addr = MapBuffer();
			memcpy(addr, data, totolSize);
			UnMapBuffer();
		}
	} else {
		CHECK(data);
		mResource = new DX12Resource(D3D12_HEAP_TYPE_DEFAULT, D3D12_RESOURCE_STATE_COMMON, destDesc);
		DX12BufferUploader::UploadBuffer(mResource, data, totolSize);
	}
}


// Constant Buffer
DX12ConstantBuffer::DX12ConstantBuffer(uint32_t size) :
	mMappedBuffer(nullptr),
	mSize(size) {
	// alignment
	size = ((size + 255) & ~255);

	// create reousrce
	D3D12_RESOURCE_DESC resDesc = CD3DX12_RESOURCE_DESC::Buffer(size);
	mResource = new DX12Resource(D3D12_HEAP_TYPE_UPLOAD, D3D12_RESOURCE_STATE_GENERIC_READ, resDesc);
	mResource->GetIResource()->Map(0, nullptr, (void**)&mMappedBuffer);

	// create view
	D3D12_GPU_VIRTUAL_ADDRESS gpuAddress = mResource->GetIResource()->GetGPUVirtualAddress();
	D3D12_CONSTANT_BUFFER_VIEW_DESC cbvDesc;
	cbvDesc.BufferLocation = gpuAddress;
	cbvDesc.SizeInBytes = size;
	mView = new DX12ConstantBufferView(cbvDesc);
}


void DX12ConstantBuffer::CopyData(const void* data, uint32_t offset, uint32_t size) {
	CHECK(size <= mSize);
	memcpy(mMappedBuffer + offset, data, size);
}


}