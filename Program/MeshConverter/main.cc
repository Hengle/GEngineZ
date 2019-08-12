#include <Core/CoreHeader.h>
#include <RHI/RHIConst.h>
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

		mVS.clear();
		mIS.clear();
		mTotalVertex = 0;
		mTotalFace = 0;

		ProcessNode(scn, scn->mRootNode);

		Log<LDEBUG>("Begin write mesh file");

		// write to binary stream
		std::string tgt_file = FilePath(mesh_path).FileNameNoExt() + ".zmesh";
#ifdef COMPRESS_MESH_FILE
		zstr::ofstream ss_bin(tgt_file, std::ios_base::binary);
#else
		std::ofstream ss_bin(tgt_file, std::ios_base::binary);
#endif
		int sz = static_cast<int>(mIS.size());
		ss_bin.write((char*)& sz, sizeof(int));

		int base = 0;
		for (size_t i = 0; i < mIS.size(); i++) {
			ss_bin.write((char*)& mInfo[i], sizeof(z::SubMeshFileHeader));
			for (size_t j = 0; j < mIS[i].size(); j++) {
				mIS[i][j] += base;
			}
			base += mInfo[i].VertCount;

			ss_bin.write((char*)mIS[i].data(), mIS[i].size() * sizeof(uint32_t));
			auto l = mInfo[i];
		}

		for (size_t i = 0; i < mVS.size(); i++) {
			ss_bin.write((char*)mVS[i].data(), mVS[i].size() * sizeof(float));
		}

		ss_bin.flush();

		Log<LDEBUG>("Write mesh to", tgt_file, "Total Vertex", mTotalVertex, "Total face", mTotalFace);
		return true;
	}


private:
	std::vector<std::vector<float>> mVS;
	std::vector<std::vector<uint32_t>> mIS;
	std::vector<z::SubMeshFileHeader> mInfo;
	int mTotalVertex{ 0 };
	int mTotalFace{ 0 };

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
		z::SubMeshFileHeader header;
		header.VertCount = mesh->mNumVertices;
		header.IndexCount = is.size();

		header.SemnaticNum = 0;
		if (mesh->HasPositions()) {
			header.Semanatics[header.SemnaticNum++] = SEMANTIC_POSITION;
		}

		if (mesh->HasNormals()) {
			header.Semanatics[header.SemnaticNum++] = SEMANTIC_NORMAL;
		}

		if (mesh->HasTextureCoords(0)) {
			header.Semanatics[header.SemnaticNum++] = SEMANTIC_UV0;
		}

		if (mesh->HasTextureCoords(1)) {
			header.Semanatics[header.SemnaticNum++] = SEMANTIC_UV1;
		}

		Log<LDEBUG>("Export mesh", mesh->HasPositions(), mesh->HasNormals(), mesh->HasTextureCoords(0), mesh->HasTextureCoords(1),
			"Vertex count", mesh->mNumVertices, "Face count", mesh->mNumFaces);

		mVS.push_back(vs);
		mIS.push_back(is);
		mInfo.push_back(header);
		mTotalVertex += mesh->mNumVertices;
		mTotalFace += mesh->mNumFaces;
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