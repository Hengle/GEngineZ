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
		const aiScene* scn = importer.ReadFile(mesh_path, aiProcess_Triangulate |  aiProcess_FlipUVs);

		if (!scn || scn->mFlags & AI_SCENE_FLAGS_INCOMPLETE || !scn->mRootNode) {
			Log<LERROR>("Load mesh failed");
			return false;
		}

		header.IndexNum = 0;
		header.VertCount = 0;

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
		int base = 0;
		ss_bin.write((char*)& header, sizeof(z::SubMeshFileHeader));

		for (size_t i = 0; i < mIS.size(); i++) {
			for (size_t j = 0; j < mIS[i].size(); j++) {
				mIS[i][j] += base;
			}
			base += mVSnum[i];

			ss_bin.write((char*)mIS[i].data(), mIS[i].size() * sizeof(uint32_t));
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
	std::vector<uint32_t> mVSnum;
	std::vector<std::vector<uint32_t>> mIS;
	
	z::SubMeshFileHeader header;
	int mTotalVertex{ 0 };
	int mTotalFace{ 0 };

	bool SemanticAdded = false;

	void ProcessNode(const aiScene* scn, aiNode* root) {
		aiMatrix4x4 m = root->mTransformation;
		Log<LDEBUG>("transform", m.a1, m.a2, m.a3, m.a4, m.b1, m.b2, m.b3, m.b4, m.c1, m.c2, m.c3, m.c4, m.d1, m.d2, m.d3, m.d4);


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
				vs.push_back(mesh->mVertices[i].x * 0.1f);
				vs.push_back(mesh->mVertices[i].y * 0.1f);
				vs.push_back(mesh->mVertices[i].z * 0.1f);
			}

			// normal
			if (mesh->HasNormals()) {
				vs.push_back(mesh->mNormals[i].x);
				vs.push_back(mesh->mNormals[i].y);
				vs.push_back(mesh->mNormals[i].z);
			}

			// tangents
			if (mesh->HasTangentsAndBitangents()) {
				vs.push_back(mesh->mTangents[i].x);
				vs.push_back(mesh->mTangents[i].y);
				vs.push_back(mesh->mTangents[i].z);

				vs.push_back(mesh->mBitangents[i].x);
				vs.push_back(mesh->mBitangents[i].y);
				vs.push_back(mesh->mBitangents[i].z);
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

		header.IndexCount[header.IndexNum++] = is.size();
		header.VertCount += mesh->mNumVertices;
		mVSnum.push_back(mesh->mNumVertices);
		
		if (!SemanticAdded) {
			SemanticAdded = true;
			header.SemnaticNum = 0;
			if (mesh->HasPositions()) {
				header.Semanatics[header.SemnaticNum++] = SEMANTIC_POSITION;
			}

			if (mesh->HasNormals()) {
				header.Semanatics[header.SemnaticNum++] = SEMANTIC_NORMAL;
			}

			if (mesh->HasTangentsAndBitangents()) {

				header.Semanatics[header.SemnaticNum++] = SEMANTIC_TANGENT;
				header.Semanatics[header.SemnaticNum++] = SEMANTIC_BINORMAL;
			}

			if (mesh->HasTextureCoords(0)) {
				header.Semanatics[header.SemnaticNum++] = SEMANTIC_UV0;
			}

			if (mesh->HasTextureCoords(1)) {
				header.Semanatics[header.SemnaticNum++] = SEMANTIC_UV1;
			}
		}
		

		Log<LDEBUG>("Export mesh", mesh->HasPositions(), mesh->HasNormals(), mesh->HasTextureCoords(0), mesh->HasTextureCoords(1),
			"Vertex count", mesh->mNumVertices, "Face count", mesh->mNumFaces);

		mVS.push_back(vs);
		mIS.push_back(is);
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