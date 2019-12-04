#pragma once

#include "../Engine/Camera.h"
#include "../Engine/Object3D.h"
#include "../Engine/World.h"
#include "../Engine/SphereMesh.h"
#include "../Engine/Material.h"
#include "../Engine/GUI.h"
#include "../Engine/GUIPanel.h"
#include "GameInput.h"

namespace Toast 
{
	class Camera;
	class Object3D;
	class World;
	class GameInput;
	class GUI;

	class Game 
	{
	public:
		Game() {};
		void Init();
		void Update(double deltaTime);
		void Stop();

		std::vector<Toast::Object3D*> mObjects3D;
		Camera *mCamera;
		GameInput *mInput;
		Object3D *mStarSphere;
		World *mMars;
		Material *mStarSphereMaterial;
		Material *mMarsMaterial;

		GUI *mGUI;
		Material *mDebugWindowMaterial;
		Material *mDebugWindowTitleBarMaterial;
		Material *mGameTimePanelMaterial;
		Material *mGameSpeedDeactivatedPanelMaterial;
		Material *mGameSpeedActivatedPanelMaterial;
		Material *mCursorMaterial;
		Material *mFontMaterial;
		Material *mTimeFontMaterial;
		Material *mYearDayFontMaterial;
		Toast::GUIFont* mGeneralFont;
		Toast::GUIFont* mTimeFont;
		Toast::GUIFont* mYearDayFont;

		LARGE_INTEGER mGameTime;
		float mGameSpeeds[4] = { 1.0f, 10.0f, 100.0f, 1000.0f };
		int mGameSpeedActiveIndex = 0;
	};
};