#pragma once
#include <Render/Mesh.h>

namespace z {

class MeshGenerator {
public:
	struct Vertex {
		Vertex() {}
		Vertex(float px, float py, float pz, float nx, float ny, float nz, float tx, float ty, float tz, float u, float v) :
			Position(px, py, pz), Normal(nx, ny, nz), TangentU(tx, ty, tz), TexC(u, v) {}

		math::Vector3F Position, Normal, TangentU;
		math::Vector2F TexC;
	};

	struct MeshData {
		std::vector<Vertex> Vertices;
		std::vector<uint32_t> Indices32;
	};

	static Mesh* ConvertToMesh(const MeshData& data) {
		Mesh* mesh = new Mesh;
		mesh->VertCount = data.Vertices.size();
		mesh->Stride = 44;
		mesh->Semantics = { SEMANTIC_POSITION, SEMANTIC_NORMAL, SEMANTIC_TANGENT, SEMANTIC_UV0 };
		mesh->Indices.push_back(data.Indices32);
		mesh->Vertexes.resize(data.Vertices.size() * 11);
		memcpy(mesh->Vertexes.data(), data.Vertices.data(), data.Vertices.size() * 44);

		mesh->CreateBuffer();
		return mesh;
	}

	static Mesh* CreateBox(float width, float height, float depth, uint32_t numSubdivisions);

	static Mesh* CreateSphere(float radius, uint32_t sliceCount, uint32_t stackCount);

	//MeshData CreateGeosphere(float radius, uint32 numSubdivisions);
	static Mesh* CreateCylinder(float bottomRadius, float topRadius, float height, uint32_t sliceCount, uint32_t stackCount);

	static Mesh* CreateGrid(float width, float depth, uint32_t m, uint32_t n);

	///<summary>
	/// Creates a quad aligned with the screen.  This is useful for postprocessing and screen effects.
	///</summary>
	//MeshData CreateQuad(float x, float y, float w, float h, float depth);

private:
	static void Subdivide(MeshData& meshData);
	static Vertex MidPoint(const Vertex& v0, const Vertex& v1);
	static void BuildCylinderTopCap(float bottomRadius, float topRadius, float height, uint32_t sliceCount, uint32_t stackCount, MeshData& meshData);
	static void BuildCylinderBottomCap(float bottomRadius, float topRadius, float height, uint32_t sliceCount, uint32_t stackCount, MeshData& meshData);
};

}
