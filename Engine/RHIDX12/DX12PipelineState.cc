#include "DX12PipelineState.h"
#include "DX12Shader.h"
#include "DX12Device.h"

namespace z {


std::unordered_map<DX12PipelineStateCache::DX12PipelineStateHash, DX12PipelineState*, 
	DX12PipelineStateCache::DX12PipelineStateHashFN> DX12PipelineStateCache::gPipelineStates;

DX12PipelineState* DX12PipelineStateCache::Get(DX12Shader* shader, const std::vector<DXGI_FORMAT>& rtsFormat,
	const DXGI_FORMAT dsFormat, const uint64_t state) {
	DX12PipelineStateHash hash{shader, rtsFormat, dsFormat, state};
	auto iter = gPipelineStates.find(hash);
	if (iter != gPipelineStates.end()) {
		return iter->second;
	}
	Log<LDEBUG>("create new pipeline state....");
	DX12PipelineState* ppState = new DX12PipelineState(shader, rtsFormat, dsFormat, state);
	gPipelineStates[hash] = ppState;
	return ppState;
}

DX12PipelineState::DX12PipelineState(DX12Shader* shader, const std::vector<DXGI_FORMAT>& rtsFormat,
	const DXGI_FORMAT dsFormat, const uint64_t state) :
	mRasterizerState(CD3DX12_RASTERIZER_DESC(D3D12_DEFAULT)),
	mBlendState(CD3DX12_BLEND_DESC(D3D12_DEFAULT)),
	mDepthStencilState(CD3DX12_DEPTH_STENCIL_DESC(D3D12_DEFAULT)),
	mRenderTargetsFormat(rtsFormat),
	mDepthStencilFormat(dsFormat),
	mShader(shader),
	mState(nullptr) {

	// RasterizerState
	int fillMode = state & 0x03;
	switch (fillMode) {
	case RS_FILL_WIREFRAME:
		mRasterizerState.FillMode = D3D12_FILL_MODE_WIREFRAME;
		break;
	case RS_FILL_SOLID:
		mRasterizerState.FillMode = D3D12_FILL_MODE_SOLID;
		break;
	}
	int cullMode = state & (0x03 << 2);
	switch (cullMode) {
	case RS_CULL_NONE:
		mRasterizerState.CullMode = D3D12_CULL_MODE_NONE;
		break;
	case RS_CULL_FRONT:
		mRasterizerState.CullMode = D3D12_CULL_MODE_FRONT;
		break;
	case RS_CULL_BACK:
		mRasterizerState.CullMode = D3D12_CULL_MODE_BACK;
		break;
	}

	// BlendState

	// DepthStencilState
	if (mDepthStencilFormat == DXGI_FORMAT_UNKNOWN)
		mDepthStencilState.DepthEnable = false;


	D3D12_GRAPHICS_PIPELINE_STATE_DESC desc;
	ZeroMemory(&desc, sizeof(D3D12_GRAPHICS_PIPELINE_STATE_DESC));

	desc.pRootSignature        = mShader->GetIRootSignature();
	desc.VS                    = mShader->GetStage<SHADER_STAGE_VERTEX>()->GetCode();
	desc.PS                    = mShader->GetStage<SHADER_STAGE_PIXEL>()->GetCode();
	desc.BlendState            = mBlendState;
	desc.SampleMask            = UINT_MAX;
	desc.RasterizerState       = mRasterizerState;
	desc.DepthStencilState     = mDepthStencilState;
	desc.InputLayout           = mShader->GetInputLayoutDesc();
	desc.PrimitiveTopologyType = D3D12_PRIMITIVE_TOPOLOGY_TYPE_TRIANGLE;
	desc.NumRenderTargets      = mRenderTargetsFormat.size();
	memcpy(&desc.RTVFormats[0], mRenderTargetsFormat.data(), mRenderTargetsFormat.size() * sizeof(DXGI_FORMAT));
	desc.DSVFormat             = mDepthStencilFormat;
	desc.SampleDesc.Count      = 1;
	desc.SampleDesc.Quality    = 0;
	
	DX12_CHECK(GDX12Device->GetIDevice()->CreateGraphicsPipelineState(&desc, IID_PPV_ARGS(mState.GetComRef())));
}


ID3D12PipelineState* DX12PipelineState::GetIPipelineState() {
	return mState;
}

ID3D12RootSignature* DX12PipelineState::GetIRootSignature() {
	return mShader->GetIRootSignature();
}

}