#include "include/Common.hlsl"
#include "include/BRDF.hlsl"
#include "include/CBuffer.hlsl"

struct a2v {
	float3 Position : POSITION;
	float3 Normal : NORMAL;
	float3 Tangnent: TANGENT;
	float3 Binormal: BINORMAL;
	float2 UV : TEXCOORD;
};

Texture2D tBaseMap : register(t0);
SamplerState sBaseMap : register(s0);


Texture2D tNormalMap : register(t1);
SamplerState sNormalMap : register(s1);

Texture2D tMixMap : register(t2);
SamplerState sMixMap : register(s2);

struct v2f {
	float4 HPosition : SV_POSITION;

	float4 WorldPosition : TEXCOORD0;
	float4 WorldNormal   : TEXCOORD1;
	float3 WorldTangent  : TEXCOORD2;
	float3 WorldBinormal : TEXCOORD3;
	float3 ViewDirection : TEXCOORD5;
	float2 UV			 : TEXCOORD4;
};


#include "include/PBRCommon.hlsl"

v2f VS(a2v IN) {
	v2f OUT;
	float4 pos = mul(float4(IN.Position, 1.0f), World);
	OUT.HPosition = mul(pos, ViewProj);
	OUT.WorldPosition = pos;
	OUT.WorldNormal = mul(float4(IN.Normal, 0), World);
	OUT.WorldTangent = normalize(mul(IN.Tangnent, (float3x3)World));
	OUT.WorldBinormal = normalize(mul(IN.Binormal, (float3x3)World));
	OUT.ViewDirection = pos.xyz - CameraPos.xyz;
	OUT.UV = IN.UV;
	return OUT;

}


float4 PS(v2f IN) : SV_Target{
	PbrPixelParamters PPP;
	PPP.IndirectColor = float3(0, 0, 0);
	PPP.Emission = float3(0, 0, 0);
	PPP.Shadow = 0;

	float4 baseColorAlpha = tBaseMap.Sample(sBaseMap, IN.UV);
	PPP.Color = DecodeGamma(baseColorAlpha.rgb);
	PPP.Alpha = baseColorAlpha.a;

	float3 mixColor = tMixMap.Sample(sMixMap, IN.UV).xyz;
	PPP.Metallic = mixColor.r;
	PPP.Roughness = mixColor.g;
	PPP.AO = mixColor.b;

	float3 tangentNormal = tNormalMap.Sample(sNormalMap, IN.UV).xyz;
	tangentNormal = tangentNormal * 2 - 1;
	float3 N = tangentNormal.x * IN.WorldTangent + tangentNormal.y * IN.WorldBinormal + tangentNormal.z * IN.WorldNormal.xyz;
	PPP.Normal = normalize(N);

	return GetPBRResult(PPP, IN);
}