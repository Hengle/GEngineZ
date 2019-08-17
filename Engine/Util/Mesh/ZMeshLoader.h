#pragma once
#include <Core/CoreHeader.h>
#include <RHI/RHIUtil.h>
#include <Render/RenderConst.h>
#include <Render/Mesh.h>
#include <zlib/zstr.hpp>

namespace z {

struct SubMeshFileHeader {
	uint8_t Semanatics[20];
	int SemnaticNum;
	uint32_t VertCount;
	uint32_t IndexCount[20];
	uint32_t IndexNum;
};


class ZMeshLoader {
public:
	static Mesh* Load(std::string const& meshFile) {
		FilePath f(meshFile);
		f.ToAbsolute();
		if (!f.IsExist()) {
			Log<LERROR>("mesh file not exist");
			return nullptr;
		}

		// read submeshes
#ifdef COMPRESS_MESH_FILE
		zstr::ifstream os(meshFile, std::ios_base::binary);
#else
		std::ifstream os(meshFile, std::ios_base::binary);
#endif
		Mesh* mesh = new Mesh();

		SubMeshFileHeader meshHeader;
		os.read((char*)& meshHeader, sizeof(SubMeshFileHeader));
		// semantic
		mesh->Stride = 0;
		mesh->Semantics.clear();
		for (int i = 0; i < meshHeader.SemnaticNum; i++) {
			ERHIInputSemantic f = (ERHIInputSemantic)meshHeader.Semanatics[i];
			mesh->Semantics.push_back(f);
			mesh->Stride += GetSemanticSize(f);
		}
		// index
		uint32_t totalIndexNum = 0;
		for (int i = 0; i < meshHeader.IndexNum; i++) {
			mesh->NumIndices.push_back(meshHeader.IndexCount[i]);
			mesh->BaseIndices.push_back(totalIndexNum);
			totalIndexNum += meshHeader.IndexCount[i];
		}
		mesh->Indices.resize(totalIndexNum);
		os.read((char*)mesh->Indices.data(), mesh->Indices.size() * sizeof(uint32_t));

		// vertex
		mesh->VertCount = meshHeader.VertCount;
		std::vector<float>& vertexes = mesh->Vertexes;
		vertexes.resize(mesh->Stride * mesh->VertCount / 4);
		os.read((char*)vertexes.data(), vertexes.size() * sizeof(float));

		mesh->CreateBuffer();
		return mesh;
	}

};




}