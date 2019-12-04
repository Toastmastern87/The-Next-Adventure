#include "World.h"

namespace Toast
{
	World::World()
	{
	}

	World::~World()
	{
		if (mConstantBuffer)
		{
			mConstantBuffer->Release();
			mConstantBuffer = 0;
		}
	}

	void World::CreateWorld(int levels, int cellLevels, bool frontCulling, float radius, float minAlt, float maxAlt)
	{
		SphereMesh *sphereMesh = new SphereMesh();
		Toast::System::tSys->mResources->mSphereMeshes.push_back(sphereMesh);

		sphereMesh->mSphereMaxTriangleSize = 400.0f;
		sphereMesh->mCellLevels = cellLevels;
		sphereMesh->mSphereMaxLevels = levels;
		sphereMesh->mRadius = radius;
		sphereMesh->mIsPlanet = true;

		mMinAltitude = minAlt;
		mMaxAltitude = maxAlt;
		mRadius = radius;

		mFrontCulling = frontCulling;
		sphereMesh->CreateSphereMesh(radius);
		mSphereMeshes.push_back(sphereMesh);
	}

	void World::Draw(D3D& d3d)
	{
		d3d.SetPlanetConstantBuffer(mConstantBuffer);

		d3d.mDeviceContext->IASetInputLayout(this->mSphereMeshes[0]->mMaterial->mInputLayout);
		mSphereMeshes[0]->Update();
		mSphereMeshes[0]->Draw(d3d);
	}

	void World::Update()
	{
		Toast::D3D* d3d = Toast::System::tSys->mD3D;

		mPlanetBufferData.worldRadius = DirectX::XMFLOAT4(mRadius, mRadius, mRadius, mRadius);
		mPlanetBufferData.minAltitude = DirectX::XMFLOAT4(mMinAltitude, mMinAltitude, mMinAltitude, mMinAltitude);
		mPlanetBufferData.maxAltitude = DirectX::XMFLOAT4(mMaxAltitude, mMaxAltitude, mMaxAltitude, mMaxAltitude);

		if (!mConstantBuffer)
		{
			// Fill in a buffer description.
			D3D11_BUFFER_DESC cbDesc;
			cbDesc.ByteWidth = sizeof(PlanetBufferData);
			cbDesc.Usage = D3D11_USAGE_DYNAMIC;
			cbDesc.BindFlags = D3D11_BIND_CONSTANT_BUFFER;
			cbDesc.CPUAccessFlags = D3D11_CPU_ACCESS_WRITE;
			cbDesc.MiscFlags = 0;
			cbDesc.StructureByteStride = 0;

			// Fill in the subresource data.
			D3D11_SUBRESOURCE_DATA InitData;
			InitData.pSysMem = &mPlanetBufferData;
			InitData.SysMemPitch = 0;
			InitData.SysMemSlicePitch = 0;
			// Create the buffer.
			Toast::System::tSys->mD3D->mDevice->CreateBuffer(&cbDesc, &InitData, &mConstantBuffer);
		}

		D3D11_MAPPED_SUBRESOURCE ms;
		d3d->mDeviceContext->Map(mConstantBuffer, NULL, D3D11_MAP_WRITE_DISCARD, NULL, &ms);
		memcpy(ms.pData, &mPlanetBufferData, sizeof(PlanetBufferData));
		d3d->mDeviceContext->Unmap(mConstantBuffer, NULL);

		d3d->SetPlanetConstantBuffer(mConstantBuffer);
	}
}