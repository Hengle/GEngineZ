// Global buffer
cbuffer GlobalPerFrame : register(b1)
{
	float4x4 ViewProj;
	float4   CameraPos;
	float4   SunColor;
	float4   SunDirection;
	float4   Ambient;
}


cbuffer GlobalPerObject : register(b2)
{
	float4x4 World;
}



// 