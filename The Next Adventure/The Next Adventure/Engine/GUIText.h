#pragma once

#include "System.h"

namespace Toast
{
	struct GUIFont
	{
		std::vector<float> xVec, yVec, widthVec, heightVec;
		Toast::Texture* texture;

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

		Toast::Material* mMaterial;
		std::string mText;
		float mSpaceSize = 5.0f;
		Toast::GUIFont* mFont;

		std::vector<GUIVertex> mVertices;
		std::vector<uint32_t> mIndices;
		size_t iVertexBuffer; //index to the vertexbuffer in resourcemanager
		size_t iIndexBuffer; // index to the indexbuffer in resourcemanager

		ID3D11Buffer* mVertexBuffer;
		ID3D11Buffer* mIndexBuffer;
	};
}