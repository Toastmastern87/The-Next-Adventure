#include "GUIText.h"

namespace Toast 
{
	GUIText::GUIText() 
	{
	}

	GUIText::~GUIText() 
	{
		mVertexBuffer = nullptr;
		mIndexBuffer = nullptr;

		mMaterial = nullptr;
	}

	void GUIText::CreateText(float x, float y, std::string* text, Toast::GUIFont* font, Toast::Material *material)
	{
		int letterASCII, nrOfSpaces;

		nrOfSpaces = 0;
		mFont = font;
		mMaterial = material;

		for (int i = 0; i < text->length(); i++)
		{
			letterASCII = static_cast<int>(text->at(i)) - 32;

			if (letterASCII == 0)
			{
				x += mSpaceSize;
				nrOfSpaces++;
			}
			else
			{
				float posX, posY, uvX, uvY;

				// UPPER LEFT
				posX = x - (Toast::System::tSys->mSettings["WindowSizeX"] / 2.0f);
				posY = (Toast::System::tSys->mSettings["WindowSizeY"] / 2.0f) - y;
				uvX = (mFont->xVec[letterASCII]) / 512.0f;
				uvY = (mFont->yVec[letterASCII]) / 512.0f;

				mVertices.push_back(Toast::GUIVertex(DirectX::XMFLOAT3(posX, posY, 0.0f), DirectX::XMFLOAT2(uvX, uvY)));

				// UPPER RIGHT
				posX = x - (Toast::System::tSys->mSettings["WindowSizeX"] / 2.0f) + mFont->widthVec[letterASCII];
				posY = (Toast::System::tSys->mSettings["WindowSizeY"] / 2.0f) - y;
				uvX = (mFont->xVec[letterASCII] + mFont->widthVec[letterASCII]) / 512.0f;
				uvY = (mFont->yVec[letterASCII]) / 512.0f;

				mVertices.push_back(Toast::GUIVertex(DirectX::XMFLOAT3(posX, posY, 0.0f), DirectX::XMFLOAT2(uvX, uvY)));

				// LOWER RIGHT
				posX = x - (Toast::System::tSys->mSettings["WindowSizeX"] / 2.0f) + mFont->widthVec[letterASCII];
				posY = (Toast::System::tSys->mSettings["WindowSizeY"] / 2.0f) - y - mFont->heightVec[letterASCII];
				uvX = (mFont->xVec[letterASCII] + mFont->widthVec[letterASCII]) / 512.0f;
				uvY = (mFont->yVec[letterASCII] + mFont->heightVec[letterASCII]) / 512.0f;

				mVertices.push_back(Toast::GUIVertex(DirectX::XMFLOAT3(posX, posY, 0.0f), DirectX::XMFLOAT2(uvX, uvY)));

				// LOWER LEFT
				posX = x - (Toast::System::tSys->mSettings["WindowSizeX"] / 2.0f);
				posY = (Toast::System::tSys->mSettings["WindowSizeY"] / 2.0f) - y - mFont->heightVec[letterASCII];
				uvX = (mFont->xVec[letterASCII]) / 512.0f;
				uvY = (mFont->yVec[letterASCII] + mFont->heightVec[letterASCII]) / 512.0f;

				mVertices.push_back(Toast::GUIVertex(DirectX::XMFLOAT3(posX, posY, 0.0f), DirectX::XMFLOAT2(uvX, uvY)));

				int iOffset = (i - nrOfSpaces) * 4;

				mIndices.push_back(iOffset);
				mIndices.push_back(iOffset + 1);
				mIndices.push_back(iOffset + 3);
				mIndices.push_back(iOffset + 3);
				mIndices.push_back(iOffset + 1);
				mIndices.push_back(iOffset + 2);

				x += mFont->widthVec[letterASCII];
			}
		}

		ConstructVertexBuffer();
	}

	void GUIText::ConstructVertexBuffer()
	{
		iVertexBuffer = Toast::Resources::sResources->CreateVertexBuffer(mVertices, static_cast<size_t>(400));
		iIndexBuffer = Toast::Resources::sResources->CreateIndexBuffer(mIndices, static_cast<size_t>(400));

		mVertexBuffer = Toast::Resources::sResources->mVertexBuffers[iVertexBuffer];
		mIndexBuffer = Toast::Resources::sResources->mIndexBuffers[iIndexBuffer];
	}

