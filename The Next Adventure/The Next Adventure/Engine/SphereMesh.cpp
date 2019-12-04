#include "SphereMesh.h"

namespace Toast 
{
	SphereMesh::SphereMesh()
	{
	}

	SphereMesh::~SphereMesh() 
	{
		mVertexBuffer = nullptr;
		mIndexBuffer = nullptr;
		mInstanceBuffer = nullptr;

		if (mConstantBuffer)
		{
			mConstantBuffer->Release();
			mConstantBuffer = 0;
		}

		mMaterial = nullptr;
	}

	void SphereMesh::CreateSphereMesh(float radius)
	{
		mRotationMatrix = DirectX::XMMatrixIdentity();

		std::vector<DirectX::XMFLOAT3> icoPositions;
		std::vector<uint32_t> icoIndices;

		GenerateSphereCellGeometry(mCellLevels);

		float ratio = (float)((1.0f + sqrt(5.0f)) / 2.0f);
		float scale = radius / sqrt(ratio * ratio + 1.0f);

		ratio *= scale;

		icoPositions.push_back(DirectX::XMFLOAT3(ratio, 0.0f, -scale));
		icoPositions.push_back(DirectX::XMFLOAT3(-ratio, 0.0f, -scale));
		icoPositions.push_back(DirectX::XMFLOAT3(ratio, 0.0f, scale));
		icoPositions.push_back(DirectX::XMFLOAT3(-ratio, 0.0f, scale));

		icoPositions.push_back(DirectX::XMFLOAT3(0.0f, -scale, ratio));
		icoPositions.push_back(DirectX::XMFLOAT3(0.0f, -scale, -ratio));
		icoPositions.push_back(DirectX::XMFLOAT3(0.0f, scale, ratio));
		icoPositions.push_back(DirectX::XMFLOAT3(0.0f, scale, -ratio));

		icoPositions.push_back(DirectX::XMFLOAT3(-scale, ratio, 0.0f));
		icoPositions.push_back(DirectX::XMFLOAT3(-scale, -ratio, 0.0f));
		icoPositions.push_back(DirectX::XMFLOAT3(scale, ratio, 0.0f));
		icoPositions.push_back(DirectX::XMFLOAT3(scale, -ratio, 0.0f));

		icoIndices = std::vector<uint32_t>{
			1, 8, 3,
			1, 3, 9,
			0, 2, 10,
			0, 11, 2,

			5, 0, 7,
			5, 7, 1,
			4, 6, 2,
			4, 3, 6,

			9, 4, 11,
			9, 11, 5,
			8, 10, 6,
			8, 7, 10,

			1, 7, 8,
			1, 9, 5,
			0, 10, 7,
			0, 5, 11,

			3, 8, 6,
			3, 4, 9,
			2, 6, 10,
			2, 11, 4
		};

		for (size_t i = 0; i < icoIndices.size(); i += 3)
		{
			mIcosphere.push_back(SphereTriangle(icoPositions[icoIndices[i]], icoPositions[icoIndices[i + 1]], icoPositions[icoIndices[i + 2]], nullptr, (short)0));
		}

		// Uploading the Distance LUT is only needed for a planet sphere and not for a regular sphere
		if (mIsPlanet)
		{
			GenerateDistanceLUT();
		}

		// Creates the sphere cells for the first time, this method is normaly called from the update method.
		GenerateSphereCells();

		ConstructVertexBuffer();
	}

