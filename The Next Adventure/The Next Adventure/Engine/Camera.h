#pragma once

#include <d3d11.h>
#include <DirectXMath.h>
#include "../Engine/System.h"

namespace Toast {
	class Camera 
	{
	public:
		struct CameraConstantBufferData
		{
			DirectX::XMMATRIX viewMatrix;
			DirectX::XMMATRIX projectionMatrix;
			DirectX::XMFLOAT4 cameraPosition;
			DirectX::XMFLOAT4 cameraDirection;
		};

		struct GUIConstantBufferData
		{
			DirectX::XMMATRIX projectionMatrix;
		};

	public:
		Camera();
		~Camera();

		void OrbitNorth();
		void OrbitSouth();
		void OrbitEast();
		void OrbitWest();
		void ZoomIn();
		void ZoomOut();
		void TurnAround(float delta);
		void SetPosition(DirectX::XMFLOAT3 pos);
		void Rotate(float xR, float yR, float zR);
		void CalculateDirections();
		void SetAspectRatio(float x);
		void SetFoV(float degrees);
		void SetOrtho(float w, float h);
		void SetNear(float n);
		void SetFar(float f);
		void SetMaxZoom(float z);
		void SetMinZoom(float z);

		void Update(double deltaTime);
		void UpdateOrbitalPosition();
		void UpdateAltitude();
		float GetDistanceFromOrigo();
		void CheckOrbitalAngleBoundaries();

		void UpdateMatrices();
		DirectX::XMMATRIX GetViewMatrix();
		DirectX::XMMATRIX GetPerspectiveProjectionMatrix();
		DirectX::XMMATRIX GetOrthoProjectionMatrix();

		DirectX::XMFLOAT3 GetUp() { return mUp; }
		DirectX::XMFLOAT3 GetFoward() { return mForward; }
		DirectX::XMFLOAT3 GetRight() { return mRight; }
		DirectX::XMFLOAT3 GetPosition() { return mPosition; }
		float GetFoV() { return mFoV * (DirectX::XM_PI / 180.0f); }
		float GetFoVRadians() { return mFoV; }
		float GetAspectRatio() { return mAspectRatio; }
		float GetOrthoWidth() { return mOrthoWidth; }
		float GetOrthoHeight() { return mOrthoHeight; }
		float GetFar() { return mFar; }
		float GetNear() { return mNear; }
		float GetAltitude() { return mAltitude; }

		CameraConstantBufferData mConstantBufferData;
		GUIConstantBufferData mGUIConstantBufferData;
		ID3D11Buffer *mConstantBuffer = nullptr;
		ID3D11Buffer *mGUIConstantBuffer = nullptr;

		bool mOrbitDirections[6] = { false,false,false,false,false,false };
		float mOrbitalAngleY = 0.0f, mOrbitalAngleXZ = 0.0f, mRotationXZ = 0.0f;

	private:
		DirectX::XMFLOAT3 mPosition;
		DirectX::XMFLOAT4 mQuaternion;
		DirectX::XMFLOAT3 mUp;
		DirectX::XMFLOAT3 mForward;
		DirectX::XMFLOAT3 mRight;
		float mOrthoWidth = 150.0f, mOrthoHeight = 150.0f;
		float mAspectRatio = 1.0f;
		float mFoV = 2.5f;
		float mNear = 0.1f, mFar = 500.0f;
		float mMaxZoom = 0.0f, mMinZoom = 0.0f;
		float mAltitude = 10000.0f;
		float mZoomOutSpeed = 0.0f, mZoomInSpeed = 0.0f;
	};
};