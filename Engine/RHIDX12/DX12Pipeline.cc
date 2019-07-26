#include "DX12Pipeline.h"
#include "DX12Command.h"

namespace z {

DX12PipelineState* DX12PipelineState::GetCache(DX12Shader * shaderVS, DX12Shader *shaderPS, DX12VertexLayout *layout) {
	//RefCountPtr<ID3D12PipelineState> cache;
	//D3D12_GRAPHICS_PIPELINE_STATE_DESC desc;
	//ZeroMemory(&desc, sizeof(D3D12_GRAPHICS_PIPELINE_STATE_DESC));
	//desc.pRootSignature;
	//desc.VS = shaderVS->GetCode();
	//desc.PS = shaderPS->GetCode();
	//desc.BlendState;
	//desc.SampleMask = UINT_MAX;
	////desc.stream
	//desc.RasterizerState = CD3DX12_RASTERIZER_DESC(D3D12_DEFAULT);
	//desc.DepthStencilState = CD3DX12_DEPTH_STENCIL_DESC(D3D12_DEFAULT);
	//desc.InputLayout;
	//desc.PrimitiveTopologyType = D3D12_PRIMITIVE_TOPOLOGY_TYPE_TRIANGLE;
	//desc.NumRenderTargets;
	//desc.RTVFormats[0];
	//desc.DSVFormat;
	//desc.SampleDesc.Count = 1;
	//desc.SampleDesc.Quality = 0;

	//DX12_CHECK(GDX12Device->GetIDevice()->CreateGraphicsPipelineState(&desc, IID_PPV_ARGS(cache.GetComRef())));
	//DX12PipelineState* state = new DX12PipelineState();
	//state->mCached = cache;
	return nullptr;
}

void DX12PipelineState::DoSetPipeline() {
	//GDX12Device->GetCommandContext()->List()->SetPipelineState(mCached.GetRef());
}


void DX12Pipeline::SetDepthStencil() {

}

void DX12Pipeline::Apply() {
	// get and set pipeline
	DX12PipelineState *state = DX12PipelineState::GetCache(
		mShaderPS, mShaderPS, mVertexLayout);
	//state->DoSetPipeline();

	// set render target and 
	

}



}