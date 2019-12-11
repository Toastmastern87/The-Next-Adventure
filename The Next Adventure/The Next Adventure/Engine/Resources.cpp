#pragma once

#include "System.h"
#include "Object3D.h"
#include "Resources.h"
#include "Mesh.h"
#include "SphereMesh.h"
#include "System.h"
#include "Material.h"
#include "GUI.h"
#include "GUIPanel.h"
#include "GUIText.h"

namespace Toast
{
	Resources* Resources::sResources = nullptr;

	Resources::Resources()
	{
		Resources::sResources = this;

		mVertexBuffers.push_back(0);
		mIndexBuffers.push_back(0);
		mInstanceBuffers.push_back(0);
	}

	Resources::~Resources() 
	{
		for (auto i : mVertexShaders)
		{
			i.second->Release();
			i.second = nullptr;
		}
		mVertexShaders.clear();

		for (auto i : mPixelShaders)
		{
			i.second->Release();
			i.second = nullptr;
		}
		mPixelShaders.clear();

		for (auto i : mTextures)
		{
			delete i.second;
			i.second = nullptr;
		}
		mTextures.clear();

		for (auto i : mMaterials)
		{
			delete i.second;
			i.second = nullptr;
		}
		mMaterials.clear();

		for (auto i : mVertexBuffers)
		{
			if (i == 0)continue;
			i->Release();
			i = nullptr;
		}
		mVertexBuffers.clear();

		for (auto i : mIndexBuffers)
		{
			if (i == 0)continue;
			i->Release();
			i = nullptr;
		}
		mIndexBuffers.clear();

		for (auto i : mInstanceBuffers)
		{
			if (i == 0)continue;
			i->Release();
			i = nullptr;
		}
		mInstanceBuffers.clear();

		for (auto i : mMeshes)
		{
			delete i;
			i = nullptr;
		}
		mMeshes.clear();

		for (auto i : mSphereMeshes)
		{
			delete i;
			i = nullptr;
		}
		mSphereMeshes.clear();

		for (auto i : mGUIPanels)
		{
			delete i;
			i = nullptr;
		}
		mGUIPanels.clear();

		for (auto i : mFonts)
		{
			delete i.second;
			i.second = nullptr;
		}
		mFonts.clear();

		Resources::sResources = nullptr;
	}

	size_t Resources::CreateVertexBuffer(std::vector<Vertex> vertices, size_t numVertices)
	{
		Toast::D3D *d3d = Toast::System::tSys->mD3D;
		D3D11_BUFFER_DESC vbd;
		D3D11_MAPPED_SUBRESOURCE ms;
		ID3D11Buffer *vertexBuffer;
		HRESULT hResult;

		ZeroMemory(&vbd, sizeof(D3D11_BUFFER_DESC));
		ZeroMemory(&ms, sizeof(D3D11_MAPPED_SUBRESOURCE));

		vbd.Usage = D3D11_USAGE_DYNAMIC;
		vbd.ByteWidth = sizeof(Vertex) * static_cast<UINT>(numVertices);
		vbd.BindFlags = D3D11_BIND_VERTEX_BUFFER;
		vbd.CPUAccessFlags = D3D11_CPU_ACCESS_WRITE;
		vbd.MiscFlags = 0;
		vbd.StructureByteStride = 0;

		hResult = d3d->mDevice->CreateBuffer(&vbd, NULL, &vertexBuffer);
		if (FAILED(hResult))
		{
			Toast::System::tSys->Print("Error creating Vertex buffer \n");

			return 0;
		}

		d3d->mDeviceContext->Map(vertexBuffer, NULL, D3D11_MAP_WRITE_DISCARD, NULL, &ms);
		memcpy(ms.pData, &vertices[0], sizeof(Vertex) * numVertices);
		d3d->mDeviceContext->Unmap(vertexBuffer, NULL);

		mVertexBuffers.push_back(vertexBuffer);

		return mVertexBuffers.size() - 1;
	}