	void GUIText::Draw(D3D& d3d) 
	{
		uint32_t stride[] = { sizeof(Toast::GUIVertex) };
		uint32_t offset[] = { 0 };

		d3d.UploadConstantBuffersToGPU();

		d3d.mDeviceContext->IASetVertexBuffers(0, 1, &mVertexBuffer, stride, offset);
		d3d.mDeviceContext->IASetIndexBuffer(mIndexBuffer, DXGI_FORMAT_R32_UINT, 0);

		d3d.mDeviceContext->PSSetSamplers(0, 1, &mMaterial->mTexture->mSamplerState);
		d3d.mDeviceContext->PSSetShaderResources(0, 1, &mMaterial->mTexture->mView);
		d3d.mDeviceContext->VSSetShader(mMaterial->mVertexShader, 0, 0);
		d3d.mDeviceContext->PSSetShader(mMaterial->mPixelShader, 0, 0);

		d3d.mDeviceContext->DrawIndexed(static_cast<int>(mIndices.size()), 0, 0);
	}

	void GUIText::UpdatePos(float deltaX, float deltaY)
	{
		for (auto& vertex : mVertices) 
		{
			vertex.position.x += deltaX;
			vertex.position.y -= deltaY;
		}

		Toast::Resources::sResources->RemapVertexBuffer(mVertices, iVertexBuffer);
	}

	void GUIText::UpdateText(std::string* newText)
	{
		int letterASCII, nrOfSpaces;

		// Sets the start position of the text
		float x = mVertices[0].position.x;
		float y = mVertices[0].position.y;

		nrOfSpaces = 0;

		mVertices.clear();
		mIndices.clear();

		for (int i = 0; i < newText->length(); i++)
		{
			letterASCII = static_cast<int>(newText->at(i)) - 32;

			if (letterASCII == 0)
			{
				x += mSpaceSize;
				nrOfSpaces++;
			}
			else
			{
				float posX, posY, uvX, uvY;

				// UPPER LEFT
				posX = x;
				posY = y;
				uvX = (mFont->xVec[letterASCII]) / 512.0f;
				uvY = (mFont->yVec[letterASCII]) / 512.0f;

				mVertices.push_back(Toast::GUIVertex(DirectX::XMFLOAT3(posX, posY, 0.0f), DirectX::XMFLOAT2(uvX, uvY)));

				// UPPER RIGHT
				posX = x + mFont->widthVec[letterASCII];
				posY = y;
				uvX = (mFont->xVec[letterASCII] + mFont->widthVec[letterASCII]) / 512.0f;
				uvY = (mFont->yVec[letterASCII]) / 512.0f;

				mVertices.push_back(Toast::GUIVertex(DirectX::XMFLOAT3(posX, posY, 0.0f), DirectX::XMFLOAT2(uvX, uvY)));

				// LOWER RIGHT
				posX = x + mFont->widthVec[letterASCII];
				posY = y - mFont->heightVec[letterASCII];
				uvX = (mFont->xVec[letterASCII] + mFont->widthVec[letterASCII]) / 512.0f;
				uvY = (mFont->yVec[letterASCII] + mFont->heightVec[letterASCII]) / 512.0f;

				mVertices.push_back(Toast::GUIVertex(DirectX::XMFLOAT3(posX, posY, 0.0f), DirectX::XMFLOAT2(uvX, uvY)));

				// LOWER LEFT
				posX = x;
				posY = y - mFont->heightVec[letterASCII];
				uvX = (mFont->xVec[letterASCII]) / 512.0f;
				uvY = (mFont->yVec[letterASCII] + mFont->heightVec[letterASCII]) / 512.0f;

				mVertices.push_back(Toast::GUIVertex(DirectX::XMFLOAT3(posX, posY, 0.0f), DirectX::XMFLOAT2(uvX, uvY)));

				int iOffset = (i - nrOfSpaces) * 4;

				mIndices.push_back(iOffset);
				mIndices.push_back(iOffset + 1);
				mIndices.push_back(iOffset + 3);
				mIndices.push_back(iOffset + 3);
				mIndices.push_back(iOffset + 1);
				mIndices.push_back(iOffset + 2);				

				x += mFont->widthVec[letterASCII];
			}
		}

		Toast::Resources::sResources->RemapVertexBuffer(mVertices, iVertexBuffer);
		Toast::Resources::sResources->RemapIndexBuffer(mIndices, iIndexBuffer);
	}
}