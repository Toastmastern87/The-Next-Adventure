#include "Object3D.h"
#include "Mesh.h"
#include "SphereMesh.h"

namespace Toast 
{
	Object3D::Object3D() 
	{
		mPosition = DirectX::XMFLOAT3(0.0f, 0.0f, 0.0f);
		mScale = DirectX::XMFLOAT3(1.0f, 1.0f, 1.0f);
		mRotation = DirectX::XMFLOAT3(0.0f, 0.0f, 0.0f);
	}

	Object3D::~Object3D() 
	{
		if (mConstantBuffer) 
		{
			mConstantBuffer->Release();
			mConstantBuffer = 0;
		}
	}

	void Object3D::CreateCube() 
	{
		Mesh *mesh = new Mesh();
		Toast::System::tSys->mResources->mMeshes.push_back(mesh);

		mesh->mVertices.push_back(Toast::Vertex(DirectX::XMFLOAT3(-0.5f, 0.5f, -0.5f), DirectX::XMFLOAT3(1.0f, 0.0f, 0.0f), DirectX::XMFLOAT3(0.0f, 0.0f, 0.0f), DirectX::XMFLOAT3(0.0f, 0.0f, 0.0f), DirectX::XMFLOAT2(1.0f, 0.0f)));
		mesh->mVertices.push_back(Toast::Vertex(DirectX::XMFLOAT3(0.5f, 0.5f, -0.5f), DirectX::XMFLOAT3(0.0f, 0.0f, 1.0f), DirectX::XMFLOAT3(0.0f, 0.0f, 0.0f), DirectX::XMFLOAT3(0.0f, 0.0f, 0.0f), DirectX::XMFLOAT2(0.0f, 1.0f)));
		mesh->mVertices.push_back(Toast::Vertex(DirectX::XMFLOAT3(0.5f, 0.5f, 0.5f), DirectX::XMFLOAT3(0.0f, 0.0f, 1.0f), DirectX::XMFLOAT3(0.0f, 0.0f, 0.0f), DirectX::XMFLOAT3(0.0f, 0.0f, 0.0f), DirectX::XMFLOAT2(0.0f, 0.0f)));
		mesh->mVertices.push_back(Toast::Vertex(DirectX::XMFLOAT3(-0.5f, 0.5f, 0.5f), DirectX::XMFLOAT3(1.0f, 1.0f, 0.0f), DirectX::XMFLOAT3(0.0f, 0.0f, 0.0f), DirectX::XMFLOAT3(0.0f, 0.0f, 0.0f), DirectX::XMFLOAT2(1.0f, 1.0f)));
		mesh->mVertices.push_back(Toast::Vertex(DirectX::XMFLOAT3(-0.5f, -0.5f, 0.5f), DirectX::XMFLOAT3(0.0f, 1.0f, 1.0f), DirectX::XMFLOAT3(0.0f, 0.0f, 0.0f), DirectX::XMFLOAT3(0.0f, 0.0f, 0.0f), DirectX::XMFLOAT2(1.0f, 0.0f)));
		mesh->mVertices.push_back(Toast::Vertex(DirectX::XMFLOAT3(0.5f, -0.5f, 0.5f), DirectX::XMFLOAT3(1.0f, 1.0f, 1.0f), DirectX::XMFLOAT3(0.0f, 0.0f, 0.0f), DirectX::XMFLOAT3(0.0f, 0.0f, 0.0f), DirectX::XMFLOAT2(0.0f, 1.0f)));
		mesh->mVertices.push_back(Toast::Vertex(DirectX::XMFLOAT3(0.5f, -0.5f, -0.5f), DirectX::XMFLOAT3(1.0f, 0.0f, 1.0f), DirectX::XMFLOAT3(0.0f, 0.0f, 0.0f), DirectX::XMFLOAT3(0.0f, 0.0f, 0.0f), DirectX::XMFLOAT2(0.0f, 0.0f)));
		mesh->mVertices.push_back(Toast::Vertex(DirectX::XMFLOAT3(-0.5f, -0.5f, -0.5f), DirectX::XMFLOAT3(1.0f, 0.0f, 0.0f), DirectX::XMFLOAT3(0.0f, 0.0f, 0.0f), DirectX::XMFLOAT3(0.0f, 0.0f, 0.0f), DirectX::XMFLOAT2(1.0f, 1.0f)));

		mesh->mIndices = std::vector<uint32_t>{
			0, 1, 2,
			0, 2, 3,
			4, 5, 6,
			4, 6, 7,
			3, 2, 5,
			3, 5, 4,
			2, 1, 6,
			2, 6, 5,
			1, 7, 6,
			1, 0, 7,
			0, 3, 4,
			0, 4, 7
		};

		mesh->mNumIndices = 36;
		mesh->mNumVertices = 8;
		mesh->ConstructVertexBuffer();
		mMeshes.push_back(mesh);
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

		for (int i = 0; i < mMeshes.size(); i++)
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
		Toast::D3D *d3d = Toast::System::tSys->mD3D;
		DirectX::XMMATRIX worldMatrix = XMMatrixIdentity();

		worldMatrix = DirectX::XMMatrixMultiply(worldMatrix, DirectX::XMMatrixScalingFromVector(XMLoadFloat3(&mScale)));
		worldMatrix = DirectX::XMMatrixMultiply(worldMatrix, DirectX::XMMatrixRotationRollPitchYawFromVector(XMLoadFloat3(&mRotation)));
		worldMatrix = DirectX::XMMatrixMultiply(worldMatrix, DirectX::XMMatrixTranslationFromVector(XMLoadFloat3(&mPosition)));

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
}