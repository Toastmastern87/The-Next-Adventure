#pragma once

#include <d3d11.h>
#include <DirectXMath.h>
#include <DirectXCollision.h>
#include <string>
#include <vector>
#include "ObjectPhysic.h"

namespace Toast
{
	class WorldPhysic
	{
	public:
		WorldPhysic();
		~WorldPhysic();

		void Update(float deltaTime);

		std::vector<Toast::ObjectPhysic*> mObjects;
	};
};