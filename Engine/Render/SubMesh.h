#pragma once
#include <Core/CoreHeader.h>
#include <Render/RenderConst.h>
#include <RHI/RHIResource.h>

namespace z {

class ZMeshLoader;
struct Mesh;


struct Mesh : RefCounter{
	Mesh() {}
	virtual ~Mesh() {}


	uint32_t VertCount{ 0 };
	uint32_t IndexCount{ 0 };
	uint32_t FaceCount{ 0 };
	uint32_t Stride{ 0 };
	std::vector<ERHIInputSemantic> Semantics;

	std::vector<uint16_t> Indices16;
	std::vector<uint32_t> Indices;
	std::vector<float> Vertexes;



	void UseBuffer();
	void CreateBuffer();

	RefCountPtr<RHIVertexBuffer> mVBuffer;
	RefCountPtr<RHIIndexBuffer> mIBuffer;

};

typedef std::vector<RefCountPtr<Mesh>> MeshHub;

}