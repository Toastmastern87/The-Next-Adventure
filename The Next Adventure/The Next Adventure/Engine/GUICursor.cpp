#include "Material.h"
#include "GUICursor.h"

namespace Toast 
{
	GUICursor::GUICursor()
	{
	}

	GUICursor::~GUICursor()
	{
		mVertexBuffer = nullptr;
		mIndexBuffer = nullptr;

		mMaterial = nullptr;
	}

	void GUICursor::CreateCursor(POINT pos, float width, float height)
	{
		mWidth = width;
		mHeight = height;
		mPosX = static_cast<float>(pos.x);
		mPosY = static_cast<float>(pos.y);
		mMinX = 0.0f;
		mMinY = 0.0f;
		mVisible = true;

		mMaxX = Toast::System::tSys->mSettings["WindowSizeX"] - mWidth;
		mMaxY = Toast::System::tSys->mSettings["WindowSizeY"] - mHeight;

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

		mOldPos = pos;
	}

	void GUICursor::ConstructVertexBuffer()
	{
		iVertexBuffer = Toast::Resources::sResources->CreateVertexBuffer(mVertices);
		iIndexBuffer = Toast::Resources::sResources->CreateIndexBuffer(mIndices);

		mVertexBuffer = Toast::Resources::sResources->mVertexBuffers[iVertexBuffer];
		mIndexBuffer = Toast::Resources::sResources->mIndexBuffers[iIndexBuffer];
	}

	void GUICursor::UpdatePos(POINT pos)
	{
		mPosX = static_cast<float>(pos.x);
		mPosY = static_cast<float>(pos.y);

		if (pos.x != mOldPos.x || pos.y != mOldPos.y) 
		{
			for (auto& vertex : mVertices)
			{
				vertex.position.x += (static_cast<float>(pos.x) - static_cast<float>(mOldPos.x));
				vertex.position.y -= (static_cast<float>(pos.y) - static_cast<float>(mOldPos.y));
			}

			Toast::Resources::sResources->RemapVertexBuffer(mVertices, iVertexBuffer);
		}

		mOldPos = pos;
	}

	void GUICursor::Draw(D3D &d3d)
	{
		if (mVisible) {
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
	}

	void GUICursor::CheckRayIntersection2D(Toast::GUIPanel *panel)
	{
		if ((panel->mPosX >= mPosX) && (panel->mPosX <= (mPosX + mWidth)))
		{
			if (panel->mTitleBar)
			{
				if ((panel->mPosY >= mPosY) && (panel->mPosY <= (mPosY + panel->mTitleBarOffset)))
				{
					panel->mTargeted = true;

					panel->mMinX = panel->mPosX - mPosX;
					panel->mMinY = panel->mPosY - mPosY;
					panel->mMaxX = Toast::System::tSys->mSettings["WindowSizeX"] - ((mPosX + mWidth) - mPosX);
					panel->mMaxY = Toast::System::tSys->mSettings["WindowSizeY"] - mHeight - ((mPosY + panel->mTitleBarOffset) - panel->mPosY);
				}
			}
			else
			{
				if ((panel->mPosY >= mPosY) && (panel->mPosY <= (mPosY + mHeight)))
				{
					panel->mClicked = true;
				}
			}
		}
	}
}