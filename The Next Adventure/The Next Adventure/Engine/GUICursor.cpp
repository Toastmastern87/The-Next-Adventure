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
		mVertices.push_back(Toast::GUIVertex(DirectX::XMFLOAT3(static_cast<float>(pos.x) - (Toast::System::tSys->mSettings["WindowSizeX"] / 2.0f), ((Toast::System::tSys->mSettings["WindowSizeY"] / 2.0f) - static_cast<float>(pos.y)), 0.0f), DirectX::XMFLOAT2(0.0f, 0.0f)));
		mVertices.push_back(Toast::GUIVertex(DirectX::XMFLOAT3((static_cast<float>(pos.x) - (Toast::System::tSys->mSettings["WindowSizeX"] / 2.0f) + mWidth), ((Toast::System::tSys->mSettings["WindowSizeY"] / 2.0f) - static_cast<float>(pos.y)), 0.0f), DirectX::XMFLOAT2(1.0f, 0.0f)));
		mVertices.push_back(Toast::GUIVertex(DirectX::XMFLOAT3((static_cast<float>(pos.x) - (Toast::System::tSys->mSettings["WindowSizeX"] / 2.0f) + mWidth), ((Toast::System::tSys->mSettings["WindowSizeY"] / 2.0f) - mHeight - static_cast<float>(pos.y)), 0.0f), DirectX::XMFLOAT2(1.0f, 1.0f)));
		mVertices.push_back(Toast::GUIVertex(DirectX::XMFLOAT3(static_cast<float>(pos.x) - (Toast::System::tSys->mSettings["WindowSizeX"] / 2.0f), ((Toast::System::tSys->mSettings["WindowSizeY"] / 2.0f) - mHeight- static_cast<float>(pos.y)), 0.0f), DirectX::XMFLOAT2(0.0f, 1.0f)));

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
		pos = CheckBoundaries(pos);

		//Updates delta position which is the change of position since last check
		mDeltaPos.x = pos.x - mOldPos.x;
		mDeltaPos.y = pos.y - mOldPos.y;

		mPosX = static_cast<float>(pos.x);
		mPosY = static_cast<float>(pos.y);

		if (pos.x != mOldPos.x || pos.y != mOldPos.y) 
		{
			for (auto& vertex : mVertices)
			{
				vertex.position.x += static_cast<float>(mDeltaPos.x);
				vertex.position.y -= static_cast<float>(mDeltaPos.y);
			}

			Toast::Resources::sResources->RemapVertexBuffer(mVertices, iVertexBuffer);
		}

		mOldPos = pos;
	}

	void GUICursor::HideCursor() 
	{
		GetCursorPos(&mHiddenPos);
		mVisible = false;
	}

	void GUICursor::ShowCursor() 
	{
		SetCursorPos(static_cast<int>(mHiddenPos.x), static_cast<int>(mHiddenPos.y));
		mVisible = true;
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

	POINT GUICursor::CheckBoundaries(POINT pos)
	{
		// Checks so that the GUICursor isn't move while for exampla a GUIPanel has hit it the edge
		if (static_cast<float>(pos.x) >= mMaxX)	
			pos.x = static_cast<long>(mMaxX);

		if (static_cast<float>(pos.x) <= mMinX)
			pos.x = static_cast<long>(mMinX);

		// Checks so that the GUICursor isn't move while for exampla a GUIPanel has hit it the edge
		if (static_cast<float>(pos.y) >= mMaxY)
			pos.y = static_cast<long>(mMaxY);

		if (static_cast<float>(pos.y) <= mMinY)
			pos.y = static_cast<long>(mMinY);

		return pos;
	}

	void GUICursor::CheckRayIntersection2D(Toast::GUIPanel *panel)
	{
		if ((mPosX >= panel->mPosX) && (mPosX <= (panel->mPosX + panel->mWidth)))
		{
			if (panel->mTitleBar)
			{
				if ((mPosY >= panel->mPosY) && (mPosY <= (panel->mPosY + panel->mTitleBarOffset)))
				{
					panel->mTargeted = true;

					mMinX = mPosX - panel->mPosX;
					mMinY = mPosY - panel->mPosY;
					mMaxX = Toast::System::tSys->mSettings["WindowSizeX"] - ((panel->mPosX + panel->mWidth) - mPosX);
					mMaxY = Toast::System::tSys->mSettings["WindowSizeY"] - panel->mHeight - ((panel->mPosY + panel->mTitleBarOffset) - mPosY);
				}
			}
			else 
			{
				if ((mPosY >= panel->mPosY) && (mPosY <= (panel->mPosY + panel->mHeight)))
				{
					panel->mClicked = true;
				}
			}
		}
	}

	void GUICursor::CheckRayIntersection3D(DirectX::XMVECTOR cameraPos, DirectX::XMVECTOR pickingRay, Toast::Object3D* object)
	{
		float dist;

		object->mTargeted = object->mBoundingOrientedBox.Intersects(cameraPos, pickingRay, dist);
	}

	DirectX::XMVECTOR GUICursor::GetPickingRay(POINT cursorPos, DirectX::XMMATRIX projectionMatrix, DirectX::XMMATRIX viewMatrix)
	{
		float pointX, pointY;
		DirectX::XMFLOAT4X4 projectionMatrixXMFLOAT4X4, inverseViewMatrixXMFLOAT4X4;
		DirectX::XMFLOAT3 ret;
		DirectX::XMMATRIX inverseViewMatrix;

		DirectX::XMStoreFloat4x4(&projectionMatrixXMFLOAT4X4, projectionMatrix);

		inverseViewMatrix = DirectX::XMMatrixInverse(NULL, viewMatrix);
		DirectX::XMStoreFloat4x4(&inverseViewMatrixXMFLOAT4X4, inverseViewMatrix);

		// Move the mouse cursor coordinates into the -1 to +1 range.
		pointX = ((2.0f * static_cast<float>(cursorPos.x)) / (float)Toast::System::tSys->mSettings["WindowSizeX"]) - 1.0f;
		pointY = ((2.0f * static_cast<float>(cursorPos.y)) / (float)Toast::System::tSys->mSettings["WindowSizeY"]) - 1.0f;

		// Adjust the points using the projection matrix to account for the aspect ratio of the viewport.
		pointX = pointX / projectionMatrixXMFLOAT4X4._11;
		pointY = pointY / projectionMatrixXMFLOAT4X4._22;

		// Calculate the direction of the picking ray in view space.
		ret.x = (pointX * inverseViewMatrixXMFLOAT4X4._11) + (pointY * inverseViewMatrixXMFLOAT4X4._21) + inverseViewMatrixXMFLOAT4X4._31;
		ret.y = (pointX * inverseViewMatrixXMFLOAT4X4._12) + (pointY * inverseViewMatrixXMFLOAT4X4._22) + inverseViewMatrixXMFLOAT4X4._32;
		ret.z = (pointX * inverseViewMatrixXMFLOAT4X4._13) + (pointY * inverseViewMatrixXMFLOAT4X4._23) + inverseViewMatrixXMFLOAT4X4._33;

		return DirectX::XMVector3Normalize(DirectX::XMLoadFloat3(&ret));
	}
}