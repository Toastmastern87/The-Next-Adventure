#include "D3D.h"

namespace Toast 
{
	D3D* D3D::sD3D = nullptr;
	uint32_t D3D::sDefaultInputLayoutNumElements = 5;
	ID3D11SamplerState* D3D::sDefaultTextureSampler;
	Material* D3D::sStarBoxMaterial;

	D3D11_INPUT_ELEMENT_DESC D3D::sDefaultInputLayoutDesc[] =
	{
		{ "POSITION" , 0, DXGI_FORMAT_R32G32B32_FLOAT, 0, 0, D3D11_INPUT_PER_VERTEX_DATA, 0 },
		{ "NORMAL" , 0, DXGI_FORMAT_R32G32B32_FLOAT, 0, D3D11_APPEND_ALIGNED_ELEMENT, D3D11_INPUT_PER_VERTEX_DATA, 0 },
		{ "TANGENT", 0, DXGI_FORMAT_R32G32B32_FLOAT, 0, D3D11_APPEND_ALIGNED_ELEMENT, D3D11_INPUT_PER_VERTEX_DATA, 0 },
		{ "BITANGENT", 0, DXGI_FORMAT_R32G32B32_FLOAT, 0, D3D11_APPEND_ALIGNED_ELEMENT, D3D11_INPUT_PER_VERTEX_DATA, 0 },
		{ "UV", 0, DXGI_FORMAT_R32G32_FLOAT, 0, D3D11_APPEND_ALIGNED_ELEMENT, D3D11_INPUT_PER_VERTEX_DATA, 0 },
	};

	//0 object
	//1 camera
	//2 morph
	//3 2D
	//4 planet
	ID3D11Buffer* D3D::sVSConstantBuffers[5];

	//0 planet
	//1 sunlight
	ID3D11Buffer* D3D::sPSConstantBuffers[2];

	bool D3D::Init() 
	{
		sD3D = this;
			
		// Creates the Swapchain
		DXGI_SWAP_CHAIN_DESC scd;

		ZeroMemory(&scd, sizeof(DXGI_SWAP_CHAIN_DESC));

		scd.BufferCount = 1;
		scd.BufferDesc.Format = DXGI_FORMAT_R8G8B8A8_UNORM;
		scd.BufferUsage = DXGI_USAGE_RENDER_TARGET_OUTPUT;

		RECT windowRect;
		GetClientRect(Toast::System::tSys->mWindow, &windowRect);

		scd.Windowed = !static_cast<UINT>(Toast::System::tSys->mSettings.find("Fullscreen")->second);
		if (scd.Windowed) 
		{
			SetWindowLongPtr(Toast::System::tSys->mWindow, GWL_STYLE, WS_VISIBLE | WS_POPUP);
			//SetWindowPos(Toast::System::tSys->mWindow, HWND_TOP, 0, 0, w, h, SWP_FRAMECHANGED);

			scd.BufferDesc.Width = windowRect.right;
			scd.BufferDesc.Height = windowRect.bottom;
			scd.Flags = DXGI_SWAP_CHAIN_FLAG_ALLOW_MODE_SWITCH;
		}
		else 
		{
			scd.BufferDesc.Width = GetSystemMetrics(SM_CXSCREEN);
			scd.BufferDesc.Height = GetSystemMetrics(SM_CYSCREEN);
			scd.Flags = DXGI_SWAP_CHAIN_FLAG_ALLOW_MODE_SWITCH;
		}

		scd.OutputWindow = Toast::System::tSys->mWindow;
		scd.SampleDesc.Count = 1;
		scd.SampleDesc.Quality = 0;

		D3D_FEATURE_LEVEL featureLevel = D3D_FEATURE_LEVEL_11_0;

		HRESULT hResult;

		hResult = D3D11CreateDeviceAndSwapChain(NULL, D3D_DRIVER_TYPE_HARDWARE, NULL, D3D11_CREATE_DEVICE_DEBUG | D3D11_CREATE_DEVICE_BGRA_SUPPORT, &featureLevel, 1, D3D11_SDK_VERSION, &scd, &mSwapchain,	&mDevice, NULL, &mDeviceContext);
		if (FAILED(hResult)) 
		{ 
			MessageBox(Toast::System::tSys->mWindow, "Couldn't initiate the D3D object", "Error", MB_OK);

			return false; 
		}

		SetViewport(0.0f, 0.0f, static_cast<float>(windowRect.right), static_cast<float>(windowRect.bottom));

		CreateBackBuffer();
		CreateDepthBuffer(windowRect.right, windowRect.bottom);
		CreateDepthStencil();
		CreateBlendStates();

		D3D11_SAMPLER_DESC samplerDesc;
		samplerDesc.Filter = D3D11_FILTER_MIN_MAG_MIP_LINEAR;
		samplerDesc.AddressU = D3D11_TEXTURE_ADDRESS_WRAP;
		samplerDesc.AddressV = D3D11_TEXTURE_ADDRESS_WRAP;
		samplerDesc.AddressW = D3D11_TEXTURE_ADDRESS_WRAP;
		samplerDesc.MipLODBias = 0.0f;
		samplerDesc.MaxAnisotropy = 1;
		samplerDesc.ComparisonFunc = D3D11_COMPARISON_ALWAYS;
		samplerDesc.BorderColor[0] = 0;
		samplerDesc.BorderColor[1] = 0;
		samplerDesc.BorderColor[2] = 0;
		samplerDesc.BorderColor[3] = 0;
		samplerDesc.MinLOD = 0;
		samplerDesc.MaxLOD = D3D11_FLOAT32_MAX;

		hResult = mDevice->CreateSamplerState(&samplerDesc, &D3D::sDefaultTextureSampler);
		if (FAILED(hResult))
		{
			return false;
		}

		D3D11_RASTERIZER_DESC rasterDesc;
		memset(&rasterDesc, 0, sizeof(D3D11_RASTERIZER_DESC));
		rasterDesc.CullMode = D3D11_CULL_BACK;
		rasterDesc.FillMode = D3D11_FILL_SOLID;
		rasterDesc.DepthClipEnable = true;

		hResult = mDevice->CreateRasterizerState(&rasterDesc, &mRasterizerState);
		if (FAILED(hResult))
		{
			return false;
		}

		rasterDesc.CullMode = D3D11_CULL_FRONT;
		rasterDesc.FillMode = D3D11_FILL_SOLID;

		hResult = mDevice->CreateRasterizerState(&rasterDesc, &mFrontCullingRasterizerState);
		if (FAILED(hResult))
		{
			return false;
		}

		rasterDesc.CullMode = D3D11_CULL_FRONT;
		rasterDesc.FillMode = D3D11_FILL_WIREFRAME;

		hResult = mDevice->CreateRasterizerState(&rasterDesc, &mFrontCullingWFRasterizerState);
		if (FAILED(hResult))
		{
			return false;
		}

		rasterDesc.CullMode = D3D11_CULL_BACK;
		rasterDesc.FillMode = D3D11_FILL_WIREFRAME;

		hResult = mDevice->CreateRasterizerState(&rasterDesc, &mWireframeRasterizerState);
		if (FAILED(hResult))
		{
			return false;
		}

		mDeviceContext->RSSetState(mRasterizerState);

		return true;
	}

