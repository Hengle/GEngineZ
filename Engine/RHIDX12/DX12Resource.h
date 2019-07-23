#pragma once
#include "DX12Header.h"
#include <Core/CoreHeader.h>

namespace z {

class DX12Resource : public ThreadSafeRefCounter {
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

private:
	RefCountPtr<ID3D12Resource> mResource;
	D3D12_RESOURCE_DESC mDesc;
	D3D12_RESOURCE_STATES mState;

};

enum EResourceOwnType {
	EResourceOwn_Invaild = 0,
	EResourceOwn_Exclusive = 1,
};

class DX12ResourceOwner {
public:
	DX12ResourceOwner();
	~DX12ResourceOwner();

	void OwnResource(const EResourceOwnType ownType, DX12Resource* resource);

	DX12Resource* GetResource() {
		return mResource;
	}

private:
	EResourceOwnType mOwnType;
	DX12Resource *mResource;
};




// resource may be bound as shader resource
class DX12ShaderResource {
public:
	DX12ShaderResource() {

	}

	DX12ResourceOwner* GetResourceOwner() {
		return &mOwner;
	}

private:
	DX12ResourceOwner mOwner;

};


}