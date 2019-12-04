#pragma once

#include <d3d11.h>
#include <DirectXMath.h>
#include <string>
#include <vector>
#include "Resources.h"
#include "System.h"
#include "Functions.h"

namespace Toast 
{
	class Mesh;
	class SphereMesh;
	class D3D;

	class Object3D 
	{
	public:
		struct Object3DConstantBufferData
		{
			DirectX::XMMATRIX worldMatrix;
		};
		
		Object3D();
		~Object3D();

		void Update();

		void Draw(D3D &d3d);
		void CreateCube();
		void CreateSphere(int levels, int cellLevels, bool isPlanet = false, bool frontCulling = false, float radius = 1.0f);

		std::vector<Mesh*> mMeshes;
		std::vector<SphereMesh*> mSphereMeshes;
		DirectX::XMFLOAT3 mPosition, mRotation, mScale;
		Object3DConstantBufferData mConstantBufferData;
		ID3D11Buffer *mConstantBuffer = nullptr;

		// Used to decide which raster state to be used when rendering the object
		bool mFrontCulling = false;
	};
};