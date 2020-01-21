#include "Object3D.h"

namespace Toast 
{
	Object3D::Object3D(bool targetable, bool alphaBlending)
	{
		mPosition = DirectX::XMFLOAT3(0.0f, 0.0f, 0.0f);
		mScale = DirectX::XMFLOAT3(1.0f, 1.0f, 1.0f);
		mRotation = DirectX::XMFLOAT3(0.0f, 0.0f, 0.0f);

		mBoundingOrientedBox = BoundingOrientedBox(XMFLOAT3(0.0f, 0.0f, 0.0f), XMFLOAT3(1.0f, 1.0f, 1.0f), XMFLOAT4(0.0f, 0.0f, 0.0f, 1.0f));

		mTargetable = targetable;
		mAlphaBlending = alphaBlending;
	}

	Object3D::~Object3D() 
	{
		if (mConstantBuffer) 
		{
			mConstantBuffer->Release();
			mConstantBuffer = 0;
		}
	}

	void Object3D::CreateTargetBox(float sizeFactor)
	{
		DirectX::XMFLOAT3 BOBCorners;
		float xSize, ySize, zSize;

		BOBCorners = Object3D::GetBoundingBoxSize();

		mBoundingOrientedBox = BoundingOrientedBox(XMFLOAT3(0.0f, 0.0f, 0.0f), BOBCorners, XMFLOAT4(0.0f, 0.0f, 0.0f, 1.0f));

		xSize = BOBCorners.x * sizeFactor;
		ySize = BOBCorners.y * sizeFactor;
		zSize = BOBCorners.z * sizeFactor;

		if (mMeshes.size() > 0) 
		{
			Mesh* mesh = new Mesh();
			Toast::System::tSys->mResources->mMeshes.push_back(mesh);

			mesh->mVertices.push_back(Toast::Vertex(DirectX::XMFLOAT3(-xSize, ySize, -zSize), DirectX::XMFLOAT3(1.0f, 0.0f, 0.0f), DirectX::XMFLOAT3(0.0f, 0.0f, 0.0f), DirectX::XMFLOAT3(0.0f, 0.0f, 0.0f), DirectX::XMFLOAT2(0.0f, 0.25f)));
			mesh->mVertices.push_back(Toast::Vertex(DirectX::XMFLOAT3(-xSize, -ySize, -zSize), DirectX::XMFLOAT3(1.0f, 0.0f, 0.0f), DirectX::XMFLOAT3(0.0f, 0.0f, 0.0f), DirectX::XMFLOAT3(0.0f, 0.0f, 0.0f), DirectX::XMFLOAT2(0.25f, 0.25f)));
			mesh->mVertices.push_back(Toast::Vertex(DirectX::XMFLOAT3(xSize, ySize, -zSize), DirectX::XMFLOAT3(1.0f, 0.0f, 0.0f), DirectX::XMFLOAT3(0.0f, 0.0f, 0.0f), DirectX::XMFLOAT3(0.0f, 0.0f, 0.0f), DirectX::XMFLOAT2(0.0f, 0.5f)));
			mesh->mVertices.push_back(Toast::Vertex(DirectX::XMFLOAT3(xSize, -ySize, -zSize), DirectX::XMFLOAT3(1.0f, 0.0f, 0.0f), DirectX::XMFLOAT3(0.0f, 0.0f, 0.0f), DirectX::XMFLOAT3(0.0f, 0.0f, 0.0f), DirectX::XMFLOAT2(0.25f, 0.5f)));
			mesh->mVertices.push_back(Toast::Vertex(DirectX::XMFLOAT3(-xSize, -ySize, zSize), DirectX::XMFLOAT3(1.0f, 0.0f, 0.0f), DirectX::XMFLOAT3(0.0f, 0.0f, 0.0f), DirectX::XMFLOAT3(0.0f, 0.0f, 0.0f), DirectX::XMFLOAT2(0.5f, 0.25f)));
			mesh->mVertices.push_back(Toast::Vertex(DirectX::XMFLOAT3(xSize, -ySize, zSize), DirectX::XMFLOAT3(1.0f, 0.0f, 0.0f), DirectX::XMFLOAT3(0.0f, 0.0f, 0.0f), DirectX::XMFLOAT3(0.0f, 0.0f, 0.0f), DirectX::XMFLOAT2(0.5f, 0.5f)));
			mesh->mVertices.push_back(Toast::Vertex(DirectX::XMFLOAT3(-xSize, ySize, zSize), DirectX::XMFLOAT3(1.0f, 0.0f, 0.0f), DirectX::XMFLOAT3(0.0f, 0.0f, 0.0f), DirectX::XMFLOAT3(0.0f, 0.0f, 0.0f), DirectX::XMFLOAT2(0.75f, 0.25f)));
			mesh->mVertices.push_back(Toast::Vertex(DirectX::XMFLOAT3(xSize, ySize, zSize), DirectX::XMFLOAT3(1.0f, 0.0f, 0.0f), DirectX::XMFLOAT3(0.0f, 0.0f, 0.0f), DirectX::XMFLOAT3(0.0f, 0.0f, 0.0f), DirectX::XMFLOAT2(0.75f, 0.5f)));
			mesh->mVertices.push_back(Toast::Vertex(DirectX::XMFLOAT3(-xSize, ySize, -zSize), DirectX::XMFLOAT3(1.0f, 0.0f, 0.0f), DirectX::XMFLOAT3(0.0f, 0.0f, 0.0f), DirectX::XMFLOAT3(0.0f, 0.0f, 0.0f), DirectX::XMFLOAT2(1.0f, 0.25f)));
			mesh->mVertices.push_back(Toast::Vertex(DirectX::XMFLOAT3(xSize, ySize, -zSize), DirectX::XMFLOAT3(1.0f, 0.0f, 0.0f), DirectX::XMFLOAT3(0.0f, 0.0f, 0.0f), DirectX::XMFLOAT3(0.0f, 0.0f, 0.0f), DirectX::XMFLOAT2(1.0f, 0.5f)));
			mesh->mVertices.push_back(Toast::Vertex(DirectX::XMFLOAT3(-xSize, ySize, -zSize), DirectX::XMFLOAT3(1.0f, 0.0f, 0.0f), DirectX::XMFLOAT3(0.0f, 0.0f, 0.0f), DirectX::XMFLOAT3(0.0f, 0.0f, 0.0f), DirectX::XMFLOAT2(0.25f, 0.0f)));
			mesh->mVertices.push_back(Toast::Vertex(DirectX::XMFLOAT3(-xSize, ySize, zSize), DirectX::XMFLOAT3(1.0f, 0.0f, 0.0f), DirectX::XMFLOAT3(0.0f, 0.0f, 0.0f), DirectX::XMFLOAT3(0.0f, 0.0f, 0.0f), DirectX::XMFLOAT2(0.5f, 0.0f)));
			mesh->mVertices.push_back(Toast::Vertex(DirectX::XMFLOAT3(xSize, ySize, -zSize), DirectX::XMFLOAT3(1.0f, 0.0f, 0.0f), DirectX::XMFLOAT3(0.0f, 0.0f, 0.0f), DirectX::XMFLOAT3(0.0f, 0.0f, 0.0f), DirectX::XMFLOAT2(0.25f, 0.75f)));
			mesh->mVertices.push_back(Toast::Vertex(DirectX::XMFLOAT3(xSize, ySize, zSize), DirectX::XMFLOAT3(1.0f, 0.0f, 0.0f), DirectX::XMFLOAT3(0.0f, 0.0f, 0.0f), DirectX::XMFLOAT3(0.0f, 0.0f, 0.0f), DirectX::XMFLOAT2(0.5f, 0.75f)));

			mesh->mIndices = std::vector<uint32_t>{
					0, 2, 1,
					1, 2, 3,
					4, 5, 6,
					5, 7, 6,
					6, 7, 8,
					7, 9, 8,
					1, 3, 4,
					3, 5, 4,
					1, 11, 10,
					1, 4, 11,
					3, 12, 5,
					5, 12, 13
			};


			mesh->mNumIndices = 36;
			mesh->mNumVertices = 14;
			mesh->ConstructVertexBuffer();
			mMeshes.push_back(mesh);
		}
	}