	void D3D::SetViewport(float xPos, float yPos, float width, float height)
	{
		D3D11_VIEWPORT viewport;
		ZeroMemory(&viewport, sizeof(D3D11_VIEWPORT));

		viewport.TopLeftX = xPos;
		viewport.TopLeftY = yPos;
		viewport.Width = width;
		viewport.Height = height;
		viewport.MinDepth = 0.0f;
		viewport.MaxDepth = 1.0f;

		mDeviceContext->RSSetViewports(1, &viewport);
	}

	void D3D::Draw(Toast::Game& game)
	{
		Toast::D3D& _this = *static_cast<Toast::D3D*>(this);
		static const float color[4] = { 0.0f, 1.0f, 0.0f, 0.0f };

		mDeviceContext->ClearRenderTargetView(mBackBuffer, color);
		mDeviceContext->ClearDepthStencilView(mDepthStencilView, D3D11_CLEAR_FLAG::D3D11_CLEAR_DEPTH, 1.0f, 0);
		mDeviceContext->IASetPrimitiveTopology(D3D11_PRIMITIVE_TOPOLOGY_TRIANGLELIST);

		if (mWireframe)
		{
			mDeviceContext->RSSetState(mWireframeRasterizerState);
		}
		else
		{
			mDeviceContext->RSSetState(mRasterizerState);
		}

		ID3D11RasterizerState* tempRS = nullptr;

		for (int i = 0; i < game.mObjects3D.size(); i++) 
		{
			if (game.mObjects3D[i]->mFrontCulling && !mWireframe) 
			{
				mDeviceContext->RSGetState(&tempRS);
				mDeviceContext->RSSetState(mFrontCullingRasterizerState);
			}

			game.mObjects3D[i]->Draw(_this);

			if (game.mObjects3D[i]->mFrontCulling && !mWireframe)
			{
				mDeviceContext->RSSetState(tempRS);
			}
		}

		if (game.mMars->mFrontCulling && !mWireframe)
		{
			mDeviceContext->RSGetState(&tempRS);
			mDeviceContext->RSSetState(mFrontCullingRasterizerState);
		}

		game.mMars->Draw(_this);

		if (game.mMars->mFrontCulling && !mWireframe)
		{
			mDeviceContext->RSSetState(tempRS);
		}

		mDeviceContext->OMSetDepthStencilState(mDepthDisabledStencilState, 1);
		mDeviceContext->OMSetBlendState(mAlphaBlendingEnabledState, mBlendFactor, 0xffffffff);
		mDeviceContext->RSSetState(mRasterizerState);

		game.mGUI->Draw(_this);

		mDeviceContext->OMSetDepthStencilState(mDepthStencilState, 1);
		mDeviceContext->OMSetBlendState(mAlphaBlendingDisabledState, mBlendFactor, 0xffffffff);

		if (tempRS) 
		{
			tempRS->Release();
			tempRS = 0;
		}

		mSwapchain->Present(0, 0);

		return;
	}

