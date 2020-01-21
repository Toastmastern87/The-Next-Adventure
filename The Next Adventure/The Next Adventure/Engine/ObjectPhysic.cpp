#include "ObjectPhysic.h"
#include "System.h"

namespace Toast
{
	ObjectPhysic::ObjectPhysic(bool useGravity)
	{
		mUseGravity = useGravity;

		mPositionTransform = DirectX::XMVectorSet(0.0f, 0.0f, 0.0f, 1.0f);
	}

	ObjectPhysic::~ObjectPhysic()
	{
	}

	void ObjectPhysic::Gravity(float deltaTime)
	{
		DirectX::XMVECTOR gravityVector;

		gravityVector = DirectX::XMVectorScale(DirectX::XMVector3Normalize(DirectX::XMLoadFloat3(&mPosition)), -1.0f);

		//Toast::System::tSys->Print("Gravity Vector x: %f, y: %f, z: %f", DirectX::XMVectorGetX(gravityVector), DirectX::XMVectorGetY(gravityVector), DirectX::XMVectorGetZ(gravityVector));

		velocity = velocity + (mGravityConstant * (deltaTime / 1000.0f));

		mPositionTransform = DirectX::XMVectorScale(gravityVector, velocity);
	}

	DirectX::XMVECTOR ObjectPhysic::GetPositionTransform() 
	{
		DirectX::XMVECTOR returnVector;

		returnVector = mPositionTransform;

		mPositionTransform = DirectX::XMVectorSet(0.0f, 0.0f, 0.0f, 1.0f);

		return returnVector;
	}
}