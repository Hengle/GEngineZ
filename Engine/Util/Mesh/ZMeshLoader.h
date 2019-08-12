#pragma once
#include <Core/CoreHeader.h>
#include <RHI/RHIUtil.h>
#include <Render/RenderConst.h>
#include <Render/Mesh.h>
#include <zlib/zstr.hpp>

namespace z {

struct SubMeshFileHeader {
	uint8_t Semanatics[SEMANTIC_MAX];
	int SemnaticNum;
	uint32_t VertCount;
	uint32_t IndexCount;
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

		int submeshCnt = 0, vertexCnt = 0;
		os.read((char*)& submeshCnt, sizeof(int));
		for (int i = 0; i < submeshCnt; i++) {
			SubMeshFileHeader meshHeader;
			os.read((char*)&meshHeader, sizeof(SubMeshFileHeader));

			mesh->VertCount = meshHeader.VertCount;
			mesh->IndicesCount.push_back(meshHeader.IndexCount);
			mesh->Stride = 0;
			for (int i = 0; i < meshHeader.SemnaticNum; i++) {
				ERHIInputSemantic f = (ERHIInputSemantic)meshHeader.Semanatics[i];
				mesh->Semantics.push_back(f);
				mesh->Stride += GetSemanticSize(f);
			}
			mesh->Indices.push_back(std::vector<uint32_t>());
			std::vector<uint32_t>& indices = mesh->Indices[i];
			indices.resize(meshHeader.IndexCount);
			os.read((char*)indices.data(), indices.size() * sizeof(uint32_t));
		
			vertexCnt += mesh->VertCount;
		}

		std::vector<float>& vertexes = mesh->Vertexes;
		vertexes.resize(mesh->Stride * vertexCnt / 4);
		os.read((char*)vertexes.data(), vertexes.size() * sizeof(float));

		mesh->CreateBuffer();
		return mesh;
	}

};




}