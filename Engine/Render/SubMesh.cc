#include "SubMesh.h"
#include <RHI/RHIDevice.h>

namespace z {

void MeshInstance::UseBuffer() {
	GDevice->SetVertexBuffer(mVBuffer);
	GDevice->SetIndexBuffer(mIBuffer);
}

void MeshInstance::CreateBuffer() {
	mVBuffer = GDevice->CreateVertexBuffer(mVertexes.size() / (mVertexStride / 4), mVertexStride, mVertexes.data());
	if (mIndexStride == 2) {
		mIBuffer = GDevice->CreateIndexBuffer(mIndices16.size(), 2, mIndices16.data());
	} else {
		CHECK(0);
	}
}

MeshInstance* Mesh::GetMeshInstance(const FVFs& fvfs, int indexstride) {
	MeshInstance* inst = new MeshInstance();
	
	// calc instance offset
	constexpr int ArraySize = FVF_MAX + 1;
	int meshOffset[ArraySize], instOffset[ArraySize], instStride = 0;
	memset(instOffset, -1, ArraySize * sizeof(int));
	memset(meshOffset, -1, ArraySize * sizeof(int));
	for (int i = 0; i < fvfs.size(); i++) {
		instOffset[fvfs[i]] = instStride;
		instStride += GetFVFSize(fvfs[i]);
	}
	for (int i = 0; i < FVFOrder.size(); i++) {
		meshOffset[FVFOrder[i]] = FVFOffset[i];
	}
	// copy
	inst->mVertexes.resize(VertCount * instStride / 4);
	inst->mVertexStride = instStride;
	for (int i = 0; i < VertCount; i++) {
		for (int j = 0; j < fvfs.size(); j++) {
			EFVFormat f = fvfs[j];
			if (meshOffset[f] >= 0) {
				int oidx = FVFStride * i + meshOffset[f];
				int nidx = instStride * i + instOffset[f];
				memcpy((char*)inst->mVertexes.data() + nidx, (char*)Vertexes.data() + oidx, GetFVFSize(f));
			} else {
				int nidx = instStride * i + instOffset[f];
				memset((char*)inst->mVertexes.data() + nidx, 0, GetFVFSize(f));
			}
			
		}
	}

	// index
	if (indexstride == 2) {
		inst->mIndexStride = 2;
		inst->mIndices16.resize(IndexCount);
		for (size_t i = 0; i < IndexCount; i++) {
			inst->mIndices16[i] = static_cast<uint16_t>(Indices[i]);
		}
	} else {
		CHECK(0);
	}

	inst->CreateBuffer();
	return inst;
}


}