	size_t Resources::CreateVertexBuffer(std::vector<SphereVertex> vertices)
	{
		Toast::D3D *d3d = Toast::System::tSys->mD3D;
		D3D11_BUFFER_DESC vbd;
		D3D11_MAPPED_SUBRESOURCE ms;
		ID3D11Buffer *vertexBuffer;
		HRESULT hResult;

		ZeroMemory(&vbd, sizeof(D3D11_BUFFER_DESC));
		ZeroMemory(&ms, sizeof(D3D11_MAPPED_SUBRESOURCE));

		vbd.Usage = D3D11_USAGE_DYNAMIC;
		vbd.ByteWidth = sizeof(SphereVertex) * static_cast<UINT>(vertices.size());
		vbd.BindFlags = D3D11_BIND_VERTEX_BUFFER;
		vbd.CPUAccessFlags = D3D11_CPU_ACCESS_WRITE;
		vbd.MiscFlags = 0;
		vbd.StructureByteStride = 0;

		hResult = d3d->mDevice->CreateBuffer(&vbd, NULL, &vertexBuffer);
		if (FAILED(hResult))
		{
			Toast::System::tSys->Print("Error creating Vertex buffer \n");

			return 0;
		}

		d3d->mDeviceContext->Map(vertexBuffer, NULL, D3D11_MAP_WRITE_DISCARD, NULL, &ms);
		memcpy(ms.pData, &vertices[0], sizeof(SphereVertex) * static_cast<UINT>(vertices.size()));
		d3d->mDeviceContext->Unmap(vertexBuffer, NULL);

		mVertexBuffers.push_back(vertexBuffer);

		return mVertexBuffers.size() - 1;
	}

	size_t Resources::CreateVertexBuffer(std::vector<GUIVertex> vertices)
	{
		Toast::D3D *d3d = Toast::System::tSys->mD3D;
		D3D11_BUFFER_DESC vbd;
		D3D11_MAPPED_SUBRESOURCE ms;
		ID3D11Buffer *vertexBuffer;
		HRESULT hResult;

		ZeroMemory(&vbd, sizeof(D3D11_BUFFER_DESC));
		ZeroMemory(&ms, sizeof(D3D11_MAPPED_SUBRESOURCE));

		vbd.Usage = D3D11_USAGE_DYNAMIC;
		vbd.ByteWidth = sizeof(GUIVertex) * static_cast<UINT>(vertices.size());
		vbd.BindFlags = D3D11_BIND_VERTEX_BUFFER;
		vbd.CPUAccessFlags = D3D11_CPU_ACCESS_WRITE;
		vbd.MiscFlags = 0;
		vbd.StructureByteStride = 0;

		hResult = d3d->mDevice->CreateBuffer(&vbd, NULL, &vertexBuffer);
		if (FAILED(hResult))
		{
			Toast::System::tSys->Print("Error creating Vertex buffer \n");

			return 0;
		}

		d3d->mDeviceContext->Map(vertexBuffer, NULL, D3D11_MAP_WRITE_DISCARD, NULL, &ms);
		memcpy(ms.pData, &vertices[0], sizeof(GUIVertex) * static_cast<UINT>(vertices.size()));
		d3d->mDeviceContext->Unmap(vertexBuffer, NULL);

		mVertexBuffers.push_back(vertexBuffer);

		return mVertexBuffers.size() - 1;
	}

