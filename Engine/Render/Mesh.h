#pragma once
#include <Core/CoreHeader.h>
#include <Render/RenderConst.h>
#include <RHI/RHIResource.h>

namespace z {

class ZMeshLoader;
struct Mesh;


struct Mesh : RefCounter {
	Mesh() {}
	virtual ~Mesh();

	std::vector<ERHIInputSemantic> Semantics;

	uint32_t Stride{ 0 };
	uint32_t VertCount{ 0 };
	std::vector<float> Vertexes;

	std::vector<std::vector<uint32_t>> Indices;

	void UseBuffer(int idx);
	void CreateBuffer();

	RefCountPtr<RHIVertexBuffer> mVBuffer;
	std::vector<RefCountPtr<RHIIndexBuffer>> mIBuffer;

};

typedef std::vector<RefCountPtr<Mesh>> MeshHub;

}