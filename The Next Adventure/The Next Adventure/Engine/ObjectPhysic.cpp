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

		//velocity is suppose to be in km/s not in m/s therefor I didvide by 1000.0f
		velocity = velocity + ((mGravityConstant * deltaTime) / 1000.0f);

		mPositionTransform = DirectX::XMVectorScale(gravityVector, (velocity * deltaTime));
	}

	DirectX::XMVECTOR ObjectPhysic::GetPositionTransform() 
	{
		DirectX::XMVECTOR returnVector;

		returnVector = mPositionTransform;

		mPositionTransform = DirectX::XMVectorSet(0.0f, 0.0f, 0.0f, 1.0f);

		return returnVector;
	}
}