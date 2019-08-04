#pragma once
#include <Core/CoreHeader.h>
#include <Render/RenderConst.h>
#include <Render/SubMesh.h>
#include <zlib/zstr.hpp>

namespace z {

struct SubMeshFileHeader {
	int FVF;
	int FVFSize;
	uint32_t VertCount;
	uint32_t FaceCount;
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

		Mesh* mesh = new Mesh();
		mesh->mMeshes.clear();

		// read submeshes
#ifdef COMPRESS_MESH_FILE
		zstr::ifstream os(meshFile, std::ios_base::binary);
#else
		std::ifstream os(meshFile, std::ios_base::binary);
#endif

		int submeshCnt = 0;;
		os.read((char*)& submeshCnt, sizeof(int));
		for (int i = 0; i < submeshCnt; i++) {
			SubMesh* subMesh = new SubMesh();

			SubMeshFileHeader meshHeader;
			os.read((char*)&meshHeader, sizeof(SubMeshFileHeader));

			subMesh->FVF = meshHeader.FVF;
			subMesh->FVFSize = meshHeader.FVFSize;
			subMesh->VertCount = meshHeader.VertCount;
			subMesh->FaceCount = meshHeader.FaceCount;
			subMesh->IndexCount = meshHeader.IndexCount;

			std::vector<uint32_t>& indices = subMesh->Indices;
			std::vector<float>& vertexes = subMesh->Vertexes;
			indices.resize(meshHeader.IndexCount);
			vertexes.resize(meshHeader.FVFSize * meshHeader.VertCount);
			os.read((char*)indices.data(), indices.size() * sizeof(uint32_t));
			os.read((char*)vertexes.data(), vertexes.size() * sizeof(float));

			mesh->mMeshes.push_back(subMesh);
		}
		return mesh;
	}

};




}