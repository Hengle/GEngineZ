#include "include/Common.hlsl"
#include "include/Cbuffer.hlsl"

struct a2v {
	float3 Position : POSITION;
	float2 UV : TEXCOORD;
};

struct v2f {
	float4 HPosition 	: SV_Position;
	float2 UV			: TEXCOORD;
};

texture2D tBaseMap : register(t0);
SamplerState sBaseMap : register(s0);

float3 ACES_Knarkowicz(float3 x) {
	half a = 2.51;
	half b = 0.03;
	half c = 2.43;
	half d = 0.59;
	half s = 0.14;
	return (x * (a * x + b)) / (x * (c * x + d) + s);
}


v2f VS(a2v IN) {
	v2f OUT;
	float3 vPos = IN.Position.xyz;
	OUT.HPosition = float4(vPos.xy * 2 - 1, vPos.z, 1.0);
	OUT.UV = IN.UV;

	return OUT;
}

float4 PS(v2f IN) : SV_Target{
	float4 colorAlpha = tBaseMap.Sample(sBaseMap, IN.UV);
	float3 color = colorAlpha.rgb;
	color = ACES_Knarkowicz(color);
	color = EncodeGamma(color);
	return float4(color, colorAlpha.a);
}