#include "Camera.h"

namespace Toast 
{
	Camera::Camera() 
	{
		D3D11_BUFFER_DESC cbDesc;
		cbDesc.ByteWidth = sizeof(CameraConstantBufferData);
		cbDesc.Usage = D3D11_USAGE_DYNAMIC;
		cbDesc.BindFlags = D3D11_BIND_CONSTANT_BUFFER;
		cbDesc.CPUAccessFlags = D3D11_CPU_ACCESS_WRITE;
		cbDesc.MiscFlags = 0;
		cbDesc.StructureByteStride = 0;

		D3D11_SUBRESOURCE_DATA initData;
		initData.pSysMem = &mConstantBufferData;
		initData.SysMemPitch = 0;
		initData.SysMemSlicePitch = 0;

		Toast::System::tSys->mD3D->mDevice->CreateBuffer(&cbDesc, &initData, &mConstantBuffer);

		D3D11_BUFFER_DESC cbGUIDesc;
		cbGUIDesc.ByteWidth = sizeof(GUIConstantBufferData);
		cbGUIDesc.Usage = D3D11_USAGE_DYNAMIC;
		cbGUIDesc.BindFlags = D3D11_BIND_CONSTANT_BUFFER;
		cbGUIDesc.CPUAccessFlags = D3D11_CPU_ACCESS_WRITE;
		cbGUIDesc.MiscFlags = 0;
		cbGUIDesc.StructureByteStride = 0;

		D3D11_SUBRESOURCE_DATA initGUIData;
		initGUIData.pSysMem = &mGUIConstantBufferData;
		initGUIData.SysMemPitch = 0;
		initGUIData.SysMemSlicePitch = 0;

		Toast::System::tSys->mD3D->mDevice->CreateBuffer(&cbGUIDesc, &initData, &mGUIConstantBuffer);

		SetFoV(75.0f);
		mNear = 0.1f;
		mFar = 1000.0f;
		SetAspectRatio(1.0f);
		Rotate(0.0f, 0.0f, 0.0f);
		SetPosition(DirectX::XMFLOAT3(0.0f, 0.0f, 0.0f));
	}

	Camera::~Camera() 
	{
		mConstantBuffer->Release();
		mConstantBuffer = nullptr;
	}

	void Camera::OrbitNorth() 
	{
		mOrbitDirections[0] = true;
	}

	void Camera::OrbitSouth() 
	{
		mOrbitDirections[1] = true;
	}

	void Camera::OrbitEast()
	{
		mOrbitDirections[2] = true;
	}

	void Camera::OrbitWest()
	{
		mOrbitDirections[3] = true;
	}

	void Camera::ZoomIn() 
	{
		mOrbitDirections[4] = true;
	}

	void Camera::ZoomOut() 
	{
		mOrbitDirections[5] = true;
	}

	void Camera::SetPosition(DirectX::XMFLOAT3 position)
	{
		mPosition = position;
		
		mAltitude = GetDistanceFromOrigo();
	}

	void Camera::Rotate(float xR, float yR, float zR)
	{
		DirectX::XMVECTOR quad = XMQuaternionRotationRollPitchYaw(xR, yR, zR);

		DirectX::XMStoreFloat4(&mQuaternion, quad);

		CalculateDirections();
	}

	void Camera::CalculateDirections() 
	{
		DirectX::XMVECTOR quat = DirectX::XMLoadFloat4(&mQuaternion);

		DirectX::XMFLOAT3 inputEdit = DirectX::XMFLOAT3(1, 0, 0);
		DirectX::XMVECTOR input = DirectX::XMLoadFloat3(&inputEdit);
		input = DirectX::XMVector3Rotate(input, quat);
		DirectX::XMStoreFloat3(&inputEdit, input);
		mRight = inputEdit;

		inputEdit = DirectX::XMFLOAT3(0, 1, 0);
		input = DirectX::XMLoadFloat3(&inputEdit);
		input = DirectX::XMVector3Rotate(input, quat);
		DirectX::XMStoreFloat3(&inputEdit, input);
		mUp = inputEdit;

		inputEdit = DirectX::XMFLOAT3(0, 0, 1);
		input = DirectX::XMLoadFloat3(&inputEdit);
		input = DirectX::XMVector3Rotate(input, quat);
		DirectX::XMStoreFloat3(&inputEdit, input);
		mForward = inputEdit;
	}

