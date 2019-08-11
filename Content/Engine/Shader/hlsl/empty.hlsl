#include "include/Common.hlsl"

struct VertexIn {
	float3 pos : POSITION;
};


struct VertexOut {
	float4 pos : SV_POSITION;
};

VertexOut VS(VertexIn vin) {
	VertexOut vout;
	float4 pos = mul(float4(vin.pos, 1.0f), World);
	vout.pos = mul(pos, ViewProj);
	return vout;

}

float4 PS(VertexOut vin) : SV_Target{
	return float4(0.5, 0.5, 0.5, 1.0);
}