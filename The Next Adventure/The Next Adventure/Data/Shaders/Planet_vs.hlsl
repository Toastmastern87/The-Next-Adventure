Texture2D heightMap : register(t0);
SamplerState sampleType;

#pragma pack_matrix( row_major )

#define PI 3.141592653589793

cbuffer CameraBuffer : register(b1)
{
	matrix viewMatrix;
	matrix projectionMatrix;
	float4 cameraPosition;
	float4 cameraDirection;
};

cbuffer MorphBuffer : register(b2)
{
	matrix rotationMatrix;
	float4 distanceLUT[32];
	float4 morphRange;
};

cbuffer PlanetBuffer : register(b4)
{
	float4 radius;
	float4 minAltitude;
	float4 maxAltitude;
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
	float2 uv : UV0;
	float3 normal : NORMAL0;
	float3 viewVector : NORMAL1;
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

float GetHeight(float2 uv, float maxHeight, float minHeight)
{
	float heightColorValue;
	float finalHeight;

	heightColorValue = heightMap.SampleLevel(sampleType, uv, 0).r;

	finalHeight = (heightColorValue * (29.429f)) + minHeight;

	return finalHeight;
}

PixelInputType main(VertexInputType input)
{
	PixelInputType output;
	float4 finalPos;
	float3 pos;
	float2 uv;
	float distance, morphPercentage;
	float terrainHeight;

	pos = input.a + input.r * input.localPosition.x + input.s * input.localPosition.y;

	distance = length(mul(pos, rotationMatrix) - cameraPosition.xyz);
	morphPercentage = MorphFac(distance, input.level);

	pos += morphPercentage * (input.r * input.localMorph.x + input.s * input.localMorph.y);

	finalPos = float4(normalize(pos), 1.0f);

	pos = normalize(pos);

	uv = float2((0.5f + (atan2(pos.z, pos.x) / (2 * PI))), (0.5f - (asin(pos.y) / PI)));

	output.uv = uv;

	terrainHeight = GetHeight(uv, maxAltitude.a, minAltitude.a);

	float4x4 worldMatrix =
	{
		(radius.a + terrainHeight), 0.0f, 0.0f, 0.0f,
		0.0f, (radius.a + terrainHeight), 0.0f, 0.0f,
		0.0f, 0.0f, (radius.a + terrainHeight), 0.0f,
		0.0f, 0.0f, 0.0f, 1.0f
	};

	//Normal calculations
	output.normal = normalize(mul(finalPos, worldMatrix));
	output.viewVector = normalize(mul(finalPos, worldMatrix) - cameraPosition.xyz);
	//This should work but will be a bug to fix for a later date
	//output.viewVector = normalize(cameraDirection.xyz); 

	output.position = mul(finalPos, worldMatrix);
	output.position = mul(output.position, viewMatrix);
	output.position = mul(output.position, projectionMatrix);

	return output;
}