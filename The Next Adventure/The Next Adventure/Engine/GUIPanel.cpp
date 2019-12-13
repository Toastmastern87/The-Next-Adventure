#include "Material.h"
#include "GUIPanel.h"

namespace Toast 
{
	GUIPanel::GUIPanel() 
	{
	}

	GUIPanel::~GUIPanel() 
	{
		mVertexBuffer = nullptr;
		mIndexBuffer = nullptr;

		mMaterial = nullptr;

		delete mTitle;

		for (auto i : mElements)
		{
			delete i;
			i = nullptr;
		}
		mElements.clear();
	}

	void GUIPanel::CreatePanel(float startX, float startY, float width, float height, std::string *titleText, bool titleBar)
	{
		mTitleBar = titleBar;
		mWidth = width;
		mHeight = height;
		mPosX = startX;
		mPosY = startY;
		mMinX = 0.0f;
		mMinY = 0.0f;
		mVisible = true;

		mMaxX = Toast::System::tSys->mSettings["WindowSizeX"] - mWidth;
		mMaxY = Toast::System::tSys->mSettings["WindowSizeY"] - mHeight;

		if (mTitleBar)
		{
			mTitleBarVertices.push_back(Toast::GUIVertex(DirectX::XMFLOAT3(mPosX - (Toast::System::tSys->mSettings["WindowSizeX"] / 2.0f), ((Toast::System::tSys->mSettings["WindowSizeY"] / 2.0f) - mPosY), 0.0f), DirectX::XMFLOAT2(0.0f, 0.0f)));
			mTitleBarVertices.push_back(Toast::GUIVertex(DirectX::XMFLOAT3((mPosX - (Toast::System::tSys->mSettings["WindowSizeX"] / 2.0f) + mWidth), ((Toast::System::tSys->mSettings["WindowSizeY"] / 2.0f) - mPosY), 0.0f), DirectX::XMFLOAT2(1.0f, 0.0f)));
			mTitleBarVertices.push_back(Toast::GUIVertex(DirectX::XMFLOAT3((mPosX - (Toast::System::tSys->mSettings["WindowSizeX"] / 2.0f) + mWidth), ((Toast::System::tSys->mSettings["WindowSizeY"] / 2.0f) - mPosY - 20.0f), 0.0f), DirectX::XMFLOAT2(1.0f, 1.0f)));
			mTitleBarVertices.push_back(Toast::GUIVertex(DirectX::XMFLOAT3(mPosX - (Toast::System::tSys->mSettings["WindowSizeX"] / 2.0f), ((Toast::System::tSys->mSettings["WindowSizeY"] / 2.0f) - mPosY - 20.0f), 0.0f), DirectX::XMFLOAT2(0.0f, 1.0f)));

			mTitleBarOffset = 20.0f;

			D3D11_INPUT_ELEMENT_DESC GUIIED[] =
			{
				{ "POSITION" , 0, DXGI_FORMAT_R32G32B32_FLOAT, 0, 0, D3D11_INPUT_PER_VERTEX_DATA, 0 },
				{ "TEXCOORD", 0, DXGI_FORMAT_R32G32_FLOAT, 0, D3D11_APPEND_ALIGNED_ELEMENT, D3D11_INPUT_PER_VERTEX_DATA, 0 }
			};

			mTitleFontMaterial = Resources::sResources->GetMaterial("SegoeUIFont", "SegoeUI", "GUI", GUIIED, 2);

			std::string fontName = "SegoeUI";
			mTitleFont = Resources::sResources->LoadFontData(fontName);

			mTitle = new Toast::GUIText();
			mTitle->CreateText((mPosX + 10.0f), mPosY, titleText, mTitleFont, mTitleFontMaterial);
		}

		// The first XMFLOAT3 is the positions of the vertices and the second, XMFLOAT2 is the UV-coordinates
		mVertices.push_back(Toast::GUIVertex(DirectX::XMFLOAT3(mPosX - (Toast::System::tSys->mSettings["WindowSizeX"] / 2.0f), ((Toast::System::tSys->mSettings["WindowSizeY"] / 2.0f) - mTitleBarOffset - mPosY), 0.0f), DirectX::XMFLOAT2(0.0f, 0.0f)));
		mVertices.push_back(Toast::GUIVertex(DirectX::XMFLOAT3((mPosX - (Toast::System::tSys->mSettings["WindowSizeX"] / 2.0f) + mWidth), ((Toast::System::tSys->mSettings["WindowSizeY"] / 2.0f) - mTitleBarOffset - mPosY), 0.0f), DirectX::XMFLOAT2(1.0f, 0.0f)));
		mVertices.push_back(Toast::GUIVertex(DirectX::XMFLOAT3((mPosX - (Toast::System::tSys->mSettings["WindowSizeX"] / 2.0f) + mWidth), ((Toast::System::tSys->mSettings["WindowSizeY"] / 2.0f) - mHeight - mTitleBarOffset - mPosY), 0.0f), DirectX::XMFLOAT2(1.0f, 1.0f)));
		mVertices.push_back(Toast::GUIVertex(DirectX::XMFLOAT3(mPosX - (Toast::System::tSys->mSettings["WindowSizeX"] / 2.0f), ((Toast::System::tSys->mSettings["WindowSizeY"] / 2.0f) - mHeight - mTitleBarOffset - mPosY), 0.0f), DirectX::XMFLOAT2(0.0f, 1.0f)));

		mIndices.push_back(0);
		mIndices.push_back(1);
		mIndices.push_back(3);
		mIndices.push_back(3);
		mIndices.push_back(1);
		mIndices.push_back(2);

		ConstructVertexBuffer();
	}

