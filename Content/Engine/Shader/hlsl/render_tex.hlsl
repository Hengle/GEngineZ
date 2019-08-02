
Texture2D  gDiffuseMap : register(t0);
SamplerState gSampler  : register(s0);

Texture2D  gDiffuseMap2 : register(t1);
SamplerState gSampler2  : register(s1);

struct VertexIn {
	float3 pos : POSITION;
	float2 uv : TEXCOORD;
};


struct VertexOut {
	float4 pos : SV_POSITION;
	float2 uv : TEXCOORD;
};

VertexOut VS(VertexIn vin) {
	VertexOut vout;
	vout.pos = gDiffuseMap2.SampleLevel(gSampler2, vin.uv, 0);
	vout.uv = vin.uv;
	return vout;
}

float4 PS(VertexOut vin) : SV_Target {
	float4 color = gDiffuseMap.Sample(gSampler, vin.uv);
	return color;
}