	void SphereMesh::UpdateConstantBuffer() 
	{
		Toast::D3D *d3d = Toast::System::tSys->mD3D;

		// Note for the future, this might only have to be uploaded once and no more! A future fix!
		for (int i = 0; i < mDistanceLUT.size(); i++)
		{
			mConstantBufferData.distanceLUT[i] = XMFLOAT4(mDistanceLUT[i], mDistanceLUT[i], mDistanceLUT[i], mDistanceLUT[i]);
		}

		mConstantBufferData.morphRange = DirectX::XMFLOAT4(0.5f, 0.5f, 0.5f, 0.5f);
		mConstantBufferData.rotationMatrix = mRotationMatrix;

		if (!mConstantBuffer)
		{
			// Fill in a buffer description.
			D3D11_BUFFER_DESC cbDesc;
			cbDesc.ByteWidth = sizeof(MorphConstantBufferData);
			cbDesc.Usage = D3D11_USAGE_DYNAMIC;
			cbDesc.BindFlags = D3D11_BIND_CONSTANT_BUFFER;
			cbDesc.CPUAccessFlags = D3D11_CPU_ACCESS_WRITE;
			cbDesc.MiscFlags = 0;
			cbDesc.StructureByteStride = 0;

			// Fill in the subresource data.
			D3D11_SUBRESOURCE_DATA InitData;
			InitData.pSysMem = &mConstantBufferData;
			InitData.SysMemPitch = 0;
			InitData.SysMemSlicePitch = 0;
			// Create the buffer.
			Toast::System::tSys->mD3D->mDevice->CreateBuffer(&cbDesc, &InitData, &mConstantBuffer);
		}

		D3D11_MAPPED_SUBRESOURCE ms;
		d3d->mDeviceContext->Map(mConstantBuffer, NULL, D3D11_MAP_WRITE_DISCARD, NULL, &ms);
		memcpy(ms.pData, &mConstantBufferData, sizeof(MorphConstantBufferData));
		d3d->mDeviceContext->Unmap(mConstantBuffer, NULL);

		d3d->SetMorphConstantBuffer(mConstantBuffer);
	}

	void SphereMesh::Update() 
	{
		// If the mesh is a planet the cells is regenerated here, this is due to a planet sphere using frustum culling compared to a normal sphere
		if (mIsPlanet) 
		{
			GenerateSphereCells();
		}

		//Remaps the instance buffer
		Toast::Resources::sResources->RemapInstanceBuffer(mSphereCells, iInstanceBuffer);

		UpdateConstantBuffer();
	}

	void SphereMesh::GenerateSphereCellGeometry(int maxPlanetCellLevel)
	{
		int mRC = 1 + (int)pow(2, maxPlanetCellLevel);

		mSphereCellPatchDelta = 1 / (float)(mRC - 1);

		int rowIndex = 0;
		int nextIndex = 0;

		for (int row = 0; row < mRC; row++)
		{
			int numCols = mRC - row;

			nextIndex += numCols;

			for (int column = 0; column < numCols; column++)
			{
				//calc position
				DirectX::XMFLOAT2 pos = DirectX::XMFLOAT2(column / (float)(mRC - 1), row / (float)(mRC - 1));

				//calc morph factor
				DirectX::XMFLOAT2 morph = DirectX::XMFLOAT2(0.0f, 0.0f);
				if (row % 2 == 0)
				{
					if (column % 2 == 1)
					{
						morph = DirectX::XMFLOAT2(-mSphereCellPatchDelta, 0);
					}
				}
				else
				{
					if (column % 2 == 0)
					{
						morph = DirectX::XMFLOAT2(0, mSphereCellPatchDelta);
					}
					else
					{
						morph = DirectX::XMFLOAT2(mSphereCellPatchDelta, -mSphereCellPatchDelta);
					}
				}

				//create vertex
				mSphereCellVertices.push_back(SphereVertex(pos, morph));

				//calc index
				if (row < mRC - 1 && column < numCols - 1)
				{
					mSphereCellIndices.push_back(rowIndex + column);
					mSphereCellIndices.push_back(nextIndex + column);
					mSphereCellIndices.push_back(1 + rowIndex + column);

					if (column < numCols - 2)
					{
						mSphereCellIndices.push_back(nextIndex + column);
						mSphereCellIndices.push_back(1 + nextIndex + column);
						mSphereCellIndices.push_back(1 + rowIndex + column);
					}
				}
			}

			rowIndex = nextIndex;
		}
	}

