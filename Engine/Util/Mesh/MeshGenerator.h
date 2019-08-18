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

	static RenderMesh* ConvertToMesh(const MeshData& data) {
		RenderMesh* mesh = new RenderMesh;
		mesh->SetVertexSemantics({ SEMANTIC_POSITION, SEMANTIC_NORMAL, SEMANTIC_TANGENT, SEMANTIC_UV0 });
		mesh->SetIndexStride(4);
		mesh->CopyVertex(0, data.Vertices.size() * sizeof(Vertex), data.Vertices.data(), 0);
		mesh->CopyIndex(0, data.Indices32.size() * sizeof(uint32_t), data.Indices32.data(), 0);
		mesh->Complete(1, 1);
		return mesh;
	}

	static RenderMesh* CreateBox(float width, float height, float depth, uint32_t numSubdivisions);

	static RenderMesh* CreateSphere(float radius, uint32_t sliceCount, uint32_t stackCount);

	//MeshData CreateGeosphere(float radius, uint32 numSubdivisions);
	static RenderMesh* CreateCylinder(float bottomRadius, float topRadius, float height, uint32_t sliceCount, uint32_t stackCount);

	static RenderMesh* CreateGrid(float width, float depth, uint32_t m, uint32_t n);

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
