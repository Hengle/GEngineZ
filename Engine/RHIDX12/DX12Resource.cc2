#include "DX12Resource.h"

namespace z {

// DX12Resource
DX12Resource::DX12Resource(ID3D12Resource* res, D3D12_RESOURCE_STATES state, D3D12_RESOURCE_DESC const& desc) :
	mResource(res),
	mDesc(desc) {
	InitResourceState(state);
}

void DX12Resource::InitResourceState(D3D12_RESOURCE_STATES state) {
	// todo
}




// DX12ResourceOwner
DX12ResourceOwner::DX12ResourceOwner() :
	mOwnType(EResourceOwnType::EResourceOwn_Invaild),
	mResource(nullptr) {
}

DX12ResourceOwner::~DX12ResourceOwner() {
	if (mOwnType == EResourceOwnType::EResourceOwn_Exclusive) {
		CHECK(mResource->RefCount() == 1, "Invaild ref of EResourceOwn_Exclusive Resource");
		mResource->Release();
	}
}

void DX12ResourceOwner::OwnResource(const EResourceOwnType ownType, DX12Resource* resource) {
	mOwnType = ownType;
	mResource = resource;
	if (ownType == EResourceOwnType::EResourceOwn_Exclusive) {
		mResource->AddRef();
	}
}

}