#pragma once
#include <Core/CoreHeader.h>
#include <Render/RenderConst.h>
#include <RHI/RHIResource.h>

namespace z {

class ZMeshLoader;
class Mesh;

class SubMeshInstance : public RefCounter {
public:
	friend class Mesh;

	void Use();

private:
	void CreateBuffer();

	int mVertexStride;
	int mIndexStride;
	RefCountPtr<RHIVertexBuffer> mVBuffer;
	RefCountPtr<RHIIndexBuffer> mIBuffer;

	std::vector<uint16_t> mIndices16;
	std::vector<uint32_t> mIndices32;
	std::vector<float> mVertexes;

};

struct SubMesh {
	int FVF;
	int FVFSize;
	uint32_t VertCount;
	uint32_t FaceCount;
	uint32_t IndexCount;

	std::vector<uint32_t> Indices;
	std::vector<float> Vertexes;
};

class Mesh {
public:
	friend class ZMeshLoader;

	SubMesh* GetSubMesh(int index) {
		return mMeshes[index];
	}

	SubMeshInstance* GetSubMeshInstance(int index, const std::vector<EFVFormat>&, int indexstride);

	int GetSubMeshNum() {
		return mMeshes.size();
	}

private:
	std::vector<SubMesh*> mMeshes;
};


}