	size_t Resources::CreateVertexBuffer(std::vector<GUIVertex> vertices, size_t numVertices)
	{
		Toast::D3D *d3d = Toast::System::tSys->mD3D;
		D3D11_BUFFER_DESC vbd;
		D3D11_MAPPED_SUBRESOURCE ms;
		ID3D11Buffer *vertexBuffer;
		HRESULT hResult;

		ZeroMemory(&vbd, sizeof(D3D11_BUFFER_DESC));
		ZeroMemory(&ms, sizeof(D3D11_MAPPED_SUBRESOURCE));

		vbd.Usage = D3D11_USAGE_DYNAMIC;
		vbd.ByteWidth = sizeof(GUIVertex) * static_cast<UINT>(numVertices);
		vbd.BindFlags = D3D11_BIND_VERTEX_BUFFER;
		vbd.CPUAccessFlags = D3D11_CPU_ACCESS_WRITE;
		vbd.MiscFlags = 0;
		vbd.StructureByteStride = 0;

		hResult = d3d->mDevice->CreateBuffer(&vbd, NULL, &vertexBuffer);
		if (FAILED(hResult))
		{
			Toast::System::tSys->Print("Error creating Vertex buffer \n");

			return 0;
		}

		d3d->mDeviceContext->Map(vertexBuffer, NULL, D3D11_MAP_WRITE_DISCARD, NULL, &ms);
		memcpy(ms.pData, &vertices[0], sizeof(GUIVertex) * static_cast<UINT>(vertices.size()));
		d3d->mDeviceContext->Unmap(vertexBuffer, NULL);

		mVertexBuffers.push_back(vertexBuffer);

		return mVertexBuffers.size() - 1;
	}

	void Resources::RemapVertexBuffer(std::vector<GUIVertex> vertices, size_t bufferID)
	{
		Toast::D3D *d3d = Toast::System::tSys->mD3D;
		D3D11_MAPPED_SUBRESOURCE ms;

		d3d->mDeviceContext->Map(mVertexBuffers[bufferID], NULL, D3D11_MAP_WRITE_DISCARD, NULL, &ms);
		memcpy(ms.pData, &vertices[0], sizeof(GUIVertex) * static_cast<UINT>(vertices.size()));
		d3d->mDeviceContext->Unmap(mVertexBuffers[bufferID], NULL);
	}

	size_t Resources::CreateIndexBuffer(std::vector<uint32_t> indices, size_t numIndices)
	{
		Toast::D3D *d3d = Toast::System::tSys->mD3D;
		D3D11_BUFFER_DESC ibd;
		D3D11_MAPPED_SUBRESOURCE ms;
		ID3D11Buffer *indexBuffer;
		HRESULT hResult;

		ZeroMemory(&ibd, sizeof(D3D11_BUFFER_DESC));
		ZeroMemory(&ms, sizeof(D3D11_MAPPED_SUBRESOURCE));

		ibd.Usage = D3D11_USAGE_DYNAMIC;
		ibd.ByteWidth = sizeof(uint32_t) * static_cast<UINT>(numIndices);
		ibd.BindFlags = D3D11_BIND_INDEX_BUFFER;
		ibd.CPUAccessFlags = D3D11_CPU_ACCESS_WRITE;
		ibd.MiscFlags = 0;
		ibd.StructureByteStride = 0;

		hResult = d3d->mDevice->CreateBuffer(&ibd, NULL, &indexBuffer);
		if (FAILED(hResult))
		{
			Toast::System::tSys->Print("Error creating index buffer \n");

			return 0;
		}

		d3d->mDeviceContext->Map(indexBuffer, NULL, D3D11_MAP_WRITE_DISCARD, NULL, &ms);
		memcpy(ms.pData, &indices[0], sizeof(uint32_t) * numIndices);
		d3d->mDeviceContext->Unmap(indexBuffer, NULL);

		mIndexBuffers.push_back(indexBuffer);

		return mIndexBuffers.size() - 1;
	}

