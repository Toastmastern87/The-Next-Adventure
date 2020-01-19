#pragma once

#pragma comment(lib, "d3d11.lib")
#pragma comment(lib, "d3dcompiler.lib")

#include <d3d11.h>
#include <d3d10.h>
#include <directxmath.h>
#include <vector>
#include "System.h"
#include "Mesh.h"
#include "Object3D.h"
#include "ObjectPhysic.h"
#include "World.h"
#include "Resources.h"
#include "Material.h"

namespace Toast {
	class Game;
	class Material;
	class Object3D;

	using namespace DirectX;

	class D3D
	{
	public:
		D3D() {};
		~D3D();

		bool Init();
		void Draw(Toast::Game& game);
		void SetViewport(float xPos, float yPos, float width, float height);
		bool CreateBackBuffer();
		bool CreateDepthBuffer(int screenWidth, int screenHeight);
		bool CreateDepthStencil();
		bool CreateBlendStates();
		void CaptureScreenshot();

		void SetObjectConstantBuffer(ID3D11Buffer* buf);
		void SetCameraConstantBuffer(ID3D11Buffer* buf);
		void SetMorphConstantBuffer(ID3D11Buffer* buf);
		void SetPlanetConstantBuffer(ID3D11Buffer* buf);
		void SetGUIConstantBuffer(ID3D11Buffer* buf);
		void SetSunlightConstantBuffer(ID3D11Buffer* buf);
		void UploadConstantBuffersToGPU();

		ID3D11RenderTargetView* mBackBuffer = nullptr;
		ID3D11Texture2D* mDepthStencil = nullptr;
		ID3D11DepthStencilState* mDepthStencilState = nullptr;
		ID3D11DepthStencilState* mDepthDisabledStencilState = nullptr;
		ID3D11DepthStencilView* mDepthStencilView = nullptr;
		ID3D11RasterizerState* mRasterizerState = nullptr;
		ID3D11RasterizerState* mWireframeRasterizerState = nullptr;
		ID3D11RasterizerState* mFrontCullingRasterizerState = nullptr;
		ID3D11RasterizerState* mFrontCullingWFRasterizerState = nullptr;
		ID3D11Texture2D* mDepthStencilBuffer = nullptr;
		IDXGISwapChain* mSwapchain = nullptr;
		ID3D11Device* mDevice = nullptr;
		ID3D11DeviceContext* mDeviceContext = nullptr;
		ID3D11InputLayout* mInputLayout = nullptr;
		ID3D11BlendState* mAlphaBlendingEnabledState = nullptr;
		ID3D11BlendState* mAlphaBlendingDisabledState = nullptr;
		float mBlendFactor[4] = { 0.0f, 0.0f, 0.0f, 0.0f };

		bool mWireframe = false;

		Object3D *mStarBox;

		static D3D* sD3D;
		static D3D11_INPUT_ELEMENT_DESC sDefaultInputLayoutDesc[];
		static uint32_t sDefaultInputLayoutNumElements;
		static ID3D11SamplerState* sDefaultTextureSampler;
		static ID3D11Buffer* sVSConstantBuffers[5];
		static ID3D11Buffer* sPSConstantBuffers[2];
		static Material *sStarBoxMaterial;
	};
};