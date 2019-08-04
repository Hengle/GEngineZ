#include <Core/CoreHeader.h>
#include <Util/Mesh/ZMeshLoader.h>

#include <zlib/zstr.hpp>
#include <assimp/Importer.hpp>
#include <assimp/scene.h>
#include <assimp/postprocess.h>

#include <vector>
#include <string>
#include <cstdint>
#include <fstream>
#include <sstream>

using namespace z;

class MeshLoader {
public:
	bool LoadMesh(std::string const& mesh_path) {
		Assimp::Importer importer;
		const aiScene* scn = importer.ReadFile(mesh_path, aiProcess_Triangulate | aiProcess_FlipUVs);

		if (!scn || scn->mFlags & AI_SCENE_FLAGS_INCOMPLETE || !scn->mRootNode) {
			Log<LERROR>("Load mesh failed");
			return false;
		}

		m_vs.clear();
		m_is.clear();
		m_total_vertex = 0;
		m_total_face = 0;

		ProcessNode(scn, scn->mRootNode);

		Log<LDEBUG>("Begin write mesh file");

		// write to binary stream
		std::string tgt_file = FilePath(mesh_path).FileNameNoExt() + ".zmesh";
#ifdef COMPRESS_MESH_FILE
		zstr::ofstream ss_bin(tgt_file, std::ios_base::binary);
#else
		std::ofstream ss_bin(tgt_file, std::ios_base::binary);
#endif
		int sz = static_cast<int>(m_is.size());
		ss_bin.write((char*)& sz, sizeof(int));

		for (size_t i = 0; i < m_is.size(); i++) {
			ss_bin.write((char*)& m_info[i], sizeof(z::SubMeshFileHeader));
			ss_bin.write((char*)m_is[i].data(), m_is[i].size() * sizeof(uint32_t));
			ss_bin.write((char*)m_vs[i].data(), m_vs[i].size() * sizeof(float));
			auto l = m_info[i];
		}

		ss_bin.flush();

		Log<LDEBUG>("Write mesh to", tgt_file, "Total Vertex", m_total_vertex, "Total face", m_total_face);
		return true;
	}


private:
	std::vector<std::vector<float>> m_vs;
	std::vector<std::vector<uint32_t>> m_is;
	std::vector<z::SubMeshFileHeader> m_info;
	int m_total_vertex;
	int m_total_face;

	void ProcessNode(const aiScene* scn, aiNode* root) {
		for (size_t i = 0; i < root->mNumMeshes; i++) {
			ProcessMesh(scn, scn->mMeshes[root->mMeshes[i]]);
		}

		for (size_t i = 0; i < root->mNumChildren; i++) {
			ProcessNode(scn, root->mChildren[i]);
		}
	}

	void ProcessMesh(const aiScene* scn, aiMesh* mesh) {
		std::vector<float> vs;
		std::vector<uint32_t> is;

		for (size_t i = 0; i < mesh->mNumVertices; i++) {
			// position
			if (mesh->HasPositions()) {
				vs.push_back(mesh->mVertices[i].x);
				vs.push_back(mesh->mVertices[i].y);
				vs.push_back(mesh->mVertices[i].z);
			}

			// tangents

			// normal
			if (mesh->HasNormals()) {
				vs.push_back(mesh->mNormals[i].x);
				vs.push_back(mesh->mNormals[i].y);
				vs.push_back(mesh->mNormals[i].z);
			}

			// uv1
			if (mesh->HasTextureCoords(0)) {
				vs.push_back(mesh->mTextureCoords[0][i].x);
				vs.push_back(mesh->mTextureCoords[0][i].y);
			}

			// uv2
			if (mesh->HasTextureCoords(1)) {
				vs.push_back(mesh->mTextureCoords[1][i].x);
				vs.push_back(mesh->mTextureCoords[1][i].y);
			}
		}

		// index
		for (size_t i = 0; i < mesh->mNumFaces; i++) {
			aiFace face = mesh->mFaces[i];

			for (size_t j = 0; j < face.mNumIndices; j++) {
				is.push_back(face.mIndices[j]);
			}
		}

		int fvf_sz = 0;
		int fvf = 0;

		if (mesh->HasPositions()) {
			fvf |= z::FVF_XYZ;
			fvf_sz += 3;
		}

		if (mesh->HasNormals()) {
			fvf |= z::FVF_NORMAL;
			fvf_sz += 3;
		}

		if (mesh->HasTextureCoords(0)) {
			fvf |= z::FVF_UV0;
			fvf_sz += 2;
		}

		if (mesh->HasTextureCoords(1)) {
			fvf |= z::FVF_UV1;
			fvf_sz += 2;
		}

		Log<LDEBUG>("Export mesh", mesh->HasPositions(), mesh->HasNormals(), mesh->HasTextureCoords(0), mesh->HasTextureCoords(1),
			"Vertex count", mesh->mNumVertices, "Face count", mesh->mNumFaces);

		m_vs.push_back(vs);
		m_is.push_back(is);
		m_info.push_back(z::SubMeshFileHeader{
			fvf, fvf_sz, mesh->mNumVertices, mesh->mNumFaces, static_cast<uint32_t>(is.size())});
		m_total_vertex += mesh->mNumVertices;
		m_total_face += mesh->mNumFaces;
	}

};
int main(int argc, char* argv[]) {
	if (argc < 2) {
		std::cout << "meshconveter mesh.obj";
		return 0;
	}
	z::FilePath f(argv[1]);
	f.ToAbsolute();
	if (!f.IsExist()) {
		Log<LERROR>("File Not Exist");
		return 0;
	}
	MeshLoader().LoadMesh(f);
	return 0;
}