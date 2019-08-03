#pragma once
#include "DX12Header.h"
#include <Core/CoreHeader.h>

namespace z {

class DX12Resource : public RefCounter {
public:
	// new from exsit resource
	DX12Resource(ID3D12Resource* res, D3D12_RESOURCE_STATES state, D3D12_RESOURCE_DESC const& desc);
	// new from heap create
	DX12Resource(D3D12_HEAP_TYPE heapType, D3D12_RESOURCE_STATES state, D3D12_RESOURCE_DESC const& desc);

	virtual ~DX12Resource();

	void Transition(D3D12_RESOURCE_STATES toState);

	void SetState(D3D12_RESOURCE_STATES state) {
		mState = state;
	}

	D3D12_RESOURCE_STATES GetState() {
		return mState;
	}

	ID3D12Resource* GetIResource() {
		return mResource.GetRef();
	}

	const D3D12_RESOURCE_DESC& GetDesc() {
		return mDesc;
	}

private:
	RefCountPtr<ID3D12Resource> mResource;
	D3D12_RESOURCE_DESC mDesc;
	D3D12_RESOURCE_STATES mState;

};


// resource may be bound as shader resource
class DX12ShaderResource {
public:
	DX12ShaderResource() {}

	DX12Resource* GetResource() {
		return mResource;
	}

protected:
	RefCountPtr<DX12Resource> mResource;

};


}