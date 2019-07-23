#pragma once
#include <Core/CoreHeader.h>
#include <RHI/RHIResource.h>
#include "DX12Resource.h"

namespace z {

class DX12BufferHelper {
public:
	template<typename BufferT>
	static BufferT* CreateWithUpload(uint32_t stride, uint32_t size, const void* data) {
		BufferT* buffer = new BufferT(stride, size);
		RefCountPtr<DX12Resource> resource = UploadBuffer(data, size);
		buffer->GetResourceOwner()->OwnResource(EResourceOwn_Exclusive, resource);
		return buffer;
	}

	template<typename BufferT>
	static BufferT* Create(const D3D12_RESOURCE_DESC& desc, uint32_t stride, uint32_t size) {}

	static RefCountPtr<DX12Resource> UploadBuffer(const void* data, size_t size);

};





class DX12IndexBuffer : public DX12ShaderResource, public RHIIndexBuffer {
public:
	DX12IndexBuffer(uint32_t stride, uint32_t size) {
	}

	D3D12_VERTEX_BUFFER_VIEW GetView() {
		return D3D12_VERTEX_BUFFER_VIEW{};
	}
};

class DX12VertexBuffer : public DX12ShaderResource, public RHIVertexBuffer {
public:
	DX12VertexBuffer(uint32_t stride, uint32_t size) {
	}

	D3D12_INDEX_BUFFER_VIEW GetView() {
		return D3D12_INDEX_BUFFER_VIEW{};
	}
};



}