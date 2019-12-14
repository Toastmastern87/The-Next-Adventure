#pragma once

#include "System.h"

namespace Toast
{
	struct GUIFont
	{
		std::vector<float> xVec, yVec, widthVec, heightVec;
		Toast::Texture* texture = nullptr;

		GUIFont()
		{
		}

		~GUIFont() 
		{
		}
	};

	class GUIText
	{
	public:
		GUIText();
		~GUIText();

		void CreateText(float x, float y, std::string* text, Toast::GUIFont* font, Toast::Material* material);
		void ConstructVertexBuffer();
		void Draw(D3D& d3d);
		void UpdateText(std::string* newText);
		void UpdatePos(float deltaX, float deltaY);

		Toast::Material* mMaterial = nullptr;
		std::string mText = "";
		float mSpaceSize = 5.0f;
		Toast::GUIFont* mFont = nullptr;

		std::vector<GUIVertex> mVertices;
		std::vector<uint32_t> mIndices;
		size_t iVertexBuffer = 0; //index to the vertexbuffer in resourcemanager
		size_t iIndexBuffer = 0; // index to the indexbuffer in resourcemanager

		ID3D11Buffer* mVertexBuffer = nullptr;
		ID3D11Buffer* mIndexBuffer = nullptr;
	};
}