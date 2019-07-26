
struct VertexIn {
	float3 pos : POSITION;
	float4 color : COLOR;
};

struct VertexOut {
	float4 pos : SV_POSITION;
	float4 color : COLOR;
};

VertexOut VS(VertexIn vin) {
	VertexOut vout;
	vout.pos = float4(1.0, 1.0, 1.0, 1.0);
	vout.color = float4(1.0, 1.0, 1.0, 1.0);
	return vout;
}

float4 PS(VertexOut vin) : SV_Target {
	return vin.color;
}

