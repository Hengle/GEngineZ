// Global buffer
cbuffer GlobalPerFrame : register(b0)
{
	float4x4 ViewProj;
	float3   CameraPos;
}


cbuffer GlobalPerObject : register(b2)
{
	float4x4 World;
}