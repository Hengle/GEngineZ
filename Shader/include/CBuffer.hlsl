// Global buffer
cbuffer GlobalPerFrame : register(b1)
{
	float4x4 ViewProj;
	float4   CameraPos;
	float4   SunColor;
	float4   SunDirection;
	float4   AmbientColor;
}


cbuffer GlobalPerObject : register(b2)
{
	float4x4 World;
}

// just easy, regardless of efficiency
cbuffer GlobalRenderOption : register(b3) 
{
	int EnableHDR;
}
 