	void Camera::SetAspectRatio(float x) 
	{
		mAspectRatio = x;
	}

	void Camera::SetFoV(float degrees)
	{
		mFoV = degrees * (DirectX::XM_PI / 180.0f);
	}

	void Camera::SetOrtho(float w, float h)
	{
		mOrthoWidth = w;
		mOrthoHeight = h;
	}

	void Camera::SetNear(float n)
	{
		mNear = n;
	}

	void Camera::SetFar(float f)
	{
		mFar = f;
	}

	void Camera::SetMaxZoom(float z) 
	{
		mMaxZoom = z;
	}

	void Camera::SetMinZoom(float z)
	{
		mMinZoom = z;
	}

	void Camera::Update(double deltaTime)
	{
		if (mOrbitDirections[0])
		{
			if (mOrbitalAngleY >= (((2 * DirectX::XM_PI) - (DirectX::XM_PI / 2.0f)) + DirectX::XMConvertToRadians(15.0f)) || mOrbitalAngleY < ((DirectX::XM_PI / 2.0f) - DirectX::XMConvertToRadians(15.0f)))
			{
				mOrbitalAngleY += static_cast<float>(deltaTime) * (GetDistanceFromOrigo() / 10000.0f);
			}
			else 
			{
				mOrbitalAngleY = (DirectX::XM_PI / 2.0f) - DirectX::XMConvertToRadians(15.0f);
			}
		}

		if (mOrbitDirections[1]) 
		{
			if (mOrbitalAngleY > (((2 * DirectX::XM_PI) - (DirectX::XM_PI / 2.0f)) + DirectX::XMConvertToRadians(15.0f)) || mOrbitalAngleY <= ((DirectX::XM_PI / 2.0f) - DirectX::XMConvertToRadians(15.0f)))
			{
				mOrbitalAngleY -= static_cast<float>(deltaTime) * (GetDistanceFromOrigo() / 10000.0f);
			}
			else
			{
				mOrbitalAngleY = (2 * DirectX::XM_PI) - (DirectX::XM_PI / 2.0f) + DirectX::XMConvertToRadians(15.0f);
			}
		}

		if (mOrbitDirections[2])
		{
			mOrbitalAngleXZ += static_cast<float>(deltaTime) * (GetDistanceFromOrigo() / 10000.0f);
		}

		if (mOrbitDirections[3])
		{
			mOrbitalAngleXZ -= static_cast<float>(deltaTime) * (GetDistanceFromOrigo() / 10000.0f);
		}

		if (mOrbitDirections[4])
		{
			mZoomInSpeed = static_cast<float>(deltaTime) * powf(log10f(mAltitude - mMaxZoom), 5.0f) * 5.0f;
		}
		else
		{
			mZoomInSpeed -= static_cast<float>(deltaTime) * mZoomInSpeed * 4.0f;
		}

		if (mOrbitDirections[5])
		{
			mZoomOutSpeed = static_cast<float>(deltaTime) * powf(log10f(mAltitude - mMaxZoom), 5.0f) * 5.0f;
		}
		else 
		{
			mZoomOutSpeed -= static_cast<float>(deltaTime) * mZoomOutSpeed * 4.0f;
		}

		UpdateAltitude();

		for (size_t i = 0; i < 6; i++)
		{
			mOrbitDirections[i] = false;
		}
	}