	size_t Resources::CreateIndexBuffer(std::vector<uint32_t> indices)
	{
		Toast::D3D *d3d = Toast::System::tSys->mD3D;
		D3D11_BUFFER_DESC ibd;
		D3D11_MAPPED_SUBRESOURCE ms;
		ID3D11Buffer *indexBuffer;
		HRESULT hResult;

		ZeroMemory(&ibd, sizeof(D3D11_BUFFER_DESC));
		ZeroMemory(&ms, sizeof(D3D11_MAPPED_SUBRESOURCE));

		ibd.Usage = D3D11_USAGE_DYNAMIC;
		ibd.ByteWidth = sizeof(uint32_t) * static_cast<UINT>(indices.size());
		ibd.BindFlags = D3D11_BIND_INDEX_BUFFER;
		ibd.CPUAccessFlags = D3D11_CPU_ACCESS_WRITE;
		ibd.MiscFlags = 0;
		ibd.StructureByteStride = 0;

		hResult = d3d->mDevice->CreateBuffer(&ibd, NULL, &indexBuffer);
		if (FAILED(hResult))
		{
			Toast::System::tSys->Print("Error creating index buffer \n");

			return 0;
		}

		d3d->mDeviceContext->Map(indexBuffer, NULL, D3D11_MAP_WRITE_DISCARD, NULL, &ms);
		memcpy(ms.pData, &indices[0], sizeof(uint32_t) * static_cast<UINT>(indices.size()));
		d3d->mDeviceContext->Unmap(indexBuffer, NULL);

		mIndexBuffers.push_back(indexBuffer);

		return mIndexBuffers.size() - 1;
	}

	void Resources::RemapIndexBuffer(std::vector<uint32_t> indices, size_t indicesID)
	{
		Toast::D3D *d3d = Toast::System::tSys->mD3D;
		D3D11_MAPPED_SUBRESOURCE ms;

		d3d->mDeviceContext->Map(mIndexBuffers[indicesID], NULL, D3D11_MAP_WRITE_DISCARD, NULL, &ms);
		memcpy(ms.pData, &indices[0], sizeof(uint32_t) * indices.size());
		d3d->mDeviceContext->Unmap(mIndexBuffers[indicesID], NULL);
	}

	size_t Resources::CreateInstanceBuffer(std::vector<SphereCell> instances, size_t numInstances)
	{
		Toast::D3D *d3d = Toast::System::tSys->mD3D;
		D3D11_BUFFER_DESC ibd;
		D3D11_MAPPED_SUBRESOURCE ms;
		ID3D11Buffer *instanceBuffer;
		HRESULT hResult;

		ZeroMemory(&ibd, sizeof(D3D11_BUFFER_DESC));
		ZeroMemory(&ms, sizeof(D3D11_MAPPED_SUBRESOURCE));

		ibd.Usage = D3D11_USAGE_DYNAMIC;
		ibd.ByteWidth = sizeof(SphereCell) * static_cast<UINT>(numInstances);
		ibd.BindFlags = D3D11_BIND_VERTEX_BUFFER;
		ibd.CPUAccessFlags = D3D11_CPU_ACCESS_WRITE;
		ibd.MiscFlags = 0;
		ibd.StructureByteStride = 0;

		hResult = d3d->mDevice->CreateBuffer(&ibd, NULL, &instanceBuffer);
		if (FAILED(hResult))
		{
			Toast::System::tSys->Print("Error creating instance buffer \n");

			return 0;
		}

		d3d->mDeviceContext->Map(instanceBuffer, NULL, D3D11_MAP_WRITE_DISCARD, NULL, &ms);
		memcpy(ms.pData, &instances[0], sizeof(SphereCell) * instances.size());
		d3d->mDeviceContext->Unmap(instanceBuffer, NULL);

		mInstanceBuffers.push_back(instanceBuffer);

		return mInstanceBuffers.size() - 1;
	}

	void Resources::RemapInstanceBuffer(std::vector<SphereCell> instances, size_t instanceID)
	{
		Toast::D3D *d3d = Toast::System::tSys->mD3D;
		D3D11_MAPPED_SUBRESOURCE ms;

		d3d->mDeviceContext->Map(mInstanceBuffers[instanceID], NULL, D3D11_MAP_WRITE_DISCARD, NULL, &ms);
		memcpy(ms.pData, &instances[0], sizeof(SphereCell) * instances.size());
		d3d->mDeviceContext->Unmap(mInstanceBuffers[instanceID], NULL);
	}

