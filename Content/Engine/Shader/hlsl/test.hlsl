#include "include/Common.hlsl"

cbuffer cbPerObject : register(b0) {
	float4x4 gWorld;
};



Texture2D  gDiffuseMap : register(t0);
SamplerState gSampler  : register(s0);



struct VertexIn {
	float3 pos : POSITION;
	float3 normal: NORMAL;
	float3 tangent: TANGENT;
	float4 uv : TEXCOORD;
};


struct VertexOut {
	float4 pos : SV_POSITION;
	float4 uv : TEXCOORD;
};

VertexOut VS(VertexIn vin) {
	VertexOut vout;
	float4 pos = mul(float4(vin.pos, 1.0f), gWorld);
	vout.pos = mul(pos, gViewProj);
	vout.uv = vin.uv;
	return vout;
}

float4 PS(VertexOut vin) : SV_Target{
	float4 color = gDiffuseMap.Sample(gSampler, vin.uv.xy);
	return color;
}