	void Camera::UpdateMatrices() 
	{
		Toast::D3D *d3d = Toast::System::tSys->mD3D;

		DirectX::XMMATRIX perspectiveProjectionMatrix;
		DirectX::XMMATRIX orthoProjectionMatrix;
		DirectX::XMMATRIX viewMatrix;

		perspectiveProjectionMatrix = DirectX::XMMatrixPerspectiveFovLH(mFoV, mAspectRatio, mNear, mFar);
		viewMatrix = DirectX::XMMatrixLookToLH(DirectX::XMLoadFloat3(&mPosition), DirectX::XMLoadFloat3(&mForward), DirectX::XMLoadFloat3(&mRight));

		mConstantBufferData.projectionMatrix = perspectiveProjectionMatrix;
		mConstantBufferData.viewMatrix = viewMatrix;
		mConstantBufferData.cameraPosition = DirectX::XMFLOAT4(mPosition.x, mPosition.y, mPosition.z, 1.0f);
		mConstantBufferData.cameraDirection = DirectX::XMFLOAT4(mForward.x, mForward.y, mForward.z, 1.0f);

		D3D11_MAPPED_SUBRESOURCE ms;
		d3d->mDeviceContext->Map(mConstantBuffer, NULL, D3D11_MAP_WRITE_DISCARD, NULL, &ms);
		memcpy(ms.pData, &mConstantBufferData, sizeof(mConstantBufferData));
		d3d->mDeviceContext->Unmap(mConstantBuffer, NULL);

		d3d->SetCameraConstantBuffer(mConstantBuffer);

		// For 2D rendering the engine sets up the Orthographic projection matrix aswell
		ZeroMemory(&ms, sizeof(ms));

		orthoProjectionMatrix = DirectX::XMMatrixOrthographicLH(mOrthoWidth, mOrthoHeight, mNear, mFar);

		mGUIConstantBufferData.projectionMatrix = orthoProjectionMatrix;

		d3d->mDeviceContext->Map(mGUIConstantBuffer, NULL, D3D11_MAP_WRITE_DISCARD, NULL, &ms);
		memcpy(ms.pData, &mGUIConstantBufferData, sizeof(mGUIConstantBufferData));
		d3d->mDeviceContext->Unmap(mGUIConstantBuffer, NULL);

		d3d->SetGUIConstantBuffer(mGUIConstantBuffer);
	}

	XMMATRIX Camera::GetViewMatrix() 
	{
		DirectX::XMMATRIX viewMatrix;

		viewMatrix = DirectX::XMMatrixLookToLH(DirectX::XMLoadFloat3(&mPosition), DirectX::XMLoadFloat3(&mForward), DirectX::XMLoadFloat3(&mRight));

		return viewMatrix;
	}

	XMMATRIX Camera::GetPerspectiveProjectionMatrix() 
	{
		DirectX::XMMATRIX perspectiveProjectionMatrix;

		perspectiveProjectionMatrix = DirectX::XMMatrixPerspectiveFovLH(mFoV, mAspectRatio, mNear, mFar);

		return perspectiveProjectionMatrix;
	}

	XMMATRIX Camera::GetOrthoProjectionMatrix()
	{
		DirectX::XMMATRIX orthoProjectionMatrix;
		orthoProjectionMatrix = DirectX::XMMatrixOrthographicLH(mOrthoWidth, mOrthoHeight, mNear, mFar);

		return orthoProjectionMatrix;
	}

	void Camera::UpdateOrbitalPosition() 
	{
		mPosition.x += (cosf(mOrbitalAngleXZ) * mAltitude * cosf(mOrbitalAngleY)) - mPosition.x;
		mPosition.y += (sinf(mOrbitalAngleY) * mAltitude) - mPosition.y;
		mPosition.z += (sinf(mOrbitalAngleXZ) * mAltitude * cosf(mOrbitalAngleY)) - mPosition.z;
	}

	void Camera::UpdateAltitude() 
	{
		if (mAltitude < mMinZoom) {
			mAltitude += mZoomOutSpeed;
		}
		else
		{
			mZoomOutSpeed = 0.0f;
			mAltitude = mMinZoom;
		}

		if (mAltitude > mMaxZoom) {
			mAltitude -= mZoomInSpeed;
		}
		else
		{
			mZoomInSpeed = 0.0f;
			mAltitude = mMaxZoom;
		}
	}

	float Camera::GetDistanceFromOrigo()
	{
		return (sqrtf((mPosition.x * mPosition.x) + (mPosition.y * mPosition.y) + (mPosition.z * mPosition.z)));
	}

	void Camera::CheckOrbitalAngleBoundaries()
	{
		if (mOrbitalAngleY < 0.0f)
		{
			mOrbitalAngleY += (DirectX::XM_2PI);
		}
		else if (mOrbitalAngleY >(DirectX::XM_2PI))
		{
			mOrbitalAngleY -= (DirectX::XM_2PI);
		}

		if (mOrbitalAngleXZ < 0.0f)
		{
			mOrbitalAngleXZ += (DirectX::XM_2PI);
		}
		else if (mOrbitalAngleXZ >(DirectX::XM_2PI))
		{
			mOrbitalAngleXZ -= (DirectX::XM_2PI);
		}
	}
};