	Toast::Material* Resources::GetMaterial(std::string materialName, std::string texture, std::string shaderPath, D3D11_INPUT_ELEMENT_DESC *nonDefaultIED, int numElements, bool instanced)
	{
		// Check if the material already exists among the resources
		std::unordered_map<std::string, Toast::Material*>::iterator s = mMaterials.find(materialName);
		if (s != mMaterials.end()) 
		{
			return s->second;
		} 

		// If it doesn't exists it loads it and then returns it
		return LoadMaterial(materialName, texture, shaderPath, nonDefaultIED, numElements);
	}

	Toast::Material* Resources::LoadMaterial(std::string materialName, std::string texture, std::string shaderPath, D3D11_INPUT_ELEMENT_DESC *nonDefaultIED, int numElements, bool instanced)
	{
		HRESULT hResult;
		Toast::Material *material = new Toast::Material();

		std::string tempPath = shaderPath;

		tempPath.append("_vs.cso");

		// Creates the vertex shader for the material
		material->mVertexShader = Resources::sResources->GetVertexShader(tempPath);

		// Creates the input layout for the material
		std::string blobPath = BASE_SHADER_PATH;
		blobPath.append(tempPath);
		std::wstring stemp = std::wstring(blobPath.begin(), blobPath.end());
		ID3D10Blob* vsShaderBlob;

		hResult = D3DReadFileToBlob(stemp.c_str(), &vsShaderBlob);
		if (FAILED(hResult))
		{
			Toast::System::tSys->Print("Couldn't load the vertex shader file : %s \n", blobPath.c_str());

			return nullptr;
		}

		// Check if the material should use the default input layer or use the one being sent as a parameter
		if (nonDefaultIED == nullptr)
		{
			hResult = Toast::System::tSys->mD3D->mDevice->CreateInputLayout(Toast::D3D::sDefaultInputLayoutDesc, Toast::D3D::sDefaultInputLayoutNumElements, vsShaderBlob->GetBufferPointer(), vsShaderBlob->GetBufferSize(), &material->mInputLayout);
			if (FAILED(hResult))
			{
				Toast::System::tSys->Print("Couldn't create the default input layer \n");

				return nullptr;
			}
			else 
			{
				vsShaderBlob->Release();
			}
		}
		else 
		{
			hResult = Toast::System::tSys->mD3D->mDevice->CreateInputLayout(nonDefaultIED, numElements, vsShaderBlob->GetBufferPointer(), vsShaderBlob->GetBufferSize(), &material->mInputLayout);
			if (FAILED(hResult))
			{
				Toast::System::tSys->Print("Couldn't create the input layer \n");

				return nullptr;
			}
			else
			{
				vsShaderBlob->Release();
			}
		}

		tempPath = shaderPath;
		tempPath.append("_ps.cso");

		// Creates the pixel shader for the material
		material->mPixelShader = Resources::sResources->GetPixelShader(tempPath);

		// Creates the texture for the material
		material->mTexture = Resources::sResources->GetTexture(texture);

		mMaterials[materialName] = material;

		return material;
	}

	void Resources::LoadWorldData(Toast::Material* material, std::string textureHeight, std::string textureColor)
	{
		material->mTerrainMap = Resources::sResources->GetTexture(textureHeight);
		material->mTexture = Resources::sResources->GetTexture(textureColor);
	}

	ID3D11VertexShader* Resources::GetVertexShader(std::string name) 
	{
		HRESULT hResult;
		ID3D11VertexShader *vertexShader;
		ID3D10Blob *VSRaw;

		std::string basePath = BASE_SHADER_PATH;
		basePath.append(name);

		// Checks if the vertex shader aleady exist
		std::unordered_map<std::string, ID3D11VertexShader*>::iterator s = mVertexShaders.find(name);
		if (s != mVertexShaders.end())
		{
			return s->second;
		}

		std::wstring stemp = std::wstring(basePath.begin(), basePath.end());

		hResult = D3DReadFileToBlob(stemp.c_str(), &VSRaw);
		if (FAILED(hResult))
		{
			return nullptr;
		}

		hResult = Toast::System::tSys->mD3D->mDevice->CreateVertexShader(VSRaw->GetBufferPointer(), VSRaw->GetBufferSize(), NULL, &vertexShader);
		if (FAILED(hResult))
		{
			return nullptr;
		}

		VSRaw->Release();

		mVertexShaders[basePath] = vertexShader;

		return vertexShader;
	}

