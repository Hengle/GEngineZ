#pragma once
#include <Core/CoreHeader.h>
#include <Render/RenderConst.h>
#include <RHI/RHIResource.h>

namespace z {

class ZMeshLoader;
struct Mesh;

typedef std::vector<EFVFormat> FVFs;


class MeshInstance : public RefCounter {
public:
	friend class Mesh;
	void UseBuffer();
	

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


struct Mesh : RefCounter{
	uint32_t VertCount{ 0 };
	uint32_t IndexCount{ 0 };
	uint32_t FaceCount{ 0 };
	uint32_t FVFStride{ 0 };
	std::vector<EFVFormat> FVFOrder;
	std::vector<uint32_t> FVFOffset;

	std::vector<uint32_t> Indices;
	std::vector<float> Vertexes;

	MeshInstance* GetMeshInstance(const FVFs&, int indexstride);

};

typedef std::vector<RefCountPtr<Mesh>> MeshHub;

}