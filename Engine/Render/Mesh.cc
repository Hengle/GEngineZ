#include "Mesh.h"
#include <RHI/RHIDevice.h>

namespace z {

Mesh::~Mesh() {
	mIBuffer.clear();
}


void Mesh::UseBuffer(int idx) {
	GDevice->SetVertexBuffer(mVBuffer);
	GDevice->SetIndexBuffer(mIBuffer[idx]);
}

void Mesh::CreateBuffer() {
	mVBuffer = GDevice->CreateVertexBuffer(Vertexes.size() / (Stride / 4), Stride, Vertexes.data(), Semantics);
	for (size_t i = 0; i < Indices.size(); i++) {
		RHIIndexBuffer* ibuffer = GDevice->CreateIndexBuffer(Indices[i].size(), 4, Indices[i].data());
		mIBuffer.push_back(ibuffer);
	}
}


}