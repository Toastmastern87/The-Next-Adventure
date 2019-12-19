#pragma pack_matrix( row_major )

cbuffer Object3DBuffer : register(b0)
{
	matrix worldMatrix;
};

cbuffer CameraBuffer : register(b1)
{
	matrix viewMatrix;
	matrix projectionMatrix;
	float4 cameraPosition;
	float4 cameraDirection;
};

struct VertexInputType
{
	float3 position : POSITION;
	float3 normal : NORMAL0;
	float3 tangent: TANGENT0;
	float3 bitangent : BITANGENT0;
	float2 uv : UV0;
};

struct PixelInputType
{
	float4 position : SV_POSITION;
	float3 normal : NORMAL0;
	float3 tangent: TANGENT0;
	float3 bitangent : BITANGENT0;
	float2 uv : UV0;
};

PixelInputType main(VertexInputType input)
{
	PixelInputType output;

	output.position = mul(float4(input.position, 1.0f), worldMatrix);
	output.position = mul(output.position, viewMatrix);
	output.position = mul(output.position, projectionMatrix);

	output.uv = input.uv;

	output.normal = mul(input.normal, worldMatrix);

	output.tangent = mul(input.tangent, worldMatrix);

	output.bitangent = mul(input.bitangent, worldMatrix);

	return output;
}