	void Object3D::CreateSphere(int levels, int cellLevels, bool isPlanet, bool frontCulling, float radius)
	{
		SphereMesh *sphereMesh = new SphereMesh();
		Toast::System::tSys->mResources->mSphereMeshes.push_back(sphereMesh);

		sphereMesh->mSphereMaxTriangleSize = 400.0f;
		sphereMesh->mCellLevels = cellLevels;
		sphereMesh->mSphereMaxLevels = levels;
		sphereMesh->mIsPlanet = isPlanet;
		sphereMesh->mRadius = radius;

		mFrontCulling = frontCulling;

		sphereMesh->CreateSphereMesh(radius);

		mSphereMeshes.push_back(sphereMesh);
	}

	void Object3D::Draw(D3D &d3d)
	{
		d3d.SetObjectConstantBuffer(mConstantBuffer);

		int endIndex = 0;

		if (mTargetable)
		{
			endIndex = 1; 
		}

		if (mTargeted) 
		{
			d3d.mDeviceContext->IASetInputLayout(this->mMeshes[1]->mMaterial->mInputLayout);
			mMeshes[1]->Update();
			mMeshes[1]->Draw(d3d);
		}

		for (int i = 0; i < endIndex; i++)
		{
			d3d.mDeviceContext->IASetInputLayout(this->mMeshes[i]->mMaterial->mInputLayout);
			mMeshes[i]->Update();
			mMeshes[i]->Draw(d3d);
		}

		for (int i = 0; i < mSphereMeshes.size(); i++)
		{
			d3d.mDeviceContext->IASetInputLayout(this->mSphereMeshes[i]->mMaterial->mInputLayout);
			mSphereMeshes[i]->Update();
			mSphereMeshes[i]->Draw(d3d);
		}
	}

