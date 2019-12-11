#include "World.h"

namespace Toast
{
	World::World()
	{
	}

	World::~World()
	{
		if (mPlanetConstantBuffer)
		{
			mPlanetConstantBuffer->Release();
			mPlanetConstantBuffer = 0;
		}

		if (mSunlightConstantBuffer)
		{
			mSunlightConstantBuffer->Release();
			mSunlightConstantBuffer = 0;
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
		d3d.SetPlanetConstantBuffer(mPlanetConstantBuffer);
		d3d.SetSunlightConstantBuffer(mSunlightConstantBuffer);

		d3d.mDeviceContext->IASetInputLayout(this->mSphereMeshes[0]->mMaterial->mInputLayout);
		mSphereMeshes[0]->Update();
		mSphereMeshes[0]->Draw(d3d);
	}

	void World::Update()
	{
		Toast::D3D* d3d = Toast::System::tSys->mD3D;

		//Planet terrain data
		mPlanetBufferData.worldRadius = DirectX::XMFLOAT4(mRadius, mRadius, mRadius, mRadius);
		mPlanetBufferData.minAltitude = DirectX::XMFLOAT4(mMinAltitude, mMinAltitude, mMinAltitude, mMinAltitude);
		mPlanetBufferData.maxAltitude = DirectX::XMFLOAT4(mMaxAltitude, mMaxAltitude, mMaxAltitude, mMaxAltitude);

		if (!mPlanetConstantBuffer)
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
			Toast::System::tSys->mD3D->mDevice->CreateBuffer(&cbDesc, &InitData, &mPlanetConstantBuffer);
		}

		D3D11_MAPPED_SUBRESOURCE ms;
		d3d->mDeviceContext->Map(mPlanetConstantBuffer, NULL, D3D11_MAP_WRITE_DISCARD, NULL, &ms);
		memcpy(ms.pData, &mPlanetBufferData, sizeof(PlanetBufferData));
		d3d->mDeviceContext->Unmap(mPlanetConstantBuffer, NULL);

		d3d->SetPlanetConstantBuffer(mPlanetConstantBuffer);

		//Sunlight
		mSunlightBufferData.sunlightDirection = mSunlightDirection;
		mSunlightBufferData.diffuseColor = mSunlightColor;// DirectX::XMFLOAT4(1.0f, 1.0f, 1.0f, 1.0f);

		if (!mSunlightConstantBuffer)
		{
			// Fill in a buffer description.
			D3D11_BUFFER_DESC cbDesc;
			cbDesc.ByteWidth = sizeof(SunlightBufferData);
			cbDesc.Usage = D3D11_USAGE_DYNAMIC;
			cbDesc.BindFlags = D3D11_BIND_CONSTANT_BUFFER;
			cbDesc.CPUAccessFlags = D3D11_CPU_ACCESS_WRITE;
			cbDesc.MiscFlags = 0;
			cbDesc.StructureByteStride = 0;

			// Fill in the subresource data.
			D3D11_SUBRESOURCE_DATA InitData;
			InitData.pSysMem = &mSunlightBufferData;
			InitData.SysMemPitch = 0;
			InitData.SysMemSlicePitch = 0;
			// Create the buffer.
			Toast::System::tSys->mD3D->mDevice->CreateBuffer(&cbDesc, &InitData, &mSunlightConstantBuffer);
		}

		d3d->mDeviceContext->Map(mSunlightConstantBuffer, NULL, D3D11_MAP_WRITE_DISCARD, NULL, &ms);
		memcpy(ms.pData, &mSunlightBufferData, sizeof(SunlightBufferData));
		d3d->mDeviceContext->Unmap(mSunlightConstantBuffer, NULL);

		d3d->SetSunlightConstantBuffer(mSunlightConstantBuffer);
	}

	void World::SetSunlightDirection(DirectX::XMFLOAT4 direction) 
	{
		mSunlightDirection = direction;
	}

	void World::SetSunlightColor(DirectX::XMFLOAT4 color)
	{
		mSunlightColor = color;
	}
}