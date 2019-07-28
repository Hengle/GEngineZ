#include "DX12PipelineState.h"

namespace z {

DX12PipelineState::DX12PipelineState() :
	mDepthStencilFormat(DXGI_FORMAT_UNKNOWN) {

}

void DX12PipelineState::Create() {
	if (mState.GetRef() != nullptr) {
		return;
	}
	D3D12_GRAPHICS_PIPELINE_STATE_DESC desc;
	ZeroMemory(&desc, sizeof(D3D12_GRAPHICS_PIPELINE_STATE_DESC));
	
	desc.pRootSignature        = mUniformLayout->GetRootSignature();
	desc.VS                    = mShaderVS->GetCode();
	desc.PS                    = mShaderPS->GetCode();
	desc.BlendState            = CD3DX12_BLEND_DESC(D3D12_DEFAULT);
	desc.SampleMask            = UINT_MAX;
	desc.RasterizerState       = CD3DX12_RASTERIZER_DESC(D3D12_DEFAULT);
	desc.DepthStencilState     = CD3DX12_DEPTH_STENCIL_DESC(D3D12_DEFAULT);
	desc.InputLayout           = mVertexLayout->GetDesc();
	desc.PrimitiveTopologyType = D3D12_PRIMITIVE_TOPOLOGY_TYPE_TRIANGLE;
	desc.NumRenderTargets      = mRenderTargetsFormat.size();
	memcpy(&desc.RTVFormats[0], mRenderTargetsFormat.data(), mRenderTargetsFormat.size() * sizeof(DXGI_FORMAT));
	desc.DSVFormat             = mDepthStencilFormat;
	desc.SampleDesc.Count      = 1;
	desc.SampleDesc.Quality    = 0;
	
	DX12_CHECK(GDX12Device->GetIDevice()->CreateGraphicsPipelineState(&desc, IID_PPV_ARGS(mState.GetComRef())));
}


ID3D12PipelineState* DX12PipelineState::GetIPipelineState() {
	Create();
	return mState.GetRef();
}

ID3D12RootSignature* DX12PipelineState::GetIRootSignature() {
	return mUniformLayout->GetRootSignature();
}

}