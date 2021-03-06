#include "include/CBuffer.hlsl"
#include "include/Common.hlsl"

struct a2v {
	float3 pos : POSITION;
	float2 uv : TEXCOORD;
};

Texture2D tSkyTexture : register(t0);
SamplerState sSkyTexture : register(s0);


struct v2f {
	float4 pos : SV_POSITION;
	float2 uv : TEXCOORD;
};

v2f VS(a2v IN) {
	v2f OUT;
	float4 pos = mul(float4(IN.pos, 1.0f), World);
	OUT.pos = mul(pos, ViewProj);
	OUT.uv = IN.uv;
	return OUT;

}

float4 PS(v2f IN) : SV_Target{
	
	float4 skyColorAlpha = tSkyTexture.Sample(sSkyTexture, IN.uv);
	float3 skyColor = DecodeGamma(skyColorAlpha.rgb);
	return float4(skyColor, skyColorAlpha.a);
}