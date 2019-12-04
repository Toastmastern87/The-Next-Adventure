#pragma once

#include <DirectXMath.h>
#include <d3d11.h>
#include <vector>
#include "System.h"
#include "Functions.h"

namespace Toast 
{
	struct SphereTriangle
	{
		DirectX::XMFLOAT3 a, b, c;
		SphereTriangle* parent;
		short level;

		SphereTriangle()
		{
		}

		SphereTriangle(DirectX::XMFLOAT3 A, DirectX::XMFLOAT3 B, DirectX::XMFLOAT3 C, SphereTriangle* Parent, short Level)
		{
			a = A;
			b = B;
			c = C;

			parent = Parent;

			level = Level;
		}
	};

	struct SphereVertex
	{
		DirectX::XMFLOAT2 position;
		DirectX::XMFLOAT2 morphing;

		SphereVertex(DirectX::XMFLOAT2 pos, DirectX::XMFLOAT2 morph)
		{
			position = pos;
			morphing = morph;
		}
	};

	struct SphereCell
	{
		int level;
		DirectX::XMFLOAT3 a;
		DirectX::XMFLOAT3 r;
		DirectX::XMFLOAT3 s;

		SphereCell(int Level, DirectX::XMFLOAT3 A, DirectX::XMFLOAT3 R, DirectX::XMFLOAT3 S)
		{
			level = Level;
			a = A;
			r = R;
			s = S;
		}
	};

	struct MorphConstantBufferData
	{
		DirectX::XMMATRIX rotationMatrix;
		DirectX::XMFLOAT4 distanceLUT[32];
		DirectX::XMFLOAT4 morphRange;
	};

	enum NextSphereTriangle
	{
		CULL,
		LEAF,
		SPLIT,
		SPLITCULL
	};

	class Material;
	class Resources;
	class D3D;

	class SphereMesh
	{
	public:
		SphereMesh();
		~SphereMesh();

		void CreateSphereMesh(float radius = 1.0f);
		void UpdateConstantBuffer();
		void ConstructVertexBuffer();
		void Update();
		void Draw(D3D& d3d);

		void RecursiveTriangle(DirectX::XMFLOAT3 a, DirectX::XMFLOAT3 b, DirectX::XMFLOAT3 c, short level);
		Toast::NextSphereTriangle CheckSphereTriangleSplit(DirectX::XMFLOAT3 a, DirectX::XMFLOAT3 b, DirectX::XMFLOAT3 c, short level);
		void GenerateSphereCellGeometry(int maxPlanetCellLevel);
		void GenerateSphereCells();
		void GenerateDistanceLUT();

		Material* mMaterial;
		std::vector<SphereVertex> mSphereCellVertices;
		std::vector<uint32_t> mSphereCellIndices;
		std::vector<SphereCell> mSphereCells;
		float mSphereCellPatchDelta;
		float mSphereMaxTriangleSize;
		int mSphereMaxLevels, mCellLevels;
		bool mIsPlanet;
		float mRadius;

		std::vector<SphereTriangle> mIcosphere;
		std::vector<float> mDistanceLUT;

		ID3D11Buffer* mVertexBuffer;
		ID3D11Buffer* mIndexBuffer;
		ID3D11Buffer* mInstanceBuffer;

		size_t iVertexBuffer; //index to the vertexbuffer in resourcemanager
		size_t iIndexBuffer; // index to the indexbuffer in resourcemanager
		size_t iInstanceBuffer; // index to the instancebuffer in resourcemanager

		MorphConstantBufferData mConstantBufferData;
		ID3D11Buffer *mConstantBuffer = nullptr;

		DirectX::XMMATRIX mRotationMatrix;
	};
};