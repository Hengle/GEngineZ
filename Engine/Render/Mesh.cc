#include "Mesh.h"
#include <RHI/RHIDevice.h>

namespace z {

void Mesh::UseBuffer() {
	GDevice->SetVertexBuffer(mVBuffer);
	GDevice->SetIndexBuffer(mIBuffer);
}

void Mesh::CreateBuffer() {
	mVBuffer = GDevice->CreateVertexBuffer(Vertexes.size() / (Stride / 4), Stride, Vertexes.data(), Semantics);
	if (2 == 2) {
		Indices16.resize(IndexCount);
		for (size_t i = 0; i < IndexCount; i++) {
			Indices16[i] = static_cast<uint16_t>(Indices[i]);
		}
		mIBuffer = GDevice->CreateIndexBuffer(Indices16.size(), 2, Indices16.data());
	} else {
		CHECK(0);
	}
}


}