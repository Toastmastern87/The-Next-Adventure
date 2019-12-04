#pragma once

#include <unordered_map>
#include <d3dcompiler.h>
#include <d3dcommon.h>
#include <d3d11.h>
#include <WICTextureLoader.h>
#include "tinyxml2.h"

#define BASE_SHADER_PATH "../The Next Adventure/Data/Shaders/"
#define BASE_TEXTURE_PATH "../The Next Adventure/Data/Textures/"
#define BASE_FONT_PATH "../The Next Adventure/Data/Fonts/"

namespace Toast 
{
	struct Vertex;
	struct SphereVertex;
	struct SphereCell;
	struct Texture;

	struct GUIVertex;
	struct GUIFont;

	class Object3D;
	class Mesh;
	class SphereMesh;
	class Material;

	class GUI;
	class GUIPanel;
	class GUIText;

	class Resources 
	{
	public:
		Resources();
		~Resources();

		Toast::Material* GetMaterial(std::string materialName, std::string texture, std::string shaderPath, D3D11_INPUT_ELEMENT_DESC *nonDefaultIED = nullptr, int numElements = 0, bool instanced = false);
		Toast::Material* LoadMaterial(std::string materialName, std::string texture, std::string shaderPath, D3D11_INPUT_ELEMENT_DESC *nonDefaultIED = nullptr, int numElements = 0, bool instanced = false);

		ID3D11VertexShader* GetVertexShader(std::string name);
		ID3D11PixelShader* GetPixelShader(std::string name);
		Texture* GetTexture(std::string name);

		size_t CreateVertexBuffer(std::vector<Vertex> vertices, size_t numVertices);
		size_t CreateIndexBuffer(std::vector<uint32_t> indices, size_t numIndices);
		size_t CreateIndexBuffer(std::vector<uint32_t> indices);

		// For spheres only
		size_t CreateInstanceBuffer(std::vector<SphereCell> instances, size_t numInstances);
		void RemapInstanceBuffer(std::vector<SphereCell> instances, size_t instanceID);
		size_t CreateVertexBuffer(std::vector<SphereVertex> vertices);

		// For planets only
		void LoadTerrainData(Toast::Material*, std::string);

		// For GUI
		size_t CreateVertexBuffer(std::vector<GUIVertex> vertices);
		size_t CreateVertexBuffer(std::vector<GUIVertex> vertices, size_t numVertices);
		void RemapVertexBuffer(std::vector<GUIVertex> vertices, size_t verticesID);
		void RemapIndexBuffer(std::vector<uint32_t> indices, size_t indicesID);

		// For GUI Texts
		Toast::GUIFont* LoadFontData(std::string filename);

		std::unordered_map<std::string, Toast::Material*> mMaterials;
		std::unordered_map<std::string, ID3D11VertexShader*> mVertexShaders;
		std::unordered_map<std::string, ID3D11PixelShader*> mPixelShaders;
		std::unordered_map<std::string, Texture*> mTextures;
		std::unordered_map<std::string, GUIFont*> mFonts;

		std::vector<ID3D11Buffer*> mVertexBuffers;
		std::vector<ID3D11Buffer*> mIndexBuffers;
		std::vector<ID3D11Buffer*> mInstanceBuffers;
		std::vector<Toast::Mesh*> mMeshes;
		std::vector<Toast::SphereMesh*> mSphereMeshes;

		std::vector<Toast::GUIPanel*> mGUIPanels;

		static Resources* sResources;
	};
};