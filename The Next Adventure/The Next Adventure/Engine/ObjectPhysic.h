#pragma once

#include <d3d11.h>
#include <DirectXMath.h>
#include <DirectXCollision.h>
#include <string>
#include <vector>

namespace Toast
{
	class ObjectPhysic
	{
	public:
		ObjectPhysic(bool useGravity = false);
		~ObjectPhysic();

		void Gravity(float deltaTimes);
		DirectX::XMVECTOR GetPositionTransform();

		bool mUseGravity = false;
		DirectX::XMFLOAT3 mPosition;
		float mGravityConstant = 0.0f, velocity = 0.0f;
		DirectX::BoundingOrientedBox mBoundingOrientedBox;
		DirectX::XMVECTOR mPositionTransform;
	};
};