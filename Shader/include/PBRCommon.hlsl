struct PbrPixelParamters {
	float3 Color;
	float3 IndirectColor;
	float3 Normal;
	float3 Emission;
	float Metallic;
	float Roughness;
	float AO;
	float Alpha;
	float Shadow;
};


float4 GetPBRResult(in PbrPixelParamters PARAM, in v2f IN) {
	float3 N = PARAM.Normal;
	float3 V = normalize(-IN.ViewDirection);
	float3 L = normalize(SunDirection.xyz);
	float NoL = dot(N, L);
	float NoV = dot(N, V);
	float VoL = dot(V, L);
	float InvLenH = rsqrt(2 + 2 * VoL);
	float NoH = saturate((NoL + NoV) * InvLenH);
	float VoH = saturate(InvLenH + InvLenH * VoL);
	NoL = saturate(NoL);
	NoV = saturate(abs(NoV) + 1e-5);

	float3 baseDiffColor = PARAM.Color - PARAM.Color * PARAM.Metallic;
	float3 baseSpecColor = lerp(half3(0.04, 0.04, 0.04), PARAM.Color, PARAM.Metallic.xxx);



	float3 color = float3(0, 0, 0);
	float3 diffColor = float3(0, 0, 0);
	float3 specColor = float3(0, 0, 0);

	// indirect diffuse (lightmap)

	// indirect spec (ibl)

	// direct color


	// === direct light color ===
	float3 directColor = SunColor.rgb * NoL + AmbientColor.rgb * PARAM.AO;
	// diffuse 
	diffColor += directColor * Diffuse_Lambert(baseDiffColor);
	// spcular GGX
	float a2 = Pow4(PARAM.Roughness);
	float D = D_GGX(a2, NoH);
	float Vis = Vis_Schlick(a2, NoV, NoL);
	float3 F = F_Fresnel(baseSpecColor, VoH);
	float3 GGXColor = D * Vis * F;
	specColor += directColor * GGXColor;

	color += diffColor;
	color += specColor;
	//color = EncodeGamma(color);

	return float4(color, PARAM.Alpha);


}


