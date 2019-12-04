Texture2D shaderTexture;
SamplerState sampleType;

#pragma pack_matrix( row_major )

struct PixelInputType
{
	float4 position : SV_POSITION;
	float2 uv : TEXCOORD0;
};

float4 main(PixelInputType input) : SV_TARGET
{
	float4 color;

	color = shaderTexture.SampleLevel(sampleType, input.uv, 0);

	return color;
}