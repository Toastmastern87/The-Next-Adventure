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

cbuffer MorphBuffer : register(b2)
{
	float4 distanceLUT[32];
	float4 morphRange;
};

struct VertexInputType
{
	float2 localPosition : TEXCOORD0;
	float2 localMorph : TEXCOORD1;
	int level : TEXTUREID;
	float3 a : POSITION0;
	float3 r : POSITION1;
	float3 s : POSITION2;
};

struct PixelInputType
{
	float4 position : SV_POSITION;
	float3 uv : UV0;
};

float MorphFac(float distance, int level)
{
	float low, high, delta, a;

	low = distanceLUT[level - 1].x;
	high = distanceLUT[level].x;

	delta = high - low;

	a = (distance - low) / delta;

	return (1 - clamp((a / morphRange.x), 0, 1));
}

PixelInputType main(VertexInputType input)
{
	PixelInputType output;
	float4 finalPos;
	float3 pos;

	pos = normalize(input.a + input.r * input.localPosition.x + input.s * input.localPosition.y);

	finalPos = float4(pos, 1.0f);

	output.uv = pos;

	output.position = mul(finalPos, worldMatrix);
	output.position = mul(output.position, viewMatrix);
	output.position = mul(output.position, projectionMatrix);

	return output;
}