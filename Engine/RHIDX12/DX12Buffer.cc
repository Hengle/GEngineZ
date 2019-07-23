#include "DX12Buffer.h"
#include "DX12Device.h"
#include "DX12Command.h"

namespace z {

RefCountPtr<DX12Resource> DX12BufferHelper::UploadBuffer(const void* data, size_t bufSize) {
	auto cmdContext = GDX12Device->GetCommandContext();

	RefCountPtr<DX12Resource> uploader, dest;
	D3D12_RESOURCE_DESC destDesc = CD3DX12_RESOURCE_DESC::Buffer(bufSize);
	dest = new DX12Resource(D3D12_HEAP_TYPE_DEFAULT, D3D12_RESOURCE_STATE_COMMON, destDesc);
	uploader = new DX12Resource(D3D12_HEAP_TYPE_UPLOAD, D3D12_RESOURCE_STATE_GENERIC_READ, destDesc);

	D3D12_SUBRESOURCE_DATA subResourceData = {};
	subResourceData.pData = data;
	subResourceData.RowPitch = bufSize;
	subResourceData.SlicePitch = subResourceData.RowPitch;

	dest->Transition(D3D12_RESOURCE_STATE_COPY_DEST);
	UpdateSubresources<1>(cmdContext->List().GetCommandList(), dest->GetIResource(), uploader->GetIResource(), 0, 0, 1, &subResourceData);
	dest->Transition(D3D12_RESOURCE_STATE_GENERIC_READ);
	return dest;
}

}