	void SphereMesh::RecursiveTriangle(DirectX::XMFLOAT3 a, DirectX::XMFLOAT3 b, DirectX::XMFLOAT3 c, short level)
	{
		DirectX::XMFLOAT3 A, B, C;
		Toast::NextSphereTriangle nextSphereTriangle;

		if (mIsPlanet) 
		{
			nextSphereTriangle = CheckSphereTriangleSplit(a, b, c, level);
		}

		if ((level < mSphereMaxLevels && !mIsPlanet) || (mIsPlanet && (level < mSphereMaxLevels) && (nextSphereTriangle == Toast::NextSphereTriangle::SPLIT || nextSphereTriangle == Toast::NextSphereTriangle::SPLITCULL))) {
			short nLevel;

			float lengthA;
			float lengthB;
			float lengthC;

			DirectX::XMStoreFloat3(&A, DirectX::XMLoadFloat3(&b) + DirectX::XMVectorScale((XMLoadFloat3(&c) - DirectX::XMLoadFloat3(&b)), 0.5f));
			DirectX::XMStoreFloat3(&B, DirectX::XMLoadFloat3(&c) + DirectX::XMVectorScale((XMLoadFloat3(&a) - DirectX::XMLoadFloat3(&c)), 0.5f));
			DirectX::XMStoreFloat3(&C, DirectX::XMLoadFloat3(&a) + DirectX::XMVectorScale((XMLoadFloat3(&b) - DirectX::XMLoadFloat3(&a)), 0.5f));

			lengthA = DirectX::XMVectorGetX(DirectX::XMVector3Length(DirectX::XMLoadFloat3(&A)));
			lengthB = DirectX::XMVectorGetX(DirectX::XMVector3Length(DirectX::XMLoadFloat3(&B)));
			lengthC = DirectX::XMVectorGetX(DirectX::XMVector3Length(DirectX::XMLoadFloat3(&C)));

			DirectX::XMStoreFloat3(&A, (DirectX::XMLoadFloat3(&A) * (mRadius / lengthA)));
			DirectX::XMStoreFloat3(&B, (DirectX::XMLoadFloat3(&B) * (mRadius / lengthB)));
			DirectX::XMStoreFloat3(&C, (DirectX::XMLoadFloat3(&C) * (mRadius / lengthC)));

			nLevel = level + 1;

			RecursiveTriangle(C, B, a, nLevel);
			RecursiveTriangle(b, A, C, nLevel);
			RecursiveTriangle(B, A, c, nLevel);
			RecursiveTriangle(A, B, C, nLevel);
		}
		else
		{
			DirectX::XMFLOAT3 secondCorner;
			DirectX::XMFLOAT3 thirdCorner;

			DirectX::XMStoreFloat3(&secondCorner, XMLoadFloat3(&b) - XMLoadFloat3(&a));
			DirectX::XMStoreFloat3(&thirdCorner, XMLoadFloat3(&c) - XMLoadFloat3(&a));

			mSphereCells.push_back(SphereCell(level, a, secondCorner, thirdCorner));
		}
	}

	Toast::NextSphereTriangle SphereMesh::CheckSphereTriangleSplit(DirectX::XMFLOAT3 a, DirectX::XMFLOAT3 b, DirectX::XMFLOAT3 c, short level)
	{
		DirectX::XMFLOAT3 cameraPos;
		float aDistance, bDistance, cDistance;

		if (level > mSphereMaxLevels)
		{
			return Toast::NextSphereTriangle::LEAF;
		}

		// Check distance between triangle vertex and the position of the camera
		if (Toast::System::tSys->mTheNextFrontier->mCamera) 
		{
			cameraPos = Toast::System::tSys->mTheNextFrontier->mCamera->GetPosition();

			XMStoreFloat3(&a, XMVector3Transform(XMLoadFloat3(&a), mRotationMatrix));
			XMStoreFloat3(&b, XMVector3Transform(XMLoadFloat3(&b), mRotationMatrix));
			XMStoreFloat3(&c, XMVector3Transform(XMLoadFloat3(&c), mRotationMatrix));

			aDistance = GetVectorDistance(a, cameraPos);
			bDistance = GetVectorDistance(b, cameraPos);
			cDistance = GetVectorDistance(c, cameraPos);
			//Toast::System::tSys->Print("aDistance: %f", aDistance);
			//Toast::System::tSys->Print("bDistance: %f", bDistance);
			//Toast::System::tSys->Print("cDistance: %f", cDistance);

			if (fminf(aDistance, fminf(bDistance, cDistance)) < mDistanceLUT[level])
			{
				//Toast::System::tSys->Print("SPLITCULL, level: %i", level);

				return Toast::NextSphereTriangle::SPLITCULL;
			}
		}
		else 
		{
			Toast::System::tSys->Print("Your game doesn't have a camera and therefor a position can not be determined by the engine. \nThe Sphere Mesh will be rendered without LoD zones");

			return Toast::NextSphereTriangle::LEAF;
		}

		return Toast::NextSphereTriangle::LEAF;
	}

