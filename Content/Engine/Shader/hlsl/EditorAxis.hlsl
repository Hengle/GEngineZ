#include "include/CBuffer.hlsl"

struct a2v {
	float3 Position : POSITION;
};

struct v2f {
	float4 HPosition : SV_POSITION;
};

cbuffer Shading : register(b0) {
	float3 Color;
};

v2f VS(a2v IN) {
	v2f OUT;
	float4 pos = mul(float4(IN.Position, 1.0f), World);
	OUT.HPosition = mul(pos, ViewProj);
	return OUT;

}

float4 PS(v2f IN) : SV_Target{
	return float4(Color, 1.0);
}