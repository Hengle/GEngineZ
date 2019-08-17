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
	std::vector<uint32_t> Indices;

	std::vector<uint32_t> NumIndices;
	std::vector<uint32_t> BaseIndices;
	
	void CreateBuffer();

	RefCountPtr<RHIVertexBuffer> mVBuffer;
	RefCountPtr<RHIIndexBuffer> mIBuffer;

};

}