	ID3D11PixelShader* Resources::GetPixelShader(std::string name)
	{
		HRESULT hResult;
		ID3D11PixelShader *pixelShader;
		ID3D10Blob *PSRaw;

		std::string basePath = BASE_SHADER_PATH;
		basePath.append(name);

		// Checks if the vertex shader aleady exist
		std::unordered_map<std::string, ID3D11PixelShader*>::iterator s = mPixelShaders.find(name);
		if (s != mPixelShaders.end())
		{
			return s->second;
		}

		std::wstring stemp = std::wstring(basePath.begin(), basePath.end());

		hResult = D3DReadFileToBlob(stemp.c_str(), &PSRaw);
		if (FAILED(hResult))
		{
			return nullptr;
		}

		hResult = Toast::System::tSys->mD3D->mDevice->CreatePixelShader(PSRaw->GetBufferPointer(), PSRaw->GetBufferSize(), NULL, &pixelShader);
		if (FAILED(hResult))
		{
			return nullptr;
		}

		PSRaw->Release();

		mPixelShaders[basePath] = pixelShader;

		return pixelShader;
	}

	Texture* Resources::GetTexture(std::string name) 
	{
		HRESULT hResult;

		std::string basePath = BASE_TEXTURE_PATH;
		basePath.append(name);
		basePath.append(".tif");

		std::wstring stemp = std::wstring(basePath.begin(), basePath.end());

		// Checks if the texture aleady exist
		auto s = mTextures.find(name);
		if (s != mTextures.end())
		{
			return s->second;
		}

		Texture* tex = new Texture();
		memset(tex, 0, sizeof(Texture));

		stemp = std::wstring(basePath.begin(), basePath.end());

		tex->mSamplerState = Toast::System::tSys->mD3D->sDefaultTextureSampler;

		hResult = CreateWICTextureFromFile(Toast::System::tSys->mD3D->mDevice, stemp.c_str(), &tex->mResource, &tex->mView);
		if (FAILED(hResult))
		{
			Toast::System::tSys->Print("Error adding texture %s ", basePath.c_str());

			return nullptr;
		}

		Toast::System::tSys->Print("Texture added");

		mTextures[name] = tex;

		return tex;
	}

	Toast::GUIFont* Resources::LoadFontData(std::string name)
	{
		tinyxml2::XMLDocument XMLFontData;
		Toast::GUIFont* GUIFont = new Toast::GUIFont();
		Toast::Texture* texture;
		std::vector<float> xVec, yVec, widthVec, heightVec;

		std::string basePath = BASE_FONT_PATH;
		basePath.append(name);
		basePath.append(".xml");

		auto s = mFonts.find(name);
		if (s != mFonts.end())
		{
			return s->second;
		}

		XMLFontData.LoadFile(basePath.c_str());

		tinyxml2::XMLNode* fontHeader = XMLFontData.FirstChildElement("font");

		for (tinyxml2::XMLElement* child = fontHeader->FirstChildElement(); child != NULL; child = child->NextSiblingElement("char"))
		{
			int x, y, width, height, id;
			child->QueryAttribute("x", &x);
			child->QueryAttribute("y", &y);
			child->QueryAttribute("id", &id);
			child->QueryAttribute("width", &width);
			child->QueryAttribute("height", &height);

			xVec.push_back(static_cast<float>(x));
			yVec.push_back(static_cast<float>(y));
			widthVec.push_back(static_cast<float>(width));
			heightVec.push_back(static_cast<float>(height));
		}

		// Creates the texture for the font
		texture = Resources::sResources->GetTexture(name);

		GUIFont->xVec = xVec;
		GUIFont->yVec = yVec;
		GUIFont->widthVec = widthVec;
		GUIFont->heightVec = heightVec;

		mFonts[name] = GUIFont;

		Toast::System::tSys->Print("Font Created");

		return GUIFont;
	}
}