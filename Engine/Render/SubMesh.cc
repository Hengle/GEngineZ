#include "SubMesh.h"
#include <RHI/RHIDevice.h>

namespace z {

void SubMeshInstance::Use() {
	GDevice->SetVertexBuffer(mVBuffer);
	GDevice->SetIndexBuffer(mIBuffer);
}

void SubMeshInstance::CreateBuffer() {
	mVBuffer = GDevice->CreateVertexBuffer(mVertexes.size() / (mVertexStride / 4), mVertexStride, mVertexes.data());
	if (mIndexStride == 2) {
		mIBuffer = GDevice->CreateIndexBuffer(mIndices16.size(), 2, mIndices16.data());
	} else {
		CHECK(0);
	}
}

SubMeshInstance* Mesh::GetSubMeshInstance(int index, const std::vector<EFVFormat>& fvfs, int indexstride) {
	SubMeshInstance* inst = new SubMeshInstance();
	SubMesh* subMesh = GetSubMesh(index);
	// calc submesh fvf meshOffset
	int meshOffset[FVF_MAX + 1], meshLen = 0;
	memset(meshOffset, 0, (FVF_MAX + 1) * sizeof(int));
	if (subMesh->FVF & FVF_XYZ) {
		meshOffset[FVF_XYZ] = meshLen;
		meshLen += GetFVFSize(FVF_XYZ);
	}
	if (subMesh->FVF & FVF_NORMAL) {
		meshOffset[FVF_NORMAL] = meshLen;
		meshLen += GetFVFSize(FVF_NORMAL);
	}
	if (subMesh->FVF & FVF_UV0) {
		meshOffset[FVF_UV0] = meshLen;
		meshLen += GetFVFSize(FVF_UV0);
	}
	if (subMesh->FVF & FVF_UV1) {
		meshOffset[FVF_UV1] = meshLen;
		meshLen += GetFVFSize(FVF_UV1);
	}
	// calc instance offset
	int instOffset[FVF_MAX + 1], instLen = 0;
	memset(instOffset, 0, (FVF_MAX + 1) * sizeof(int));
	for (int i = 0; i < fvfs.size(); i++) {
		instOffset[fvfs[i]] = instLen;
		instLen += GetFVFSize(fvfs[i]);
	}
	// copy
	inst->mVertexes.resize(subMesh->VertCount * instLen / 4);
	inst->mVertexStride = instLen;
	for (int i = 0; i < subMesh->VertCount; i++) {
		for (int j = 0; j < fvfs.size(); j++) {
			EFVFormat fvf = fvfs[j];
			int oidx = meshLen * i + meshOffset[fvf];
			int nidx = instLen * i + instOffset[fvf];
			memcpy((char*)inst->mVertexes.data() + oidx, (char*)subMesh->Vertexes.data() + nidx, GetFVFSize(fvf));
		}
	}

	// index
	if (indexstride == 2) {
		inst->mIndexStride = 2;
		inst->mIndices16.resize(subMesh->IndexCount);
		for (size_t i = 0; i < subMesh->IndexCount; i++) {
			inst->mIndices16[i] = static_cast<uint16_t>(subMesh->Indices[i]);
		}
	} else {
		CHECK(0);
	}

	inst->CreateBuffer();
	return inst;
}


}