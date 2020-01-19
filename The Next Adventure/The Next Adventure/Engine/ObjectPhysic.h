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
		ObjectPhysic();
		~ObjectPhysic();

		float mGravityConstant;
		DirectX::BoundingOrientedBox mBoundingOrientedBox;
	};
};