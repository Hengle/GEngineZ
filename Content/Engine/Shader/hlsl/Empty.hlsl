#include "include/CBuffer.hlsl"

struct a2v {
	float3 Position : POSITION;
	float3 Normal : NORMAL;
};

struct v2f {
	float4 HPosition : SV_POSITION;
	float4 WorldPosition : TEXCOORD0;
	float3 ViewDirection : TEXCOORD1;
	float4 WorldNormal : TEXCOORD2;
};

v2f VS(a2v IN) {
	v2f OUT;
	float4 pos = mul(float4(IN.Position, 1.0f), World);
	OUT.HPosition = mul(pos, ViewProj);
	OUT.WorldPosition = pos;
	OUT.WorldNormal = mul(float4(IN.Normal, 0), World);
	OUT.ViewDirection = pos.xyz - CameraPos.xyz;
	return OUT;

}

float4 PS(v2f IN) : SV_Target{
	float3 N = normalize(IN.WorldNormal.xyz);

	// calc V & R
	float3 L = normalize(SunDirection.xyz);
	float3 V = -normalize(IN.ViewDirection);
	float3 R = normalize(reflect(-L, N));

	// base color
	float3 baseColor = float3(0.5, 0.5, 0.5);

	// lambert diffuse
	float diff = max(dot(N, L), 0);;
	float3 diffuse = baseColor * SunColor.xyz * diff;

	return float4(diffuse, 1.0);
}