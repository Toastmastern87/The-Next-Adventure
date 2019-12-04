#include "Functions.h"

namespace Toast 
{
	float GetVectorDistance(const DirectX::XMFLOAT3& a, const DirectX::XMFLOAT3& b)
	{
		float distance;

		distance = sqrt(((a.x - b.x) * (a.x - b.x)) + ((a.y - b.y) * (a.y - b.y)) + ((a.z - b.z) * (a.z - b.z)));

		return distance;
	}
}