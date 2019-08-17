#pragma once

#include "DX12Const.h"
#include <RHI/RHIConst.h>

namespace z {

inline D3D12_CLEAR_VALUE FromRHIClearVale(const RHIClearValue& rv) {
	D3D12_CLEAR_VALUE v;
	v.Format = DXGI_FORMAT_UNKNOWN;
	memcpy(v.Color, rv.color, sizeof(float) * 4);
	return v;
}

#define DECLARE_PF_MAP(Define)\
	Define(PF_INVALID     , DXGI_FORMAT_UNKNOWN),           \
	Define(PF_R32         , DXGI_FORMAT_R32_FLOAT),         \
	Define(PF_R32G32      , DXGI_FORMAT_R32G32_FLOAT),      \
	Define(PF_R32G32B32   , DXGI_FORMAT_R32G32B32_FLOAT),   \
	Define(PF_R32G32B32A32, DXGI_FORMAT_R32G32B32A32_FLOAT),\
	Define(PF_R16G16B16A16, DXGI_FORMAT_R16G16B16A16_FLOAT),\
	Define(PF_R8G8B8A8    , DXGI_FORMAT_R8G8B8A8_UNORM),    \
	Define(PF_D24S8       , DXGI_FORMAT_D24_UNORM_S8_UINT)	\



inline DXGI_FORMAT FromRHIFormat(ERHIPixelFormat format) {
	static std::unordered_map<ERHIPixelFormat, DXGI_FORMAT> formatMap = {
#define RHI_TO_DXGI(rhi, dxgi) {rhi, dxgi}
		DECLARE_PF_MAP(RHI_TO_DXGI)
#undef RHI_TO_DXGI
	};
	return formatMap[format];
}

inline ERHIPixelFormat FromDXGIFormat(DXGI_FORMAT format) {
	static std::unordered_map<DXGI_FORMAT, ERHIPixelFormat> formatMap = {
#define DXGGI_TO_RHI(rhi, dxgi) {dxgi, rhi}
		DECLARE_PF_MAP(DXGGI_TO_RHI)
#undef RHI_TO_DXGI
	};
	return formatMap[format];
}

#undef DECLARE_PF_MAP

inline uint16_t GetPixelSize(DXGI_FORMAT format) {
	switch (format) {
	case DXGI_FORMAT_R32_FLOAT:
	case DXGI_FORMAT_D24_UNORM_S8_UINT:
	case DXGI_FORMAT_R8G8B8A8_UNORM:
		return 4;
	case DXGI_FORMAT_R32G32_FLOAT:
		return 8;
	case DXGI_FORMAT_R32G32B32_FLOAT:
		return 12;
	case DXGI_FORMAT_R16G16B16A16_UINT:
	case DXGI_FORMAT_R16G16B16A16_FLOAT:
		return 8;
	case DXGI_FORMAT_R32G32B32A32_FLOAT:
		return 16;
	}
	return 0;
}

inline D3D12_RENDER_TARGET_BLEND_DESC FromRHIBlendState(const RHIBlendState& rhiState) {
	auto GetDX12BlendOp = [](ERHIBlendOperation op) -> D3D12_BLEND_OP {
		switch (op) {
		case BLEND_OP_ADD         : return D3D12_BLEND_OP_ADD;
		case BLEND_OP_SUBTRACT    : return D3D12_BLEND_OP_SUBTRACT;
		case BLEND_OP_REV_SUBTRACT: return D3D12_BLEND_OP_REV_SUBTRACT;
		case BLEND_OP_MIN         : return D3D12_BLEND_OP_MIN;
		case BLEND_OP_MAX         : return D3D12_BLEND_OP_MAX;
		default                   : return (D3D12_BLEND_OP)0;
		}
		return (D3D12_BLEND_OP)0;
	};

	auto GetDX12BlendFactor = [](ERHIBlendFactor factor) -> D3D12_BLEND {
		switch (factor) {
		case BLEND_FACTOR_ZERO            : return D3D12_BLEND_ZERO;
		case BLEND_FACTOR_ONE             : return D3D12_BLEND_ONE;
		case BLEND_FACTOR_SRC_COLOR       : return D3D12_BLEND_SRC_COLOR;
		case BLEND_FACTOR_INV_SRC_COLOR   : return D3D12_BLEND_INV_SRC_COLOR;
		case BLEND_FACTOR_SRC_ALPHA       : return D3D12_BLEND_SRC_ALPHA;
		case BLEND_FACTOR_INV_SRC_ALPHA   : return D3D12_BLEND_INV_SRC_ALPHA;
		case BLEND_FACTOR_DEST_ALPHA      : return D3D12_BLEND_DEST_ALPHA;
		case BLEND_FACTOR_INV_DEST_ALPHA  : return D3D12_BLEND_INV_DEST_ALPHA;
		case BLEND_FACTOR_DEST_COLOR      : return D3D12_BLEND_DEST_COLOR;
		case BLEND_FACTOR_INV_DEST_COLOR  : return D3D12_BLEND_INV_DEST_COLOR;
		case BLEND_FACTOR_SRC_ALPHA_SAT   : return D3D12_BLEND_SRC_ALPHA_SAT;
		case BLEND_FACTOR_BLEND_FACTOR    : return D3D12_BLEND_BLEND_FACTOR;
		case BLEND_FACTOR_INV_BLEND_FACTOR: return D3D12_BLEND_INV_BLEND_FACTOR;
		case BLEND_FACTOR_SRC1_COLOR      : return D3D12_BLEND_SRC1_COLOR;
		case BLEND_FACTOR_INV_SRC1_COLOR  : return D3D12_BLEND_INV_SRC1_COLOR;
		case BLEND_FACTOR_SRC1_ALPHA      : return D3D12_BLEND_SRC1_ALPHA;
		case BLEND_FACTOR_INV_SRC1_ALPHA  : return D3D12_BLEND_INV_SRC1_ALPHA;
		default                           : return (D3D12_BLEND)0;
		}
		return (D3D12_BLEND)0;
	};

	D3D12_RENDER_TARGET_BLEND_DESC state;
	ZeroMemory(&state, sizeof(D3D12_RENDER_TARGET_BLEND_DESC));
	state.BlendEnable    = rhiState.Enable ? 1 : 0;
	state.SrcBlend       = GetDX12BlendFactor(rhiState.SrcBlend);
	state.DestBlend      = GetDX12BlendFactor(rhiState.DestBlend);
	state.BlendOp        = GetDX12BlendOp(rhiState.BlendOp);
	state.SrcBlendAlpha  = GetDX12BlendFactor(rhiState.SrcBlendAlpha);
	state.DestBlendAlpha = GetDX12BlendFactor(rhiState.DestBlendAlpha);
	state.BlendOpAlpha   = GetDX12BlendOp(rhiState.BlendOpAlpha);
	state.RenderTargetWriteMask = D3D12_COLOR_WRITE_ENABLE_ALL;
	return state;
}

inline D3D12_COMPARISON_FUNC FromRHICompFunc(ERHIRenderState func) {
	switch (func) {
	case RS_COMP_FUNC_NEVER  : return D3D12_COMPARISON_FUNC_NEVER;
	case RS_COMP_FUNC_EQUAL  : return D3D12_COMPARISON_FUNC_EQUAL;
	case RS_COMP_FUNC_LESS   : return D3D12_COMPARISON_FUNC_LESS;
	case RS_COMP_FUNC_LEQUAL : return D3D12_COMPARISON_FUNC_LESS_EQUAL;
	case RS_COMP_FUNC_GREATER: return D3D12_COMPARISON_FUNC_GREATER;
	case RS_COMP_FUNC_GEQUAL : return D3D12_COMPARISON_FUNC_GREATER_EQUAL;
	case RS_COMP_FUNC_NEQUAL : return D3D12_COMPARISON_FUNC_NOT_EQUAL;
	case RS_COMP_FUNC_ALWAYS : return D3D12_COMPARISON_FUNC_ALWAYS;
	default: return (D3D12_COMPARISON_FUNC)0;
	}
	return (D3D12_COMPARISON_FUNC)0;
}

// from rhi depth stencil
inline D3D12_DEPTH_STENCIL_DESC FromRHIDepthStencilDesc(const RHIRenderState& state) {
	D3D12_DEPTH_STENCIL_DESC desc;

	desc.DepthEnable = state.EnableDepthTest;
	desc.DepthWriteMask = state.EnableDepthWrite ? D3D12_DEPTH_WRITE_MASK_ALL : D3D12_DEPTH_WRITE_MASK_ZERO;
	desc.DepthFunc = FromRHICompFunc((ERHIRenderState)state.DepthCompFunc);
	// not used now
	desc.StencilEnable = FALSE;
	desc.StencilReadMask = D3D12_DEFAULT_STENCIL_READ_MASK;
	desc.StencilWriteMask = D3D12_DEFAULT_STENCIL_WRITE_MASK;
	const D3D12_DEPTH_STENCILOP_DESC defaultStencilOp =
	{ D3D12_STENCIL_OP_KEEP, D3D12_STENCIL_OP_KEEP, D3D12_STENCIL_OP_KEEP, D3D12_COMPARISON_FUNC_ALWAYS };
	desc.FrontFace = defaultStencilOp;
	desc.BackFace = defaultStencilOp;

	return desc;
}


//inline const D3D12_SAMPLER_DESC FromRHISamplerDesc(const RHISamplerDesc& desc) {
//	auto static DecideAddressMode = [](ERHISamplerAddressMode mode) -> D3D12_TEXTURE_ADDRESS_MODE {
//		if (mode == SAMPLER_ADDRESS_MODE_WRAP) 
//			return D3D12_TEXTURE_ADDRESS_MODE_WRAP;
//		if (mode == SAMPLER_ADDRESS_MODE_MIRROR) 
//			return D3D12_TEXTURE_ADDRESS_MODE_MIRROR;
//		if (mode == SAMPLER_ADDRESS_MODE_CLAMP) 
//			return D3D12_TEXTURE_ADDRESS_MODE_CLAMP;
//		if (mode == SAMPLER_ADDRESS_MODE_BORDER) 
//			return D3D12_TEXTURE_ADDRESS_MODE_BORDER;
//		if (mode == SAMPLER_MODE_MIRROR_ONCE) 
//			return D3D12_TEXTURE_ADDRESS_MODE_MIRROR_ONCE;
//		return D3D12_TEXTURE_ADDRESS_MODE_WRAP;
//	};
//		
//	auto static DecideFilter = [](ERHISamplerFitler minf, ERHISamplerFitler maxf, ERHISamplerFitler mipf) -> D3D12_FILTER {
//#define DECIDE(m0, m1, m2, mr)	if (minf == m0 && maxf == m1 && mipf == m2) return mr;
//		DECIDE(SAMPLER_FILTER_POINT  , SAMPLER_FILTER_POINT  , SAMPLER_FILTER_POINT  , D3D12_FILTER_MIN_MAG_MIP_POINT);
//		DECIDE(SAMPLER_FILTER_POINT  , SAMPLER_FILTER_POINT  , SAMPLER_FILTER_LINEAR , D3D12_FILTER_MIN_MAG_MIP_POINT);
//		DECIDE(SAMPLER_FILTER_POINT  , SAMPLER_FILTER_LINEAR , SAMPLER_FILTER_POINT  , D3D12_FILTER_MIN_POINT_MAG_LINEAR_MIP_POINT);
//		DECIDE(SAMPLER_FILTER_POINT  , SAMPLER_FILTER_LINEAR , SAMPLER_FILTER_LINEAR , D3D12_FILTER_MIN_POINT_MAG_MIP_LINEAR);
//		DECIDE(SAMPLER_FILTER_LINEAR , SAMPLER_FILTER_POINT  , SAMPLER_FILTER_POINT  , D3D12_FILTER_MIN_LINEAR_MAG_MIP_POINT);
//		DECIDE(SAMPLER_FILTER_LINEAR , SAMPLER_FILTER_POINT  , SAMPLER_FILTER_LINEAR , D3D12_FILTER_MIN_LINEAR_MAG_POINT_MIP_LINEAR);
//		DECIDE(SAMPLER_FILTER_LINEAR , SAMPLER_FILTER_LINEAR , SAMPLER_FILTER_POINT  , D3D12_FILTER_MIN_MAG_LINEAR_MIP_POINT);
//		DECIDE(SAMPLER_FILTER_LINEAR , SAMPLER_FILTER_LINEAR , SAMPLER_FILTER_LINEAR , D3D12_FILTER_MIN_MAG_MIP_LINEAR);
//		DECIDE(SAMPLER_FILTER_ANISOTROPIC, SAMPLER_FILTER_ANISOTROPIC, SAMPLER_FILTER_ANISOTROPIC, D3D12_FILTER_ANISOTROPIC);
//#undef DECIDE
//		Log<LWARN>("sampler filter not supported");
//		return D3D12_FILTER_MIN_MAG_MIP_LINEAR;
//	};
//
//
//	D3D12_SAMPLER_DESC samplerDesc{};
//	samplerDesc.Filter         = DecideFilter(desc.minFilter, desc.maxFilter, desc.mipFilter);
//	samplerDesc.AddressU       = DecideAddressMode(desc.addressU);
//	samplerDesc.AddressV       = DecideAddressMode(desc.addressV);
//	samplerDesc.AddressW       = DecideAddressMode(desc.addressW);
//	samplerDesc.MipLODBias     = 0;
//	samplerDesc.MaxAnisotropy  = 1;
//	samplerDesc.ComparisonFunc = D3D12_COMPARISON_FUNC_ALWAYS;
//	samplerDesc.BorderColor[4] = { 0.f };
//	samplerDesc.MinLOD         = 0;
//	samplerDesc.MaxLOD         = D3D12_FLOAT32_MAX;
//	return samplerDesc;
//}



}
