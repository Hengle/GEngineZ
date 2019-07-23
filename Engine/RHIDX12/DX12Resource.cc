#include "DX12Resource.h"
#include "DX12Device.h"
#include "DX12Command.h"

namespace z {

// DX12Resource
DX12Resource::DX12Resource(ID3D12Resource* res, D3D12_RESOURCE_STATES state, D3D12_RESOURCE_DESC const& desc) :
	mResource(res),
	mDesc(desc),
	mState(state) {
}

DX12Resource::DX12Resource(D3D12_HEAP_TYPE heapType, D3D12_RESOURCE_STATES state, D3D12_RESOURCE_DESC const& desc):
	DX12Resource(nullptr, state, desc) {
	DX12_CHECK(GDX12Device->GetIDevice()->CreateCommittedResource(
		&CD3DX12_HEAP_PROPERTIES(heapType),
		D3D12_HEAP_FLAG_NONE,
		&desc,
		state,
		nullptr,
		IID_PPV_ARGS(mResource.GetComRef())
	));
}

DX12Resource::~DX12Resource() {
}


void DX12Resource::Transition(D3D12_RESOURCE_STATES toState) {
	if (toState == GetState()) {
		Log<LWARN>("resource transition with same state.");
		return;
	}
	D3D12_RESOURCE_BARRIER barrier;
	barrier.Type = D3D12_RESOURCE_BARRIER_TYPE_TRANSITION;
	barrier.Flags = D3D12_RESOURCE_BARRIER_FLAG_NONE;
	barrier.Transition.pResource = GetIResource();
	barrier.Transition.StateBefore = GetState();
	barrier.Transition.StateAfter = toState;
	barrier.Transition.Subresource = D3D12_RESOURCE_BARRIER_ALL_SUBRESOURCES;

	SetState(toState);
	GDX12Device->GetCommandContext()->List()->ResourceBarrier(1, &barrier);
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