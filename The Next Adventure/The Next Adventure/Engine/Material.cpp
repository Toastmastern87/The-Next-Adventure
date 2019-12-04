#include "Material.h"

namespace Toast 
{
	Material::Material()
	{
		mTexture = nullptr;
		mTerrainMap = nullptr;

		mSamplerState = nullptr;
		mView = nullptr;
	}

	Material::~Material() 
	{
		mSamplerState = nullptr;
		mView = nullptr;

		if (mInputLayout)
		{
			mInputLayout->Release();
			mInputLayout = nullptr;
		}

		if (mVertexShader)
		{
			mVertexShader = nullptr;
		}
		if (mPixelShader)
		{
			mPixelShader = nullptr;
		}
	}
};