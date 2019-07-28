#include "DX12Buffer.h"
#include "DX12Device.h"
#include "DX12Executor.h"
#include "DX12View.h"

namespace z {

static RefCountPtr<DX12Resource> UploadBuffer(DX12ResourceOwner &uploadOnwer, uint32_t bufSize, const void* data) {
	auto exec = GDX12Device->GetExecutor();

	RefCountPtr<DX12Resource> uploader, dest;
	D3D12_RESOURCE_DESC destDesc = CD3DX12_RESOURCE_DESC::Buffer(bufSize);
	dest = new DX12Resource(D3D12_HEAP_TYPE_DEFAULT, D3D12_RESOURCE_STATE_COMMON, destDesc);
	uploader = new DX12Resource(D3D12_HEAP_TYPE_UPLOAD, D3D12_RESOURCE_STATE_GENERIC_READ, destDesc);

	D3D12_SUBRESOURCE_DATA subResourceData = {};
	subResourceData.pData = data;
	subResourceData.RowPitch = bufSize;
	subResourceData.SlicePitch = subResourceData.RowPitch;

	dest->Transition(D3D12_RESOURCE_STATE_COPY_DEST);
	UpdateSubresources<1>(exec->GetCommandList(), dest->GetIResource(), uploader->GetIResource(), 0, 0, 1, &subResourceData);
	dest->Transition(D3D12_RESOURCE_STATE_GENERIC_READ);
	uploadOnwer.OwnResource(EResourceOwn_Exclusive, uploader);
	return dest;
}

// DX12 Index Buffer
DX12IndexBuffer::DX12IndexBuffer(uint32_t num, uint32_t stride, const void* data) :
	mNum(num) {
	assert(stride == 2);
	RefCountPtr<DX12Resource> resource = UploadBuffer(mUploader, num * sizeof(uint16_t), data);
	GetResourceOwner()->OwnResource(EResourceOwn_Exclusive, resource);

}


// DX12 Vertex Buffer
DX12VertexBuffer::DX12VertexBuffer(uint32_t num, uint32_t stride, const void* data) :
	mNum(num),
	mStride(stride) {
	RefCountPtr<DX12Resource> resource = UploadBuffer(mUploader, num * stride, data);
	GetResourceOwner()->OwnResource(EResourceOwn_Exclusive, resource);
}

// Constant Buffer

DX12ConstantBuffer::DX12ConstantBuffer(uint32_t size) :
	mMappedBuffer(nullptr),
	mSize(size) {
	// alignment
	size = ((size + 255) & ~255);

	// create reousrce and hold it
	D3D12_RESOURCE_DESC resDesc = CD3DX12_RESOURCE_DESC::Buffer(size);
	RefCountPtr<DX12Resource> resource = new DX12Resource(D3D12_HEAP_TYPE_UPLOAD, D3D12_RESOURCE_STATE_GENERIC_READ, resDesc);
	GetResourceOwner()->OwnResource(EResourceOwn_Exclusive, resource);

	// create view and hold it
	D3D12_GPU_VIRTUAL_ADDRESS gpuAddress = resource->GetIResource()->GetGPUVirtualAddress();
	D3D12_CONSTANT_BUFFER_VIEW_DESC cbvDesc;
	cbvDesc.BufferLocation = gpuAddress;
	cbvDesc.SizeInBytes = size;
	mView = new DX12ConstantBufferView(cbvDesc);
	
	// mapped memory
	resource->GetIResource()->Map(0, nullptr, (void**)& mMappedBuffer); 
}


void DX12ConstantBuffer::CopyData(void* data, uint32_t size) {
	CHECK(size <= mSize);
	memcpy(mMappedBuffer, data, size);
}


}