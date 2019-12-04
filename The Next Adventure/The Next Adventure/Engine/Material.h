#pragma once

#include <d3d11.h>

namespace Toast 
{
	struct Texture
	{
		ID3D11Resource* mResource = nullptr;
		ID3D11ShaderResourceView* mView = nullptr;
		ID3D11SamplerState* mSamplerState = nullptr;

		~Texture()
		{
			if (mResource)
			{
				mResource->Release();
				mResource = nullptr;
			}
			if (mView)
			{
				mView->Release();
				mView = nullptr;
			}

			mSamplerState = nullptr;
		}
	};

	class Material
	{
	public:
		Material();
		~Material();

		ID3D11VertexShader *mVertexShader;
		ID3D11PixelShader *mPixelShader;
		ID3D11InputLayout *mInputLayout;

		Texture* mTexture;
		Texture* mTerrainMap;
		ID3D11ShaderResourceView* mView = nullptr;
		ID3D11SamplerState* mSamplerState = nullptr;

		bool instanced;
	};

};