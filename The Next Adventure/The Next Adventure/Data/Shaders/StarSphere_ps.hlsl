Texture2D tex;
SamplerState sampleType;

#pragma pack_matrix( row_major )

#define PI 3.141592653589793

struct PixelInputType
{
	float4 position : SV_POSITION;
	float3 uv : UV0;
};

float4 main(PixelInputType input) : SV_TARGET
{
	float4 finalColor;
	float2 uv;

	uv = float2((0.5f + (atan2(input.uv.z, input.uv.x) / (2 * PI))), (0.5f - (asin(input.uv.y) / PI)));

	finalColor = tex.SampleLevel(sampleType, uv, 0).rgba;

	return finalColor;
}