	void SphereMesh::GenerateDistanceLUT()
	{
		float vectorDistance, frac;

		mDistanceLUT.clear();

		vectorDistance = GetVectorDistance(mIcosphere[0].a, mIcosphere[0].b);
		frac = tanf((mSphereMaxTriangleSize * (45.0f * (DirectX::XM_PI / 180.0f))) / Toast::System::tSys->mSettings.find("WindowSizeX")->second);

		for (int level = 0; level < mSphereMaxLevels; level++)
		{
			mDistanceLUT.push_back(vectorDistance / frac);
			vectorDistance *= 0.5f;
		}
	}

	void SphereMesh::ConstructVertexBuffer()
	{
		iVertexBuffer = Toast::Resources::sResources->CreateVertexBuffer(mSphereCellVertices);
		iIndexBuffer = Toast::Resources::sResources->CreateIndexBuffer(mSphereCellIndices);
		iInstanceBuffer = Toast::Resources::sResources->CreateInstanceBuffer(mSphereCells, 20000); // 20000 is a random number for now

		mVertexBuffer = Toast::Resources::sResources->mVertexBuffers[iVertexBuffer];
		mIndexBuffer = Toast::Resources::sResources->mIndexBuffers[iIndexBuffer];
		mInstanceBuffer = Toast::Resources::sResources->mInstanceBuffers[iInstanceBuffer];
	}

	void SphereMesh::GenerateSphereCells()
	{
		mSphereCells.clear();

		for (auto triangle : mIcosphere)
		{
			RecursiveTriangle(triangle.a, triangle.b, triangle.c, triangle.level);
		}
	}

	void SphereMesh::Draw(D3D &d3d)
	{
		uint32_t stride[] = { sizeof(SphereVertex),	sizeof(SphereCell) };
		uint32_t offset[] = { 0, 0 };
		ID3D11Buffer* bufferPointers[2];

		d3d.UploadConstantBuffersToGPU();

		bufferPointers[0] = mVertexBuffer;
		bufferPointers[1] = mInstanceBuffer;

		d3d.mDeviceContext->IASetVertexBuffers(0, 2, bufferPointers, stride, offset);
		d3d.mDeviceContext->IASetIndexBuffer(mIndexBuffer, DXGI_FORMAT_R32_UINT, 0);

		d3d.mDeviceContext->PSSetSamplers(0, 1, &mMaterial->mTexture->mSamplerState);
		d3d.mDeviceContext->PSSetShaderResources(0, 1, &mMaterial->mTexture->mView);

		if (mIsPlanet)
		{
			d3d.mDeviceContext->VSSetSamplers(0, 1, &mMaterial->mTexture->mSamplerState);
			d3d.mDeviceContext->VSSetShaderResources(0, 1, &mMaterial->mTerrainMap->mView);
		}

		d3d.mDeviceContext->VSSetShader(mMaterial->mVertexShader, 0, 0);
		d3d.mDeviceContext->PSSetShader(mMaterial->mPixelShader, 0, 0);

		d3d.mDeviceContext->DrawIndexedInstanced(static_cast<UINT>(mSphereCellIndices.size()), static_cast<UINT>(mSphereCells.size()), 0, 0, 0);
	}
}