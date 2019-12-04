#pragma once

#include "Mesh.h"
#include "Resources.h"
#include "D3D.h"
#include "Material.h"

namespace Toast
{
	Mesh::Mesh() 
	{
	}

	Mesh::~Mesh() 
	{
		mVertexBuffer = nullptr;
		mIndexBuffer = nullptr;

		mMaterial = nullptr;
	}

	void Mesh::ConstructVertexBuffer()
	{
		iVertexBuffer = Toast::Resources::sResources->CreateVertexBuffer(mVertices, mNumVertices);
		iIndexBuffer = Toast::Resources::sResources->CreateIndexBuffer(mIndices, mNumIndices);

		mVertexBuffer = Toast::Resources::sResources->mVertexBuffers[iVertexBuffer];
		mIndexBuffer = Toast::Resources::sResources->mIndexBuffers[iIndexBuffer];
	}

	void Mesh::Update()
	{
	}

	void Mesh::Draw(D3D &d3d)
	{
		uint32_t stride[] = { sizeof(Vertex) };
		uint32_t offset[] = { 0 };

		d3d.UploadConstantBuffersToGPU();

		d3d.mDeviceContext->IASetVertexBuffers(0, 1, &mVertexBuffer, stride, offset);
		d3d.mDeviceContext->IASetIndexBuffer(mIndexBuffer, DXGI_FORMAT_R32_UINT, 0);

		d3d.mDeviceContext->PSSetSamplers(0, 1, &mMaterial->mTexture->mSamplerState);
		d3d.mDeviceContext->PSSetShaderResources(0, 1, &mMaterial->mTexture->mView);
		d3d.mDeviceContext->VSSetShader(mMaterial->mVertexShader, 0, 0);
		d3d.mDeviceContext->PSSetShader(mMaterial->mPixelShader, 0, 0);

		d3d.mDeviceContext->DrawIndexed(mNumIndices, 0, 0);
	}
}