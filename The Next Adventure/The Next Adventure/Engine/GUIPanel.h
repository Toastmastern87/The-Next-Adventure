#pragma once

#include <d3d11.h>
#include <DirectXMath.h>
#include <SpriteFont.h>
#include <SpriteBatch.h>
#include <vector>
#include "System.h"
#include "GUIText.h"

namespace Toast
{
	struct GUIVertex
	{
		DirectX::XMFLOAT3 position = DirectX::XMFLOAT3(0.0f, 0.0f, 0.0f);
		DirectX::XMFLOAT2 uv = DirectX::XMFLOAT2(0.0f, 0.0f);

		GUIVertex(DirectX::XMFLOAT3 pos, DirectX::XMFLOAT2 UV)
		{
			position = pos;
			uv = UV;
		}
	};

	class Material;
	class Resources;
	class D3D;
	class GUIText;

	class GUIPanel
	{
	public:
		GUIPanel();
		~GUIPanel();

		void CreatePanel(float startX, float startY, float width, float height, std::string *titleText = nullptr, bool titleBar = false);
		void ConstructVertexBuffer();
		void UpdatePos(POINT deltaPos);
		void CheckEdges();
		void SetPos(float posX, float posY);
		void Draw(D3D& d3d);

		void AddElement(Toast::GUIText *newElement);

		bool mTitleBar, mVisible = true, mTargeted = false, mClicked = false;
		float mWidth = 0.0f, mHeight = 0.0f, mPosX = 0.0f, mPosY = 0.0f, mOldPosX, mOldPosY;
		float mMinX = 0.0f, mMinY = 0.0f, mMaxX = 0.0f, mMaxY = 0.0f;
		float mTitleBarOffset = 0.0f;

		Toast::GUIText* mTitle = nullptr;
		Toast::GUIFont* mTitleFont = nullptr;

		Material* mMaterial = nullptr;
		Material* mTitleBarMaterial = nullptr;
		Material* mTitleFontMaterial = nullptr;
		Material* mBorderMaterial = nullptr;
		std::vector<GUIVertex> mVertices;
		std::vector<GUIVertex> mTitleBarVertices;
		std::vector<uint32_t> mIndices;
		size_t iVertexBuffer; //index to the vertexbuffer in resourcemanager
		size_t iTitleBarVertexBuffer; //index to the vertexbuffer in resourcemanager
		size_t iIndexBuffer; // index to the indexbuffer in resourcemanager

		ID3D11Buffer* mVertexBuffer = nullptr;
		ID3D11Buffer* mTitleBarVertexBuffer = nullptr;
		ID3D11Buffer* mIndexBuffer = nullptr;

		std::vector<Toast::GUIText*> mElements;
	};
}