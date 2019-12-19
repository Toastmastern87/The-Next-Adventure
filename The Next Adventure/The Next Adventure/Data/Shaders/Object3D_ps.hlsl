Texture2D tex;
SamplerState sampleType;

#pragma pack_matrix( row_major )

struct PixelInputType
{
	float4 position : SV_POSITION;
	float3 normal : NORMAL0;
	float3 tangent: TANGENT0;
	float3 bitangent : BITANGENT0;
	float2 uv : UV0;
};

float4 main(PixelInputType input) : SV_TARGET
{
	float4 color;

	color = tex.SampleLevel(sampleType, input.uv, 0);

	return color;
}