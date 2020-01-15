#pragma once

#include <d3d11.h>
#include <DirectXMath.h>
#include <DirectXCollision.h>
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
		
		Object3D(bool targetable = false, bool alphaBlending = false);
		~Object3D();

		void Update();

		void Draw(D3D &d3d);
		void CreateTargetBox();
		void CreateSphere(int levels, int cellLevels, bool isPlanet = false, bool frontCulling = false, float radius = 1.0f);

		std::vector<Mesh*> mMeshes;
		std::vector<SphereMesh*> mSphereMeshes;
		DirectX::XMFLOAT3 mPosition, mRotation, mScale;
		Object3DConstantBufferData mConstantBufferData;
		ID3D11Buffer *mConstantBuffer = nullptr;
		DirectX::BoundingOrientedBox mBoundingOrientedBox;

		// Used to decide which raster state to be used when rendering the object
		bool mFrontCulling = false, mTargetable = false, mTargeted = false, mAlphaBlending = false;
	};
};