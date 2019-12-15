Texture2D colorMap : register(t0);
Texture2D heightMap : register(t1);
SamplerState sampleType;

#pragma pack_matrix( row_major )

#define PI 3.141592653589793

cbuffer PlanetBuffer : register(b0)
{
	float4 radius;
	float4 minAltitude;
	float4 maxAltitude;
};

cbuffer LightCalculations : register(b1)
{
	float4 lightDirection;
	float4 diffuseColor;
};

struct PixelInputType
{
	float4 position : SV_POSITION;
	float2 uv : UV0;
	float3 normal : NORMAL0;
	float3 viewVector : NORMAL1;
};

float3x3 GetTBNMatrix(float3 normalVector, float3 posVector, float2 uv)
{
	float3 dp1, dp2, dp2perp, dp1perp, T, B;
	float2 duv1, duv2;
	float invMax;

	dp1 = ddx(posVector);
	dp2 = ddy(posVector);
	duv1 = ddx(uv);
	duv2 = ddx(uv);

	dp2perp = cross(dp2, normalVector);
	dp1perp = cross(normalVector, dp1);

	T = (dp2perp * duv1.x + dp1perp * duv2.x);
	B = (dp2perp * duv1.y + dp1perp * duv2.y);

	invMax = rsqrt(max(dot(T, T), dot(B, B)));

	return float3x3(T * invMax, B * invMax, normalVector);
}

float GetHeight(float2 uv)
{
	return (heightMap.SampleLevel(sampleType, uv, 0).r * (maxAltitude.a + minAltitude.a) + minAltitude.a);
}

float3 CalculateNormal(float3 normalVector, float3 viewVector, float2 uv)
{
	float textureWidth, textureHeight, hL, hR, hD, hU;
	float3 texOffset, N;
	float3x3 TBN;

	heightMap.GetDimensions(textureWidth, textureHeight);

	texOffset = float3((1.0f / (textureWidth)), (1.0f / (textureHeight)), 0.0f);
	
	if (uv.x >= (1.0f - (1.0f / 8192.0f)))
	{
		//Add code here to handle the seam in Mars
	}
	else 
	{
		hL = GetHeight((uv - texOffset.xz));
		hR = GetHeight((uv + texOffset.xz));
		hD = GetHeight((uv + texOffset.zy));
		hU = GetHeight((uv - texOffset.zy));
	}

	N = normalize(float3((hL - hR), (hU - hD), 2.0f));

	TBN = GetTBNMatrix(normalVector, -viewVector, uv);

	return mul(N, TBN);
}

float4 main(PixelInputType input) : SV_TARGET
{
	float3 normal;
	float sunlightIntensity, sunlightcolor;
	float4 finalColor;
	float2 uv;

	normal = normalize(CalculateNormal(input.normal, -input.viewVector, input.uv));

	sunlightIntensity = saturate(dot(normal, normalize(-lightDirection.xyz)));

	sunlightcolor = saturate(diffuseColor * sunlightIntensity);

	finalColor = colorMap.SampleLevel(sampleType, input.uv, 0).rgba;

	return finalColor * sunlightcolor;
}