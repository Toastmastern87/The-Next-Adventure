#pragma once

#include "Resources.h"
#include "System.h"
#include "Functions.h"
#include <d3d11.h>
#include <DirectXMath.h>
#include <string>
#include <vector>

namespace Toast
{
	class Mesh;
	class SphereMesh;
	class D3D;

	class World
	{
	public:
		struct PlanetBufferData
		{
			DirectX::XMFLOAT4 worldRadius;
			DirectX::XMFLOAT4 minAltitude;
			DirectX::XMFLOAT4 maxAltitude;
		};

		struct SunlightBufferData
		{
			DirectX::XMFLOAT4 sunlightDirection;
			DirectX::XMFLOAT4 diffuseColor;
		};

		World();
		~World();

		void Update();

		void Draw(D3D& d3d);
		void CreateWorld(int levels, int cellLevels, bool frontCulling = false, float radius = 1.0f, float minAltitude = 1.0f, float maxAltitude = 1.0f);
		void SetSunlightDirection(DirectX::XMFLOAT4 direction);
		void SetSunlightColor(DirectX::XMFLOAT4 color);

		float mMinAltitude, mMaxAltitude, mRadius;

		std::vector<SphereMesh*> mSphereMeshes;
		PlanetBufferData mPlanetBufferData;
		SunlightBufferData mSunlightBufferData;
		ID3D11Buffer* mPlanetConstantBuffer = nullptr;
		ID3D11Buffer* mSunlightConstantBuffer = nullptr;

		// Used to decide which raster state to be used when rendering the object
		bool mFrontCulling = false;

	private:
		DirectX::XMFLOAT4 mSunlightDirection, mSunlightColor;
	};
};