	bool D3D::CreateBackBuffer() 
	{
		HRESULT hResult;

		ID3D11Texture2D *pBackBuffer;

		hResult = mSwapchain->GetBuffer(0, __uuidof(ID3D11Texture2D), (LPVOID*)&pBackBuffer);
		if (FAILED(hResult))
		{
			return false;
		}	

		D3D11_RENDER_TARGET_VIEW_DESC RTVDesc;

		ZeroMemory(&RTVDesc, sizeof(D3D11_RENDER_TARGET_VIEW_DESC));

		RTVDesc.Format = DXGI_FORMAT::DXGI_FORMAT_R32G32B32A32_FLOAT;
		RTVDesc.ViewDimension = D3D11_RTV_DIMENSION_TEXTURE2D;

		hResult = mDevice->CreateRenderTargetView(pBackBuffer, NULL, &mBackBuffer);
		if (FAILED(hResult))
		{
			return false;
		}

		pBackBuffer->Release();

		return true;
	}

	bool D3D::CreateDepthBuffer(int screenWidth, int screenHeight)
	{
		HRESULT hResult;
		D3D11_TEXTURE2D_DESC depthBufferDesc;

		depthBufferDesc.Width = screenWidth;
		depthBufferDesc.Height = screenHeight;
		depthBufferDesc.MipLevels = 1;
		depthBufferDesc.ArraySize = 1;
		depthBufferDesc.Format = DXGI_FORMAT_D24_UNORM_S8_UINT;
		depthBufferDesc.SampleDesc.Count = 1;
		depthBufferDesc.SampleDesc.Quality = 0;
		depthBufferDesc.Usage = D3D11_USAGE_DEFAULT;
		depthBufferDesc.BindFlags = D3D11_BIND_DEPTH_STENCIL;
		depthBufferDesc.CPUAccessFlags = 0;
		depthBufferDesc.MiscFlags = 0;

		hResult = mDevice->CreateTexture2D(&depthBufferDesc, NULL, &mDepthStencilBuffer);
		if (FAILED(hResult))
		{
			return false;
		}

		return true;
	}

	bool D3D::CreateDepthStencil() 
	{
		HRESULT hResult;
		D3D11_DEPTH_STENCIL_DESC depthStencilDesc;
		D3D11_DEPTH_STENCIL_VIEW_DESC depthStencilViewDesc;

		// Depth test parameters
		depthStencilDesc.DepthEnable = true;
		depthStencilDesc.DepthWriteMask = D3D11_DEPTH_WRITE_MASK_ALL;
		depthStencilDesc.DepthFunc = D3D11_COMPARISON_LESS;

		// Stencil test parameters
		depthStencilDesc.StencilEnable = true;
		depthStencilDesc.StencilReadMask = 0xFF;
		depthStencilDesc.StencilWriteMask = 0xFF;

		// Stencil operations if pixel is front-facing
		depthStencilDesc.FrontFace.StencilFailOp = D3D11_STENCIL_OP_KEEP;
		depthStencilDesc.FrontFace.StencilDepthFailOp = D3D11_STENCIL_OP_INCR;
		depthStencilDesc.FrontFace.StencilPassOp = D3D11_STENCIL_OP_KEEP;
		depthStencilDesc.FrontFace.StencilFunc = D3D11_COMPARISON_ALWAYS;

		// Stencil operations if pixel is back-facing
		depthStencilDesc.BackFace.StencilFailOp = D3D11_STENCIL_OP_KEEP;
		depthStencilDesc.BackFace.StencilDepthFailOp = D3D11_STENCIL_OP_DECR;
		depthStencilDesc.BackFace.StencilPassOp = D3D11_STENCIL_OP_KEEP;
		depthStencilDesc.BackFace.StencilFunc = D3D11_COMPARISON_ALWAYS;

		hResult = mDevice->CreateDepthStencilState(&depthStencilDesc, &mDepthStencilState);
		if (FAILED(hResult))
		{
			return false;
		}

		depthStencilDesc.DepthEnable = false;

		hResult = mDevice->CreateDepthStencilState(&depthStencilDesc, &mDepthDisabledStencilState);
		if (FAILED(hResult))
		{
			return false;
		}

		mDeviceContext->OMSetDepthStencilState(mDepthStencilState, 1);

		ZeroMemory(&depthStencilViewDesc, sizeof(depthStencilViewDesc));

		depthStencilViewDesc.Format = DXGI_FORMAT_D24_UNORM_S8_UINT;
		depthStencilViewDesc.ViewDimension = D3D11_DSV_DIMENSION_TEXTURE2D;
		depthStencilViewDesc.Texture2D.MipSlice = 0;

		hResult = mDevice->CreateDepthStencilView(mDepthStencilBuffer, &depthStencilViewDesc, &mDepthStencilView);
		if (FAILED(hResult))
		{
			return false;
		}

		mDeviceContext->OMSetRenderTargets(1, &mBackBuffer, mDepthStencilView);

		return true;
	}

