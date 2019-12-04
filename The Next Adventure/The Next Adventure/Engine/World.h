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

		World();
		~World();

		void Update();

		void Draw(D3D& d3d);
		void CreateWorld(int levels, int cellLevels, bool frontCulling = false, float radius = 1.0f, float minAltitude = 1.0f, float maxAltitude = 1.0f);

		float mMinAltitude, mMaxAltitude, mRadius;

		std::vector<SphereMesh*> mSphereMeshes;
		PlanetBufferData mPlanetBufferData;
		ID3D11Buffer* mConstantBuffer = nullptr;

		// Used to decide which raster state to be used when rendering the object
		bool mFrontCulling = false;
	};
};