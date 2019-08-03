#pragma once

#include "DX12Header.h"
#include <Core/CoreHeader.h>

namespace z {

class DX12Shader;
class DX12PipelineState;

class DX12PipelineStateCache {
public:
	static DX12PipelineState* Get(DX12Shader* shader, const std::vector<DXGI_FORMAT>&, const DXGI_FORMAT, const uint64_t);

	struct DX12PipelineStateHash {
		uint64_t State;
		DX12Shader* Shader;;
		DXGI_FORMAT RTs[MAX_SIGNATURE_NUM];
		DXGI_FORMAT DS;

		DX12PipelineStateHash(DX12Shader* shader, const std::vector<DXGI_FORMAT>& rtsFormat,
			const DXGI_FORMAT dsFormat, const uint64_t state) {
			Shader = shader;
			memset(RTs, 0, MAX_SIGNATURE_NUM * sizeof(DXGI_FORMAT));
			memcpy(RTs, rtsFormat.data(), rtsFormat.size() * sizeof(DXGI_FORMAT));
			DS = DXGI_FORMAT_UNKNOWN;
			State = state;
		}

		bool operator == (const DX12PipelineStateHash& state) const {
			return 0 == memcmp(this, &state, sizeof(DX12PipelineStateHash));
		}
	};

	struct DX12PipelineStateHashFN {
		std::size_t operator() (const DX12PipelineStateHash& node) const {
			size_t hv = (uint64_t)node.DS ^ node.State;
			for (int i = 0; i < MAX_SIGNATURE_NUM; i++) {
				hv ^= (uint64_t)node.RTs[i];
			}
			hv += (uint64_t)node.Shader;
			return hv;
		}
	};

private:
	static std::unordered_map<DX12PipelineStateHash, DX12PipelineState*, DX12PipelineStateHashFN> gPipelineStates;
};


class DX12PipelineState {
public:
	friend class DX12PipelineStateCache;

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
	DX12PipelineState(DX12Shader* shader, const std::vector<DXGI_FORMAT>&, const DXGI_FORMAT, const uint64_t);

	RefCountPtr<DX12Shader> mShader;
	RefCountPtr<ID3D12PipelineState> mState;

	std::vector<DXGI_FORMAT> mRenderTargetsFormat;
	DXGI_FORMAT mDepthStencilFormat;
	D3D12_RASTERIZER_DESC mRasterizerState;
	D3D12_BLEND_DESC mBlendState;
	D3D12_DEPTH_STENCIL_DESC mDepthStencilState;
};



}
