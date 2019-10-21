#pragma once
#include <Core/CoreHeader.h>
#include <Render/RenderConst.h>
#include <RHI/RHIResource.h>

namespace z {


class RenderMesh : public RefCounter {
public:
	RenderMesh(bool dynamic = false);

	void SetVertexSemantics(const std::vector<ERHIInputSemantic>&);
	void SetIndexStride(uint8_t stride);

	void CopyVertex(uint32_t begin, uint32_t size, const void* data, int8_t grpIdx);
	void CopyIndex(uint32_t begin, uint32_t size, const void* data, int8_t grpIdx);

	bool Complete(int maxVGroup, int maxIGroup, int expandSize=0);

	uint8_t GetVertexStride() {
		return mVertexStride;
	}

	uint8_t GetIndexStride() {
		return mIndexStride;
	}

	uint32_t GetVertexGroupNum() {
		return (uint32_t)mVertexOffset.size();
	}

	uint32_t GetIndexGroupNum() {
		return (uint32_t)mIndexOffset.size();
	}

	uint32_t GetVertexOffset(int8_t grpIdx) {
		CHECK(grpIdx < mVertexOffset.size());
		return (uint32_t)mVertexOffset[grpIdx];
	}

	uint32_t GetIndexOffset(int8_t grpIdx) {
		CHECK(grpIdx < mIndexOffset.size());
		return (uint32_t)mIndexOffset[grpIdx];
	}

	uint32_t GetVertexCount(int8_t grpIdx=-1) {
		if (grpIdx < 0) {
			return mVertexData.size() / mVertexStride;
		}
		CHECK(grpIdx < mVertexCount.size());
		return (uint32_t)mVertexCount[grpIdx];
	}

	uint32_t GetIndexCount(int8_t grpIdx=-1) {
		if (grpIdx < 0) {
			return mIndexData.size() / mIndexStride;
		}
		CHECK(grpIdx < mIndexCount.size());
		return (uint32_t)mIndexCount[grpIdx];
	}

	std::tuple<RHIVertexBuffer*, RHIIndexBuffer*> GetRHIResource() {
		return { mVBuffer , mIBuffer };
	}

    bool HasSemantic(ERHIInputSemantic sem);
    void GetVertex(ERHIInputSemantic sem, int count, math::Vector2F &v);
    void GetVertex(ERHIInputSemantic sem, int count, math::Vector3F &v);

private:
	bool mIsDynamic;
	bool mIsCompleted;
	
	uint8_t mSemanticsOffset[SEMANTIC_MAX];
	std::vector<ERHIInputSemantic> mSemantics;

	uint8_t mVertexStride;
	uint8_t mIndexStride;

	std::vector<uint8_t> mVertexData;
	std::vector<uint8_t> mIndexData;

	std::vector<uint32_t> mVertexOffset;
	std::vector<uint32_t> mIndexOffset;
	std::vector<uint32_t> mVertexCount;
	std::vector<uint32_t> mIndexCount;
	uint32_t mVertexGroup;
	uint32_t mIndexGroup;

	RefCountPtr<RHIVertexBuffer> mVBuffer;
	RefCountPtr<RHIIndexBuffer> mIBuffer;
};

}