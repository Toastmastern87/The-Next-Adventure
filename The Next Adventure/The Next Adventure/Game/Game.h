#pragma once

#include "../Engine/Camera.h"
#include "../Engine/Object3D.h"
#include "../Engine/World.h"
#include "../Engine/SphereMesh.h"
#include "../Engine/Material.h"
#include "../Engine/GUI.h"
#include "../Engine/GUIPanel.h"
#include "../Engine/GUICursor.h"
#include "../Engine/WorldPhysic.h"
#include "../Engine/Time.h"
#include "GameInput.h"

const float MARSROTATESPEED = DirectX::XM_2PI / 89676000.0f;
const double MARSSUNORBITSPEED = (0.55333333333f * DirectX::XM_PI) / 100319175000.0f;

namespace Toast 
{
	class Camera;
	class Object3D;
	class World;
	class GameInput;
	class Time;
	class GUI;

	class Game 
	{
	public:
		Game() {};
		void Init();
		void Update(double deltaTime);
		void InitializeCamera();
		void Stop();

		std::vector<Toast::Object3D*> mObjects3D;
		Camera *mCamera = nullptr;
		WorldPhysic* mWorldPhysic = nullptr;
		GameInput *mInput = nullptr;
		Object3D *mStarSphere = nullptr;
		Object3D* mSpaceship = nullptr;
		World *mMars = nullptr;
		Material *mStarSphereMaterial = nullptr;
		Material *mMarsMaterial = nullptr;
		Material* mSpaceshipMaterial = nullptr;
		Material* mTargetBoxMaterial = nullptr;

		GUI *mGUI = nullptr;
		Material *mDebugWindowMaterial = nullptr;
		Material *mDebugWindowTitleBarMaterial = nullptr;
		Material *mGameTimePanelMaterial = nullptr;
		Material* mGameResearchPanelMaterial = nullptr;
		Material *mGameSpeedDeactivatedPanelMaterial = nullptr;
		Material *mGameSpeedActivatedPanelMaterial = nullptr;
		Material *mCursorMaterial = nullptr;
		Material *mFontMaterial = nullptr;
		Material *mTimeFontMaterial = nullptr;
		Material *mYearDayFontMaterial = nullptr;
		Toast::GUIFont* mGeneralFont = nullptr;
		Toast::GUIFont* mTimeFont = nullptr;
		Toast::GUIFont* mYearDayFont = nullptr;
		Time* mTime = nullptr;

		int mMarsSeasonalRotationDir = 1;
		float mSunlightRotateAngle = 0.0f, mSunlightSeasonalRotateAngle = 0.0f;

		POINT mRawCursorPos;
	};
};