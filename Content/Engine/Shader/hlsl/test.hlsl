#include "include/Common.hlsl"

cbuffer cbPerObject : register(b0)
{
	float4x4 gWorld :WORLD;
};



Texture2D  gDiffuseMap : DEFUSEMAP: register(t0);
SamplerState gSampler  : register(s0);



struct VertexIn {
	float3 pos : POSITION;
	float3 normal : NORMAL;
	float3 tangent : TANGENT;
	float4 uv : TEXCOORD;
	float4 uv2 : TEXCOORD1;
};


struct VertexOut {
	float4 pos : SV_POSITION;
	float2 uv : TEXCOORD;
};

VertexOut VS(VertexIn vin) {
	VertexOut vout;
	float4 pos = mul(float4(vin.pos, 1.0f), gWorld);
	vout.pos = mul(float4(vin.normal, 1.0), mul(pos, gViewProj));
	vout.uv = vin.uv;
	return vout;
}

float4 PS(VertexOut vin) : SV_Target{
	float4 color = gDiffuseMap.Sample(gSampler, vin.uv);
	return color;
}