	void GUIPanel::ConstructVertexBuffer() 
	{
		iVertexBuffer = Toast::Resources::sResources->CreateVertexBuffer(mVertices);
		iIndexBuffer = Toast::Resources::sResources->CreateIndexBuffer(mIndices);

		if (mTitleBar) 
		{
			iTitleBarVertexBuffer = Toast::Resources::sResources->CreateVertexBuffer(mTitleBarVertices);
		}

		mVertexBuffer = Toast::Resources::sResources->mVertexBuffers[iVertexBuffer];
		mIndexBuffer = Toast::Resources::sResources->mIndexBuffers[iIndexBuffer];

		if (mTitleBar)
		{
			mTitleBarVertexBuffer = Toast::Resources::sResources->mVertexBuffers[iTitleBarVertexBuffer];
		}
	}

	void GUIPanel::UpdatePos(POINT deltaPos)
	{
		mPosX += static_cast<float>(deltaPos.x);
		mPosY += static_cast<float>(deltaPos.y);

		//mMaxX = Toast::System::tSys->mSettings["WindowSizeX"] - mWidth;

		//Toast::System::tSys->Print("Pos X: %f", mPosX);
		//Toast::System::tSys->Print("Pos Y: %f", mPosY);
		//Toast::System::tSys->Print("Width: %f", mWidth);
		//Toast::System::tSys->Print("Max pos Y: %f", mMaxY);
		//Toast::System::tSys->Print("Min pos X: %f", mMinX);
		//Toast::System::tSys->Print("Min pos Y: %f", mMinY);

		//Check the edges, this 
		CheckEdges();

		if (mPosY != mOldPosY || mPosX != mOldPosX) 
		{
			if (mTitleBar)
			{
				// Moves the title according to the window movement and not by following the deltaX and Y of the cursor
				mTitle->UpdatePos((mPosX - mOldPosX), (mPosY - mOldPosY));

				for (auto& vertex : mTitleBarVertices)
				{
					vertex.position.x += (mPosX - mOldPosX);
					vertex.position.y -= (mPosY - mOldPosY);

					Toast::Resources::sResources->RemapVertexBuffer(mTitleBarVertices, iTitleBarVertexBuffer);
				}
			}

			for (auto& vertex : mVertices)
			{
				vertex.position.x += (mPosX - mOldPosX);
				vertex.position.y -= (mPosY - mOldPosY);
			}

			Toast::Resources::sResources->RemapVertexBuffer(mVertices, iVertexBuffer);

			// Updates the position of all elements in the panel
			for (auto& elements : mElements)
			{
				elements->UpdatePos((mPosX - mOldPosX), (mPosY - mOldPosY));
			}
		}

		mOldPosX = mPosX;
		mOldPosY = mPosY;
	}

