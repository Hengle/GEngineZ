#pragma once
#include <Core/CoreHeader.h>
#include <Render/RenderConst.h>
#include <Render/Material.h>
#include <Render/Mesh.h>
#include <RHI/RHIDevice.h>

namespace z {

class RenderItem : public RefCounter{
public:
	ERenderSet RenderSet;

	math::Matrix4F WorldMatrix;
	RefCountPtr<RenderMesh> Mesh;
	RefCountPtr<MaterialInstance> Material;

public:
	RenderItem() :
		mMeshIndexGroup(0),
		mMeshVertexGroup(0),
		RenderSet(RSET_OPAQUE) {
	}

	void SetMeshIndexGroup(int idx) {
		mMeshIndexGroup = idx;
	}
	
	void SetMeshVertexGroup(int idx) {
		mMeshVertexGroup = idx;
	}

	void Draw() {
		auto [vb, ib] = Mesh->GetRHIResource();
		int num = Mesh->GetIndexCount(mMeshIndexGroup);
		int baseIndex = Mesh->GetIndexOffset(mMeshIndexGroup);
		int baseVertex = Mesh->GetVertexOffset(mMeshVertexGroup);

		GDevice->DrawIndexed(Material->mRHIShaderInstance, vb, ib, Material->mRState, num, baseIndex, baseVertex);
	}

	void CustomDraw(int indexNum, int indexOffset, int vertexOffset) {
		auto [vb, ib] = Mesh->GetRHIResource();
		int baseIndex = Mesh->GetIndexOffset(mMeshIndexGroup) + indexOffset;
		int baseVertex = Mesh->GetVertexOffset(mMeshVertexGroup) + vertexOffset;

		GDevice->DrawIndexed(Material->mRHIShaderInstance, vb, ib, Material->mRState, indexNum, baseIndex, baseVertex);
	}

private:
	int mMeshIndexGroup;
	int mMeshVertexGroup;
};

}