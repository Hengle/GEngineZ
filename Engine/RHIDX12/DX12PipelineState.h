#pragma once

#include "DX12Header.h"
#include <Core/CoreHeader.h>

namespace z {

class DX12Shader;
class DX12PipelineState;
class DX12RenderTarget;
class DX12DepthStencil;

class DX12PipelineStateCache {
public:
	static DX12PipelineState* Get(DX12Shader* shader, const uint8_t semoff[SEMANTIC_MAX], 
		const std::vector<DX12RenderTarget*>&, const DX12DepthStencil*, const RHIRenderState);

	static void ClearCache();

#pragma pack(push, 1)
	struct DX12PipelineStateHash {
		uint8_t Semoffs[SEMANTIC_MAX];
		// render target
		int RTNum;
		DXGI_FORMAT RTsFormat[MAX_RT_NUM];
		D3D12_BLEND_DESC BlendDesc;
		// depth stencil
		DXGI_FORMAT DSFormat;
		// state
		RHIRenderState State;
		DX12Shader* Shader;

		DX12PipelineStateHash(DX12Shader* shader, const uint8_t sem[SEMANTIC_MAX], const std::vector<DX12RenderTarget*>& rts, 
			const DX12DepthStencil* ds, const RHIRenderState state);

		bool operator == (const DX12PipelineStateHash& state) const {
			// becareful of auto padding data between members
			return  0 == memcmp(this, &state, sizeof(DX12PipelineStateHash));
		}
	};
#pragma pack(pop) 

	struct DX12PipelineStateHashFN {
		std::size_t operator() (const DX12PipelineStateHash& node) const {
			size_t hv = (uint64_t)node.DSFormat ^ node.State.Value;
			for (int i = 0; i < MAX_RT_NUM; i++) {
				hv ^= (uint64_t)node.RTsFormat[i];
			}
			for (int i = 0; i < SEMANTIC_MAX; i++) {
				hv ^= (uint64_t)node.Semoffs[i];
			}
			hv += (uint64_t)node.Shader;
			return hv;
		}
	};

private:
	static std::unordered_map<DX12PipelineStateHash, RefCountPtr<DX12PipelineState>, DX12PipelineStateHashFN> gPipelineStates;
};


class DX12PipelineState : public RefCounter {
public:
	friend class DX12PipelineStateCache;

	virtual ~DX12PipelineState();

	D3D12_RASTERIZER_DESC& GetRasterizerState() {
		return mRasterizerState;
	}

	D3D12_BLEND_DESC& GetBlendState() {
		return mBlendState;
	}

	D3D12_DEPTH_STENCIL_DESC& GetDepthStencilState() {
		return mDepthStencilState;
	}

	ID3D12PipelineState* GetIPipelineState();
	ID3D12RootSignature* GetIRootSignature();

private:
	DX12PipelineState(const DX12PipelineStateCache::DX12PipelineStateHash&);

	RefCountPtr<DX12Shader> mShader;
	RefCountPtr<ID3D12PipelineState> mState;

	D3D12_RASTERIZER_DESC mRasterizerState;
	D3D12_BLEND_DESC mBlendState;
	D3D12_DEPTH_STENCIL_DESC mDepthStencilState;
};



}
