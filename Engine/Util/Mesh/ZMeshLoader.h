#pragma once
#include <Core/CoreHeader.h>
#include <Render/RenderConst.h>
#include <Render/SubMesh.h>
#include <zlib/zstr.hpp>

namespace z {

struct SubMeshFileHeader {
	int FVFNum;
	int FVFOrder[MAX_FVF_NUM];
	uint32_t VertCount;
	uint32_t FaceCount;
	uint32_t IndexCount;
};


class ZMeshLoader {
public:
	static MeshHub Load(std::string const& meshFile) {
		MeshHub meshHub;

		FilePath f(meshFile);
		f.ToAbsolute();
		if (!f.IsExist()) {
			Log<LERROR>("mesh file not exist");
			return meshHub;
		}

		// read submeshes
#ifdef COMPRESS_MESH_FILE
		zstr::ifstream os(meshFile, std::ios_base::binary);
#else
		std::ifstream os(meshFile, std::ios_base::binary);
#endif

		int submeshCnt = 0;;
		os.read((char*)& submeshCnt, sizeof(int));
		for (int i = 0; i < submeshCnt; i++) {
			Mesh* mesh = new Mesh();

			SubMeshFileHeader meshHeader;
			os.read((char*)&meshHeader, sizeof(SubMeshFileHeader));

			mesh->VertCount = meshHeader.VertCount;
			mesh->FaceCount = meshHeader.FaceCount;
			mesh->IndexCount = meshHeader.IndexCount;
			mesh->FVFStride = 0;
			for (int i = 0; i < meshHeader.FVFNum; i++) {
				EFVFormat f = (EFVFormat)meshHeader.FVFOrder[i];
				mesh->FVFOrder.push_back(f);
				mesh->FVFOffset.push_back(mesh->FVFStride);
				mesh->FVFStride += GetFVFSize(f);
			}

			std::vector<uint32_t>& indices = mesh->Indices;
			std::vector<float>& vertexes = mesh->Vertexes;
			indices.resize(meshHeader.IndexCount);
			vertexes.resize(mesh->FVFStride * meshHeader.VertCount / 4);
			os.read((char*)indices.data(), indices.size() * sizeof(uint32_t));
			os.read((char*)vertexes.data(), vertexes.size() * sizeof(float));

			meshHub.push_back(mesh);
		}
		return meshHub;
	}

};




}