	bool D3D::CreateBlendStates()
	{
		HRESULT hResult;
		D3D11_BLEND_DESC bd;

		bd.AlphaToCoverageEnable = true;
		bd.IndependentBlendEnable = false;
		bd.RenderTarget[0].BlendEnable = true;
		bd.RenderTarget[0].BlendOp = D3D11_BLEND_OP_ADD;
		bd.RenderTarget[0].BlendOpAlpha = D3D11_BLEND_OP_ADD;
		bd.RenderTarget[0].SrcBlend = D3D11_BLEND_SRC_ALPHA;
		bd.RenderTarget[0].DestBlend = D3D11_BLEND_INV_SRC_ALPHA;
		bd.RenderTarget[0].SrcBlendAlpha = D3D11_BLEND_ONE;
		bd.RenderTarget[0].DestBlendAlpha = D3D11_BLEND_ZERO;
		bd.RenderTarget[0].RenderTargetWriteMask = 0x0f;

		hResult = mDevice->CreateBlendState(&bd, &mAlphaBlendingEnabledState);
		if (FAILED(hResult))
		{
			return false;
		}

		bd.RenderTarget[0].BlendEnable = false;
		bd.AlphaToCoverageEnable = false;

		hResult = mDevice->CreateBlendState(&bd, &mAlphaBlendingDisabledState);
		if (FAILED(hResult))
		{
			return false;
		}

		mDeviceContext->OMSetBlendState(mAlphaBlendingDisabledState, mBlendFactor, 0xffffffff);

		return true;
	}

	D3D::~D3D() 
	{
		if (Toast::System::tSys->mSettings.find("Fullscreen")->second == 1)
		{
			mSwapchain->SetFullscreenState(FALSE, NULL); 
		}

		CLEAN(mAlphaBlendingEnabledState);
		CLEAN(mAlphaBlendingDisabledState);
		CLEAN(mDepthStencilState);
		CLEAN(mDepthDisabledStencilState);
		CLEAN(mDepthStencilView);
		CLEAN(mDepthStencilBuffer);
		CLEAN(mRasterizerState);
		CLEAN(mWireframeRasterizerState);
		CLEAN(mFrontCullingRasterizerState);
		CLEAN(mInputLayout);
		CLEAN(mSwapchain);
		CLEAN(mBackBuffer);
		CLEAN(mDevice);
		CLEAN(mDeviceContext);
	}

	void D3D::SetObjectConstantBuffer(ID3D11Buffer* buf)
	{
		D3D::sVSConstantBuffers[0] = buf;
	}

	void D3D::SetCameraConstantBuffer(ID3D11Buffer* buf) 
	{
		D3D::sVSConstantBuffers[1] = buf;
	}

	void D3D::SetMorphConstantBuffer(ID3D11Buffer* buf)
	{
		D3D::sVSConstantBuffers[2] = buf;
	}

	void D3D::SetGUIConstantBuffer(ID3D11Buffer* buf)
	{
		D3D::sVSConstantBuffers[3] = buf;
	}

	void D3D::SetPlanetConstantBuffer(ID3D11Buffer* buf)
	{
		D3D::sVSConstantBuffers[4] = buf;
		D3D::sPSConstantBuffers[0] = buf;
	}

	void D3D::SetSunlightConstantBuffer(ID3D11Buffer* buf)
	{
		D3D::sPSConstantBuffers[1] = buf;
	}

	void D3D::UploadConstantBuffersToGPU() 
	{
		mDeviceContext->VSSetConstantBuffers(0, 5, D3D::sVSConstantBuffers);
		mDeviceContext->PSSetConstantBuffers(0, 2, D3D::sPSConstantBuffers);
	}

	void D3D::CaptureScreenshot() 
	{
	}
};