	void Object3D::Update()
	{
		Toast::D3D* d3d = Toast::System::tSys->mD3D;
		DirectX::XMMATRIX worldMatrix = XMMatrixIdentity();

		if (mObjectPhysic != nullptr) 
		{
			DirectX::XMVECTOR transformVector = mObjectPhysic->GetPositionTransform();

			if (mObjectPhysic->mUseGravity) 
			{
				mObjectPhysic->mPosition = mPosition;
				mPosition = DirectX::XMFLOAT3(mPosition.x + DirectX::XMVectorGetX(transformVector), mPosition.y + DirectX::XMVectorGetY(transformVector), mPosition.z + DirectX::XMVectorGetZ(transformVector));
			}
		}

		worldMatrix = DirectX::XMMatrixMultiply(worldMatrix, DirectX::XMMatrixScalingFromVector(XMLoadFloat3(&mScale)));
		worldMatrix = DirectX::XMMatrixMultiply(worldMatrix, DirectX::XMMatrixRotationRollPitchYawFromVector(XMLoadFloat3(&mRotation)));
		worldMatrix = DirectX::XMMatrixMultiply(worldMatrix, DirectX::XMMatrixTranslationFromVector(XMLoadFloat3(&mPosition)));

		if (mTargetable)
		{
			DirectX::XMFLOAT3 BOBCorners = BOBCorners = Object3D::GetBoundingBoxSize();

			mBoundingOrientedBox = BoundingOrientedBox(XMFLOAT3(0.0f, 0.0f, 0.0f), BOBCorners, XMFLOAT4(0.0f, 0.0f, 0.0f, 1.0f));

			mBoundingOrientedBox.Transform(mBoundingOrientedBox, worldMatrix);
		}

		for (int i = 0; i < mSphereMeshes.size(); i++) 
		{
			mSphereMeshes[i]->mRotationMatrix = DirectX::XMMatrixRotationRollPitchYawFromVector(XMLoadFloat3(&mRotation));
		}

		mConstantBufferData.worldMatrix = worldMatrix;

		if (!mConstantBuffer)
		{
			// Fill in a buffer description.
			D3D11_BUFFER_DESC cbDesc;
			cbDesc.ByteWidth = sizeof(Object3DConstantBufferData);
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
		memcpy(ms.pData, &mConstantBufferData, sizeof(Object3DConstantBufferData));
		d3d->mDeviceContext->Unmap(mConstantBuffer, NULL);

		d3d->SetObjectConstantBuffer(mConstantBuffer);
	}

	DirectX::XMFLOAT3 Object3D::GetBoundingBoxSize()
	{
		float distX, distY, distZ;
		DirectX::XMFLOAT3 minVertex = DirectX::XMFLOAT3(0.0f, 0.0f, 0.0f);
		DirectX::XMFLOAT3 maxVertex = DirectX::XMFLOAT3(0.0f, 0.0f, 0.0f);

		for (int i = 0; i < mMeshes[0]->mNumVertices; i++)
		{
			minVertex.x = min(minVertex.x, mMeshes[0]->mVertices[i].position.x);
			minVertex.y = min(minVertex.y, mMeshes[0]->mVertices[i].position.y);
			minVertex.z = min(minVertex.z, mMeshes[0]->mVertices[i].position.z);

			maxVertex.x = max(maxVertex.x, mMeshes[0]->mVertices[i].position.x);
			maxVertex.y = max(maxVertex.y, mMeshes[0]->mVertices[i].position.y);
			maxVertex.z = max(maxVertex.z, mMeshes[0]->mVertices[i].position.z);
		}

		distX = (maxVertex.x - minVertex.x) / 2.0f;
		distY = (maxVertex.y - minVertex.y) / 2.0f;
		distZ = (maxVertex.z - minVertex.z) / 2.0f;

		return DirectX::XMFLOAT3(distX, distY, distZ);
	}

	std::vector<DirectX::XMFLOAT3> Object3D::GetBoundingBoxVertices() 
	{
		std::vector<DirectX::XMFLOAT3> vertices;
		DirectX::XMFLOAT3 corners[8];

		mBoundingOrientedBox.GetCorners(&corners[0]);

		for (int i = 0; i < 8; i++)
		{
			//Toast::System::tSys->Print("corners[%d] x: %f, y: %f, z: %f", i, corners[i].x, corners[i].y, corners[i].z);
			vertices.push_back(corners[i]);
		}

		return vertices;
	}
}