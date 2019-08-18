#include "Mesh.h"
#include <RHI/RHIUtil.h>
#include <RHI/RHIDevice.h>

namespace z {

RenderMesh::RenderMesh(bool dynamic) :
	mIsDynamic(dynamic),
	mIsCompleted(false),
	mVertexStride(0),
	mIndexStride(0) {
}

void RenderMesh::SetVertexSemantics(const std::vector<ERHIInputSemantic>& semantic) {
	mVertexStride = 0;
	for (size_t i = 0; i < semantic.size(); i++) {
		mSemanticsOffset[semantic[i]] = mVertexStride;
		mVertexStride += GetSemanticSize(semantic[i]);
	}
	mSemantics = semantic;
}

void RenderMesh::SetIndexStride(uint8_t stride) {
	CHECK(stride == 2 || stride == 4);
	mIndexStride = stride;
}

void RenderMesh::CopyVertex(uint32_t begin, uint32_t size, const void* data, int8_t idx) {
	CHECK(begin % mVertexStride == 0 && size % mVertexStride == 0);

	if (begin + size > mVertexData.size()) {
		mVertexData.resize(begin + size);
	}
	memcpy(mVertexData.data() + begin, data, size);
	if (idx >= mVertexOffset.size()) {
		mVertexOffset.resize(idx + 1, 0x3FFFFFFF);
		mVertexCount.resize(idx + 1, 0x3FFFFFFF);
	}
	mVertexOffset[idx] = begin / mVertexStride;
	mVertexCount[idx] = size / mVertexStride;
}

void RenderMesh::CopyIndex(uint32_t begin, uint32_t size, const void* data, int8_t idx) {
	CHECK(begin % mIndexStride == 0 && size % mIndexStride == 0);

	if (begin + size > mIndexData.size()) {
		mIndexData.resize(begin + size);
	}
	memcpy(mIndexData.data() + begin, data, size);
	if (idx >= mIndexOffset.size()) {
		mIndexOffset.resize(idx + 1, 0x3FFFFFFF);
		mIndexCount.resize(idx + 1, 0x3FFFFFFF);
	}
	mIndexOffset[idx] = begin / mIndexStride;
	mIndexCount[idx] = size / mIndexStride;
}

bool RenderMesh::Complete(int maxVGroup, int maxIGroup, int expandSize) {
	CHECK(mSemantics.size() > 0 && mIndexStride > 0);
	CHECK(mIndexData.size() % mIndexStride == 0);
	CHECK(mVertexData.size() % mVertexStride == 0);

	if (!mIsCompleted) {
		mVBuffer = GDevice->CreateVertexBuffer(mVertexData.size() / mVertexStride, mSemantics, mVertexData.data(), mIsDynamic);
		mIBuffer = GDevice->CreateIndexBuffer(mIndexData.size() / mIndexStride, mIndexStride, mIndexData.data(), mIsDynamic);
		CHECK(mVBuffer && mIBuffer);

	} else if (mIsDynamic) {
		// upload vertex buffer
		if (mVertexData.size() > mVBuffer->GetBufferSize()) {
			Log<LDEBUG>("expand dynamic vertex buffer...");
			mVBuffer = GDevice->CreateVertexBuffer(mVertexData.size() / mVertexStride, mSemantics, mVertexData.data(), mIsDynamic);
		} else {
			void* addr = mVBuffer->MapBuffer();
			memcpy(addr, mVertexData.data(), mVertexData.size());
			mVBuffer->UnMapBuffer();
		}

		// update index buff
		if (mIndexData.size() > mIBuffer->GetBufferSize()) {
			Log<LDEBUG>("expand dynamic index buffer...");
			mIBuffer = GDevice->CreateIndexBuffer(mIndexData.size() / mIndexStride, mIndexStride, mIndexData.data(), mIsDynamic);
		} else {
			void* addr = mIBuffer->MapBuffer();
			memcpy(addr, mIndexData.data(), mIndexData.size());
			mIBuffer->UnMapBuffer();
		}
		CHECK(mVBuffer && mIBuffer);
	} else {
		CHECK(0, "Complete static mesh twice.");
	}
	mVertexGroup = maxVGroup;
	mIndexGroup = maxIGroup;
	// check offst
	mVertexOffset.resize(maxVGroup);
	mVertexCount.resize(maxVGroup);
	for (size_t i = 0; i < mVertexOffset.size(); i++) {
		CHECK(mVertexOffset[i] != 0x3FFFFFFF);
		CHECK(i + 1 >= mVertexOffset.size() || mVertexOffset[i] + mVertexCount[i] == mVertexOffset[i + 1]);
	}

	mIndexOffset.resize(maxIGroup);
	mIndexCount.resize(maxIGroup);
	for (size_t i = 0; i < mIndexOffset.size(); i++) {
		CHECK(mIndexOffset[i] != 0x3FFFFFFF);
		CHECK(i + 1 >= mIndexOffset.size() || mIndexOffset[i] + mIndexCount[i] == mIndexOffset[i + 1]);
	}

	mIsCompleted = true;
	return true;
}


}