	void GUIPanel::SetPos(float posX, float posY)
	{
		mPosX = posX;
		mPosY = posY;

		//Check the edges, this 
		CheckEdges();

		if (mPosY != mOldPosY || mPosX != mOldPosX)
		{
			if (mTitleBar)
			{
				// Moves the title according to the window movement and not by following the deltaX and Y of the cursor
				mTitle->UpdatePos(mPosX, mPosY);

				for (auto& vertex : mTitleBarVertices)
				{
					vertex.position.x = mPosX;
					vertex.position.y = mPosY;

					Toast::Resources::sResources->RemapVertexBuffer(mTitleBarVertices, iTitleBarVertexBuffer);
				}
			}

			for (auto& vertex : mVertices)
			{
				vertex.position.x = mPosX;
				vertex.position.y = mPosY;
			}

			Toast::Resources::sResources->RemapVertexBuffer(mVertices, iVertexBuffer);

			// Updates the position of all elements in the panel
			for (auto& elements : mElements)
			{
				elements->UpdatePos(mPosX, mPosY);
			}
		}
	}

	void GUIPanel::CheckEdges()
	{
		// Checks so that the GUIPanel isn't moved offscreen
		if (mPosX >= mMaxX)	mPosX = mMaxX;

		if (mPosX <= mMinX) mPosX = mMinX;

		// Checks so that the GUIPanel isn't moved offscreen
		if (mPosY >= mMaxY) mPosY = mMaxY;

		if (mPosY <= mMinY)	mPosY = mMinY;
	}

	void GUIPanel::Draw(D3D &d3d)
	{
		if (mVisible) {
			uint32_t stride[] = { sizeof(Toast::GUIVertex) };
			uint32_t offset[] = { 0 };

			d3d.UploadConstantBuffersToGPU();

			if (mTitleBar)
			{
				d3d.mDeviceContext->IASetVertexBuffers(0, 1, &mTitleBarVertexBuffer, stride, offset);
				d3d.mDeviceContext->IASetIndexBuffer(mIndexBuffer, DXGI_FORMAT_R32_UINT, 0);

				d3d.mDeviceContext->PSSetSamplers(0, 1, &mTitleBarMaterial->mTexture->mSamplerState);
				d3d.mDeviceContext->PSSetShaderResources(0, 1, &mTitleBarMaterial->mTexture->mView);
				d3d.mDeviceContext->VSSetShader(mTitleBarMaterial->mVertexShader, 0, 0);
				d3d.mDeviceContext->PSSetShader(mTitleBarMaterial->mPixelShader, 0, 0);

				d3d.mDeviceContext->DrawIndexed(static_cast<int>(mIndices.size()), 0, 0);

				mTitle->Draw(d3d);
			}

			d3d.mDeviceContext->IASetVertexBuffers(0, 1, &mVertexBuffer, stride, offset);
			d3d.mDeviceContext->IASetIndexBuffer(mIndexBuffer, DXGI_FORMAT_R32_UINT, 0);

			d3d.mDeviceContext->PSSetSamplers(0, 1, &mMaterial->mTexture->mSamplerState);
			d3d.mDeviceContext->PSSetShaderResources(0, 1, &mMaterial->mTexture->mView);
			d3d.mDeviceContext->VSSetShader(mMaterial->mVertexShader, 0, 0);
			d3d.mDeviceContext->PSSetShader(mMaterial->mPixelShader, 0, 0);

			d3d.mDeviceContext->DrawIndexed(static_cast<int>(mIndices.size()), 0, 0);

			for (auto& elements : mElements)
			{
				elements->Draw(d3d);
			}
		}
	}

	void GUIPanel::AddElement(Toast::GUIText* newElement) 
	{
		// Offsets the text compared to the panel.
		newElement->UpdatePos(mPosX, (mPosY + mTitleBarOffset));

		mElements.push_back(newElement);
	}
}