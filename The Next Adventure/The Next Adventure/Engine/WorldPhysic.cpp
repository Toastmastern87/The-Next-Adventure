#include "WorldPhysic.h"

namespace Toast
{
	WorldPhysic::WorldPhysic()
	{
	}

	WorldPhysic::~WorldPhysic() 
	{
	}

	void WorldPhysic::Update(float deltaTime)
	{
		for (ObjectPhysic* object : mObjects)
		{
			if (object->mUseGravity) 
			{
				object->Gravity(deltaTime);
			}
		}
	}
}