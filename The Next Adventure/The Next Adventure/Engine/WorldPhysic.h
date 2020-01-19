#pragma once

#include <d3d11.h>
#include <DirectXMath.h>
#include <DirectXCollision.h>
#include "ObjectPhysic.h"
#include <string>
#include <vector>

namespace Toast
{
	class WorldPhysic
	{
	public:
		WorldPhysic();
		~WorldPhysic();

		std::vector<ObjectPhysic*> mObjects;
	};
};