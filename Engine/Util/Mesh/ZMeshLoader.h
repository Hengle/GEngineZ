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
	static RenderMesh* Load(std::string const& meshFile) {
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
		RenderMesh* mesh = new RenderMesh(false);

		SubMeshFileHeader meshHeader;
		os.read((char*)& meshHeader, sizeof(SubMeshFileHeader));
		// semantic
		std::vector<ERHIInputSemantic> sems;
		for (int i = 0; i < meshHeader.SemnaticNum; i++) {
			ERHIInputSemantic f = (ERHIInputSemantic)meshHeader.Semanatics[i];
			sems.push_back(f);
		}
		mesh->SetVertexSemantics(sems);
		mesh->SetIndexStride(4);

		// index
		uint32_t totalIndexNum = 0;

		std::vector<uint8_t> indices;
		for (int i = 0; i < meshHeader.IndexNum; i++) {
			indices.resize(meshHeader.IndexCount[i] * mesh->GetIndexStride());

			os.read((char*)indices.data(), indices.size());
			mesh->CopyIndex(totalIndexNum * mesh->GetIndexStride(), indices.size(), indices.data(), i);

			totalIndexNum += meshHeader.IndexCount[i];
		}

		// vertex
		std::vector<uint8_t> vertexes;
		vertexes.resize(meshHeader.VertCount * mesh->GetVertexStride());
		os.read((char*)vertexes.data(), vertexes.size());

		mesh->CopyVertex(0, vertexes.size(), vertexes.data(), 0);

		mesh->Complete(1, meshHeader.IndexNum);
		return mesh;
	}

};




}