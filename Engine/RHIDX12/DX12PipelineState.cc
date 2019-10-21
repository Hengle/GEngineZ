#include "DX12PipelineState.h"
#include "DX12Shader.h"
#include "DX12Device.h"
#include "DX12Texture.h"

namespace z {


// pipeline cache
std::unordered_map<DX12PipelineStateCache::DX12PipelineStateHash, RefCountPtr<DX12PipelineState>,
	DX12PipelineStateCache::DX12PipelineStateHashFN> DX12PipelineStateCache::gPipelineStates;

DX12PipelineState* DX12PipelineStateCache::Get(DX12Shader* shader, const uint8_t semoff[SEMANTIC_MAX],
	const std::vector<DX12RenderTarget*>& rts, const DX12DepthStencil* ds, const RHIRenderState state) {

	DX12PipelineStateHash hash{ shader, semoff, rts, ds, state };
	auto iter = gPipelineStates.find(hash);
	if (iter != gPipelineStates.end()) {
		return iter->second;
	}
	DX12PipelineState* ppState = new DX12PipelineState(hash);
	{
		//DX12PipelineStateHashFN hashFN;
		//Log<LDEBUG>& log = Log<LDEBUG>();
		//log << "create new pipeline state...\n";
		//log << "   shader: " << shader << "\n";
		//log << "   state: "<< state.Value << "\n";
		//log << "   semnatic: ";
		//for (int i = 0; i < SEMANTIC_MAX; i++) log << (int)semoff[i] << " ";
		//log << "\n";
		//log << "   DS: " << hash.DSFormat << "\n";
		//log << "   RT: " << hash.RTNum << " >> ";
		//for (int i = 0; i < hash.RTNum; i++) log << hash.RTsFormat[i] << " ";
		//log << "\n";
		//log << "hash === " << hashFN(hash) << "===";
	}
	//for (int i = 0; i < SEMANTIC_MAX; i++) std:
	gPipelineStates[hash] = ppState;
	return ppState;

}

void DX12PipelineStateCache::ClearCache() {
	gPipelineStates.clear();
}

DX12PipelineStateCache::DX12PipelineStateHash::DX12PipelineStateHash(
	DX12Shader* shader, const uint8_t semoff[SEMANTIC_MAX], const std::vector<DX12RenderTarget*> &rts, const DX12DepthStencil* ds, const RHIRenderState state) {
	Shader = shader;
	memset(RTsFormat, 0, MAX_RT_NUM * sizeof(DXGI_FORMAT));
	BlendDesc = CD3DX12_BLEND_DESC(D3D12_DEFAULT);
	RTNum = rts.size();
	for (int i = 0; i < rts.size(); i++) {
		BlendDesc.RenderTarget[i] = rts[i]->GetBlendState();
		RTsFormat[i] = rts[i]->Format;
	}
	memcpy(Semoffs, semoff, sizeof(uint8_t) * SEMANTIC_MAX);
	DSFormat = ds ? ds->Format : DXGI_FORMAT_UNKNOWN;
	State = state;
}

DX12PipelineState::DX12PipelineState(const DX12PipelineStateCache::DX12PipelineStateHash& hash) :
	mRasterizerState(CD3DX12_RASTERIZER_DESC(D3D12_DEFAULT)),
	mDepthStencilState(FromRHIDepthStencilDesc(hash.State)),
	mBlendState(hash.BlendDesc),
	mShader(hash.Shader),
	mState(nullptr) {

	RHIRenderState state = hash.State;

	// RasterizerState
	switch (state.FillMode) {
	case RS_FILL_WIREFRAME:
		mRasterizerState.FillMode = D3D12_FILL_MODE_WIREFRAME;
		break;
	case RS_FILL_SOLID:
		mRasterizerState.FillMode = D3D12_FILL_MODE_SOLID;
		break;
	}
	switch (state.CullMode) {
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


	// get layout with vertex
	D3D12_INPUT_LAYOUT_DESC layout = mShader->GetInputLayoutDesc();
	const std::vector< ERHIInputSemantic>& inputSemantics = mShader->GetInputSemantics();
	std::vector< D3D12_INPUT_ELEMENT_DESC> descs;
	for (size_t i = 0; i < layout.NumElements; i++) { 
		descs.push_back(layout.pInputElementDescs[i]);
		descs.back().AlignedByteOffset = hash.Semoffs[inputSemantics[i]];
	}
	layout = { descs.data(), (uint32_t)descs.size() };

	D3D12_GRAPHICS_PIPELINE_STATE_DESC desc;
	ZeroMemory(&desc, sizeof(D3D12_GRAPHICS_PIPELINE_STATE_DESC));

	desc.pRootSignature        = mShader->GetIRootSignature();
	desc.VS                    = mShader->GetStage<SHADER_STAGE_VERTEX>()->GetCode();
	desc.PS                    = mShader->GetStage<SHADER_STAGE_PIXEL>()->GetCode();
	desc.BlendState            = mBlendState;
	desc.SampleMask            = UINT_MAX;
	desc.RasterizerState       = mRasterizerState;
	desc.DepthStencilState     = mDepthStencilState;
	desc.InputLayout           = layout;
	desc.PrimitiveTopologyType = D3D12_PRIMITIVE_TOPOLOGY_TYPE_TRIANGLE;
	desc.NumRenderTargets      = hash.RTNum;
	memcpy(&desc.RTVFormats[0], hash.RTsFormat, hash.RTNum * sizeof(DXGI_FORMAT));
	desc.DSVFormat             = hash.DSFormat;
	desc.SampleDesc.Count      = 1;
	desc.SampleDesc.Quality    = 0;
	
	DX12_CHECK(GDX12Device->GetIDevice()->CreateGraphicsPipelineState(&desc, IID_PPV_ARGS(mState.GetComRef())));
}

DX12PipelineState::~DX12PipelineState() {
}

ID3D12PipelineState* DX12PipelineState::GetIPipelineState() {
	return mState;
}

ID3D12RootSignature* DX12PipelineState::GetIRootSignature() {
	return mShader->GetIRootSignature();
}

}