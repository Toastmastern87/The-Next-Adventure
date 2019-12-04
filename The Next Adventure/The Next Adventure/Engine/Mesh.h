#pragma once

#include <DirectXMath.h>
#include <d3d11.h>
#include <vector>

namespace Toast
{
	struct Vertex
	{
		DirectX::XMFLOAT3 position;
		DirectX::XMFLOAT3 normal;
		DirectX::XMFLOAT3 tangent;
		DirectX::XMFLOAT3 biTangent;
		DirectX::XMFLOAT2 uv;

		Vertex(DirectX::XMFLOAT3 pos, DirectX::XMFLOAT3 nor, DirectX::XMFLOAT3 tan, DirectX::XMFLOAT3 biTan, DirectX::XMFLOAT2 UV)
		{
			position = pos;
			normal = nor;
			tangent = tan;
			biTangent = biTan;
			uv = UV;
		}
	};

	class Material;
	class Resources;
	class D3D;

	class Mesh 
	{
	public:
		Mesh();
		~Mesh();

		void ConstructVertexBuffer();
		void Update();
		void Draw(D3D& d3d);

		Material* mMaterial;
		std::vector<Vertex> mVertices;
		uint32_t mNumVertices;
		std::vector<uint32_t> mIndices;
		uint32_t mNumIndices;
		size_t iVertexBuffer; //index to the vertexbuffer in resourcemanager
		size_t iIndexBuffer; // index to the indexbuffer in resourcemanager

		ID3D11Buffer* mVertexBuffer;
		ID3D11Buffer* mIndexBuffer;
	};
}