#include "include/Common.hlsl"

struct a2v {
	float3 pos : POSITION;
	float3 normal : NORMAL;
	float2 uv : TEXCOORD;
};

Texture2D tBaseMap : register(t0);
SamplerState sBaseMap : register(s0);


struct v2f {
	float4 pos : SV_POSITION;
	float3 normal: TEXCOORD0;
	float2 uv : TEXCOORD1;
};

v2f VS(a2v IN) {
	v2f OUT;
	float4 pos = mul(float4(IN.pos, 1.0f), World);
	OUT.pos = mul(pos, ViewProj);
	OUT.normal = IN.normal;
	OUT.uv = IN.uv;
	return OUT;

}

float4 PS(v2f IN) : SV_Target{
	return tBaseMap.Sample(sBaseMap, IN.uv);
}