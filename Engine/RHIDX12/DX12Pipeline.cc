#include "DX12Pipeline.h"
#include "DX12Command.h"

namespace z {



void DX12PipelineState::Compile() {
	D3D12_GRAPHICS_PIPELINE_STATE_DESC desc;
	ZeroMemory(&desc, sizeof(D3D12_GRAPHICS_PIPELINE_STATE_DESC));

	desc.pRootSignature = mUniformDesc->GetRootSignature();
	desc.VS = mShaderVS->GetCode();
	desc.PS = mShaderPS->GetCode();
	desc.BlendState = CD3DX12_BLEND_DESC(D3D12_DEFAULT);
	desc.SampleMask = UINT_MAX;
	desc.RasterizerState = CD3DX12_RASTERIZER_DESC(D3D12_DEFAULT);
	desc.DepthStencilState = CD3DX12_DEPTH_STENCIL_DESC(D3D12_DEFAULT);
	desc.InputLayout = mVertexDesc->GetDesc();
	desc.PrimitiveTopologyType = D3D12_PRIMITIVE_TOPOLOGY_TYPE_TRIANGLE;
	desc.NumRenderTargets = mRenderTargets.size();
	for (int i = 0; i < mRenderTargets.size(); i++) {
		desc.RTVFormats[i] = PixelFomat[mRenderTargets[i]->GetDesc().pixelFormat].format;
	}
	if (mDepthStencil) {
		desc.DSVFormat = PixelFomat[mDepthStencil->GetDesc().pixelFormat].format;
	}
	desc.SampleDesc.Count = 1;
	desc.SampleDesc.Quality = 0;

	DX12_CHECK(GDX12Device->GetIDevice()->CreateGraphicsPipelineState(&desc, IID_PPV_ARGS(mStateCache.GetComRef())));
}



void DX12Pipeline::Apply() {

	

}



}