#pragma once

#include <d3d11.h>
#include <DirectXMath.h>
#include <SpriteFont.h>
#include <SpriteBatch.h>
#include <vector>
#include "System.h"

namespace Toast
{
	class Material;
	class Resources;
	class D3D;
	class GUIPanel;

	class GUICursor
	{
	public:
		GUICursor();
		~GUICursor();

		void CreateCursor(POINT pos, float width, float height);
		void ConstructVertexBuffer();
		void UpdatePos(POINT pos);
		POINT CheckBoundaries(POINT pos);
		void HideCursor();
		void ShowCursor();
		void Draw(D3D& d3d);
		void CheckRayIntersection2D(Toast::GUIPanel *panel);
		DirectX::XMVECTOR GetPickingRay(DirectX::XMFLOAT2, DirectX::XMMATRIX, DirectX::XMMATRIX);

		POINT GetDeltaPos() { return mDeltaPos; }

		bool mVisible = true;
		float mWidth = 0.0f, mHeight = 0.0f, mPosX = 0.0f, mPosY = 0.0f, mOldPosX, mOldPosY;
		float mMinX = 0.0f, mMinY = 0.0f, mMaxX = 0.0f, mMaxY = 0.0f;

		POINT mPos, mOldPos, mDeltaPos, mHiddenPos;

		Material* mMaterial = nullptr;

		std::vector<GUIVertex> mVertices;
		std::vector<uint32_t> mIndices;
		size_t iVertexBuffer; //index to the vertexbuffer in resourcemanager
		size_t iIndexBuffer; // index to the indexbuffer in resourcemanager

		ID3D11Buffer* mVertexBuffer = nullptr;;
		ID3D11Buffer* mIndexBuffer = nullptr;;
	};
}