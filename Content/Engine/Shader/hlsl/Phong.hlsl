#include "include/Common.hlsl"

struct a2v {
	float3 Position : POSITION;
	float3 Normal : NORMAL;
	float3 Tangnent: TANGENT;
	float3 Binormal: BITANGENT;
	float2 UV : TEXCOORD;
};

Texture2D tBaseMap : register(t0);
SamplerState sBaseMap : register(s0);

Texture2D tNormalMap : register(t1);
SamplerState sNormalMap : register(s1);

struct v2f {
	float4 HPosition : SV_POSITION;

	float4 WorldPosition : TEXCOORD0;
	float4 WorldNormal   : TEXCOORD1;
	float3 WorldTangent  : TEXCOORD2;
	float3 WorldBinormal : TEXCOORD3;
	float3 ViewDirection : TEXCOORD5;
	float2 UV			 : TEXCOORD4;
};

v2f VS(a2v IN) {
	v2f OUT;
	float4 pos = mul(float4(IN.Position, 1.0f), World);
	OUT.HPosition = mul(pos, ViewProj);
	OUT.WorldPosition = pos;
	OUT.WorldNormal = mul(float4(IN.Normal, 0), World);
	OUT.WorldTangent = normalize(mul(IN.Tangnent, (float3x3)World));
	OUT.WorldBinormal = normalize(mul(IN.Binormal, (float3x3)World));
	OUT.ViewDirection = pos.xyz - CameraPos;
	OUT.UV = IN.UV;
	return OUT;

}

float4 PS(v2f IN) : SV_Target{

	// calc world normal N
	float3 tangentNormal = tNormalMap.Sample(sNormalMap, IN.UV).xyz;
	tangentNormal = tangentNormal * 2 - 1;
	float3 N = tangentNormal.x * IN.WorldTangent + tangentNormal.y * IN.WorldBinormal + tangentNormal.z * IN.WorldNormal.xyz;

	float3 lightDir = normalize(float3(10, 10, 10) - IN.WorldPosition.xyz);

	float3 diffuse = float3(0.5, 0.5, 0.5) * max(dot(N, lightDir), 0.0) + float3(0.1, 0.1, 0.1);

	return float4(N, 1.0);
}