cbuffer GUI2DBuffer : register(b3)
{
	matrix projectionMatrix;
};

struct VertexInputType
{
	float3 position : POSITION;
	float2 uv : TEXCOORD0;
};

struct PixelInputType
{
	float4 position : SV_POSITION;
	float2 uv : TEXCOORD0;
};

PixelInputType main(VertexInputType input)
{
	PixelInputType output;

	output.position = mul(float4(input.position, 1.0f), projectionMatrix);
	output.uv = input.uv;

	return output;
}