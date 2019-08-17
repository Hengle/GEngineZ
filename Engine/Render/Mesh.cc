#include "Mesh.h"
#include <RHI/RHIDevice.h>

namespace z {

Mesh::~Mesh() {
}

void Mesh::CreateBuffer() {
	mVBuffer = GDevice->CreateVertexBuffer(Vertexes.size() / (Stride / 4), Semantics, Vertexes.data());
	mIBuffer = GDevice->CreateIndexBuffer(Indices.size(), 4, Indices.data());
}


}