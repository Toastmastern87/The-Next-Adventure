#include "Game.h"

namespace Toast 
{
	void Toast::Game::Init() 
	{
		Toast::System::tSys->Print("Initating the game");

		// Creates the Input Handler for the game
		mInput = new Toast::GameInput();
		mInput->SetDefaultKeyMap();

		// Creating the camera that the game will use
		mCamera = new Toast::Camera();
		mCamera->SetPosition(XMFLOAT3(0.0f, 0.0f, 10000.0f));
		//mCamera->Rotate(0.0f, (-DirectX::XM_PI / 2), (DirectX::XM_PI / 2));
		//DirectX::XMConvertToRadians(-180.0f)
		mCamera->Rotate(0.0f, DirectX::XMConvertToRadians(180.0f), 0.0f);
		mCamera->SetAspectRatio(Toast::System::tSys->mSettings["WindowSizeX"] / Toast::System::tSys->mSettings["WindowSizeY"]);
		mCamera->SetFoV(45.0f);
		mCamera->SetNear(0.1f);
		mCamera->SetFar(40000.0f);
		mCamera->SetOrtho(Toast::System::tSys->mSettings["WindowSizeX"], Toast::System::tSys->mSettings["WindowSizeY"]);
		// 10.0f being the min altitude above Mars, and 6610.5 as min zoom gives an distance to origo to 10000.0 with the Mars radius being 3389.5
		mCamera->SetMaxZoom(3399.5f);
		mCamera->SetMinZoom(10000.0f);

		Toast::System::tSys->Print("Camera created");

		D3D11_INPUT_ELEMENT_DESC sphereIED[] =
		{
			{ "TEXCOORD" , 0, DXGI_FORMAT_R32G32_FLOAT, 0, 0, D3D11_INPUT_PER_VERTEX_DATA, 0 },
			{ "TEXCOORD" , 1, DXGI_FORMAT_R32G32_FLOAT, 0, D3D11_APPEND_ALIGNED_ELEMENT, D3D11_INPUT_PER_VERTEX_DATA, 0 },
			{ "TEXTUREID", 0, DXGI_FORMAT_R32_SINT, 1, 0, D3D11_INPUT_PER_INSTANCE_DATA, 1 },
			{ "POSITION", 0, DXGI_FORMAT_R32G32B32_FLOAT, 1, D3D11_APPEND_ALIGNED_ELEMENT, D3D11_INPUT_PER_INSTANCE_DATA, 1 },
			{ "POSITION", 1, DXGI_FORMAT_R32G32B32_FLOAT, 1, D3D11_APPEND_ALIGNED_ELEMENT, D3D11_INPUT_PER_INSTANCE_DATA, 1 },
			{ "POSITION", 2, DXGI_FORMAT_R32G32B32_FLOAT, 1, D3D11_APPEND_ALIGNED_ELEMENT, D3D11_INPUT_PER_INSTANCE_DATA, 1 }
		};

		// Creating the star sphere that will be the outher bounds of the game.
		mStarSphere = new Toast::Object3D();
		mStarSphere->CreateSphere(1, 3, false, true, 20000.0f);
		mStarSphereMaterial = Resources::sResources->GetMaterial("StarSphere", "StarSphere", "StarSphere", sphereIED, 6);

		mStarSphere->mSphereMeshes[0]->mMaterial = mStarSphereMaterial;	

		mStarSphere->mScale = DirectX::XMFLOAT3(20000.0f, 20000.0f, 20000.0f);
		mStarSphere->mRotation = DirectX::XMFLOAT3(0.0f, 0.0f, 0.0f);
		mStarSphere->mPosition = DirectX::XMFLOAT3(0.0f, 0.0f, 0.0f);

		mStarSphere->Update();

		mObjects3D.push_back(mStarSphere);

		Toast::System::tSys->Print("Star sphere created");

		mMars = new Toast::World();
		mMars->CreateWorld(22, 4, false, 3389.5f, -8.2f, 21.229f);
		//mMars->mRotation = DirectX::XMFLOAT3(0.0f, 0.0f, 0.0f);
		mMars->SetSunlightDirection(DirectX::XMFLOAT4(0.0f, 0.0f, -1.0f, 1.0f));
		mMars->SetSunlightColor(DirectX::XMFLOAT4(1.0f, 1.0f, 1.0f, 1.0f));
		mMarsMaterial = Resources::sResources->GetMaterial("Mars", "MarsHeightMap8K", "Planet", sphereIED, 6);
		Resources::sResources->LoadWorldData(mMarsMaterial, "MarsHeightMap8K", "MarsColorMap8K");

		mMars->mSphereMeshes[0]->mMaterial = mMarsMaterial;

		// Run an update to get the matrices correctly
		mMars->Update();

		Toast::System::tSys->Print("Mars created");

		//Setting max rotation to the camera after Mars is created
		mCamera->SetMaxRotation((mCamera->GetFoVRadians() / 2.0f) + atanf(mMars->mRadius / mCamera->GetAltitude()));

		mGUI = new Toast::GUI();

		D3D11_INPUT_ELEMENT_DESC GUIIED[] =
		{
			{ "POSITION" , 0, DXGI_FORMAT_R32G32B32_FLOAT, 0, 0, D3D11_INPUT_PER_VERTEX_DATA, 0 },
			{ "TEXCOORD", 0, DXGI_FORMAT_R32G32_FLOAT, 0, D3D11_APPEND_ALIGNED_ELEMENT, D3D11_INPUT_PER_VERTEX_DATA, 0 }
		};

		std::string windowName = "Debug Window";
		mGUI->AddPanel(0.0f, 0.0f, 400.0f, 500.0f, &windowName, true);

		mDebugWindowMaterial = Resources::sResources->GetMaterial("DebugWindow", "DebugWindowTexture", "GUI", GUIIED, 2);
		mDebugWindowTitleBarMaterial = Resources::sResources->GetMaterial("DebugWindowTitleBar", "DebugWindowTitleBarTexture", "GUI", GUIIED, 2);

		mGUI->mGUIPanels[0]->mMaterial = mDebugWindowMaterial;
		mGUI->mGUIPanels[0]->mTitleBarMaterial = mDebugWindowTitleBarMaterial;
		mGUI->mGUIPanels[0]->mMaxX = Toast::System::tSys->mSettings["WindowSizeX"] - mGUI->mGUIPanels[0]->mWidth;
		mGUI->mGUIPanels[0]->mMaxY = Toast::System::tSys->mSettings["WindowSizeY"] - mGUI->mGUIPanels[0]->mHeight - mGUI->mGUIPanels[0]->mTitleBarOffset;

		Toast::System::tSys->Print("Debug window created");

		//Creating the game time GUI Panel
		mGUI->AddPanel((Toast::System::tSys->mSettings["WindowSizeX"] - (656.0f * 0.45f)), 0.0f, (656.0f * 0.45f), (221.0f * 0.35f));

		mGameTimePanelMaterial = Resources::sResources->GetMaterial("GameTime", "GameTimePanelTexture", "GUI", GUIIED, 2);
		mGUI->mGUIPanels[1]->mMaterial = mGameTimePanelMaterial;

		// Sets up the font and add element for time panel created earlier
		mTimeFontMaterial = Resources::sResources->GetMaterial("SegoeUIFontp35", "SegoeUIp35", "GUI", GUIIED, 2);
		mYearDayFontMaterial = Resources::sResources->GetMaterial("SegoeUIFontp20", "SegoeUIp20", "GUI", GUIIED, 2);

		std::string fontName = "SegoeUIp35";
		mTimeFont = Resources::sResources->LoadFontData(fontName);
		fontName = "SegoeUIp20";
		mYearDayFont = Resources::sResources->LoadFontData(fontName);

		std::string timeString = "15:32:15";
		Toast::GUIText *timeText = new Toast::GUIText();
		timeText->CreateText(140.0f, -7.0f, &timeString, mTimeFont, mTimeFontMaterial);

		mGUI->mGUIPanels[1]->AddElement(timeText);

		std::string mYearString = "M: 1";
		Toast::GUIText *mYearText = new Toast::GUIText();
		mYearText->CreateText(80.0f, -6.0f, &mYearString, mYearDayFont, mYearDayFontMaterial);

		mGUI->mGUIPanels[1]->AddElement(mYearText);

		std::string solString = "Sol: 1";
		Toast::GUIText *solText = new Toast::GUIText();
		solText->CreateText(80.0f, 16.0f, &solString, mYearDayFont, mYearDayFontMaterial);

		mGUI->mGUIPanels[1]->AddElement(solText);

		// Sets up the font and add an element to the Debug Window created earlier
		mFontMaterial = Resources::sResources->GetMaterial("SegoeUIFontp14", "SegoeUIp14", "GUI", GUIIED, 2);

		fontName = "SegoeUIp14";
		mGeneralFont = Resources::sResources->LoadFontData(fontName);

		std::string fpsString = "FPS: " + std::to_string(Toast::System::tSys->mFPS);
		Toast::GUIText *fpsText = new Toast::GUIText();
		fpsText->CreateText(10.0f, 10.0f, &fpsString, mGeneralFont, mFontMaterial);

		mGUI->mGUIPanels[0]->AddElement(fpsText);

		Toast::System::tSys->Print("FPS Text created in the Debug window");

		std::string altitudeString = "Altitude: " + std::to_string(mCamera->GetAltitude());
		Toast::GUIText* altitudeText = new Toast::GUIText();
		altitudeText->CreateText(10.0f, 30.0f, &altitudeString, mGeneralFont, mFontMaterial);

		mGUI->mGUIPanels[0]->AddElement(altitudeText);

		Toast::System::tSys->Print("Altitude Text created in the Debug window");

		std::string verticesString = "Vertices being rendered: " + std::to_string(mCamera->GetAltitude());
		Toast::GUIText* verticesText = new Toast::GUIText();
		verticesText->CreateText(10.0f, 50.0f, &verticesString, mGeneralFont, mFontMaterial);

		mGUI->mGUIPanels[0]->AddElement(verticesText);

		Toast::System::tSys->Print("Vertices Text created in the Debug window");

		std::string orbitalAngleYString = "Orbital Angle Y: " + std::to_string(DirectX::XMConvertToDegrees(mCamera->mOrbitalAngleY));
		Toast::GUIText* orbitalAngleYText = new Toast::GUIText();
		orbitalAngleYText->CreateText(10.0f, 70.0f, &orbitalAngleYString, mGeneralFont, mFontMaterial);

		mGUI->mGUIPanels[0]->AddElement(orbitalAngleYText);

		Toast::System::tSys->Print("orbitalAngleY Text created in the Debug window");

		std::string orbitalAngleXZString = "Orbital Angle XZ: " + std::to_string(DirectX::XMConvertToDegrees(mCamera->mOrbitalAngleXZ));
		Toast::GUIText* orbitalAngleXZText = new Toast::GUIText();
		orbitalAngleXZText->CreateText(10.0f, 90.0f, &orbitalAngleXZString, mGeneralFont, mFontMaterial);

		mGUI->mGUIPanels[0]->AddElement(orbitalAngleXZText);

		Toast::System::tSys->Print("orbitalAngleXZ Text created in the Debug window");

		std::string rotationXZString = "Rotation XZ: " + std::to_string(DirectX::XMConvertToDegrees(mCamera->mRotationXZ));
		Toast::GUIText* rotationXZText = new Toast::GUIText();
		rotationXZText->CreateText(10.0f, 110.0f, &rotationXZString, mGeneralFont, mFontMaterial);

		mGUI->mGUIPanels[0]->AddElement(rotationXZText);

		Toast::System::tSys->Print("Rotation XZ Text created in the Debug window");

		std::string maxRotationXZString = "Max Rotation XZ: " + std::to_string(DirectX::XMConvertToDegrees(mCamera->GetMaxRotation()));
		Toast::GUIText* maxRotationXZText = new Toast::GUIText();
		maxRotationXZText->CreateText(10.0f, 130.0f, &maxRotationXZString, mGeneralFont, mFontMaterial);

		mGUI->mGUIPanels[0]->AddElement(maxRotationXZText);

		Toast::System::tSys->Print("Max Rotation XZ Text created in the Debug window");

		std::string posXString = "Position X: " + std::to_string(static_cast<int>(mCamera->GetPosition().x));
		Toast::GUIText* posXText = new Toast::GUIText();
		posXText->CreateText(10.0f, 150.0f, &posXString, mGeneralFont, mFontMaterial);

		mGUI->mGUIPanels[0]->AddElement(posXText);

		Toast::System::tSys->Print("Pos X Text created in the Debug window");

		std::string posYString = "Y: " + std::to_string(static_cast<int>(mCamera->GetPosition().y));
		Toast::GUIText* posYText = new Toast::GUIText();
		posYText->CreateText(140.0f, 150.0f, &posYString, mGeneralFont, mFontMaterial);

		mGUI->mGUIPanels[0]->AddElement(posYText);

		Toast::System::tSys->Print("Pos Y Text created in the Debug window");

		std::string posZString = "Z: " + std::to_string(static_cast<int>(mCamera->GetPosition().z));
		Toast::GUIText* posZText = new Toast::GUIText();
		posZText->CreateText(210.0f, 150.0f, &posZString, mGeneralFont, mFontMaterial);

		mGUI->mGUIPanels[0]->AddElement(posZText);

		Toast::System::tSys->Print("Pos Z Text created in the Debug window");

		//Creating the game time speed GUI Panels
		mGUI->AddPanel((Toast::System::tSys->mSettings["WindowSizeX"] - 34.0f), 2.0f, (48.0f * 0.5f), (21.0f * 0.5f));
		mGUI->AddPanel((Toast::System::tSys->mSettings["WindowSizeX"] - 34.0f), 15.0f, (48.0f * 0.5f), (21.0f * 0.5f));
		mGUI->AddPanel((Toast::System::tSys->mSettings["WindowSizeX"] - 34.0f), 28.0f, (48.0f * 0.5f), (21.0f * 0.5f));
		mGUI->AddPanel((Toast::System::tSys->mSettings["WindowSizeX"] - 34.0f), 41.0f, (48.0f * 0.5f), (21.0f * 0.5f));

		mGameSpeedActivatedPanelMaterial = Resources::sResources->GetMaterial("GameTimeSpeedActivated", "GameSpeedActivatedTexture", "GUI", GUIIED, 2);
		mGameSpeedDeactivatedPanelMaterial = Resources::sResources->GetMaterial("GameTimeSpeedDeactivated", "GameSpeedDeactivatedTexture", "GUI", GUIIED, 2);
		mGUI->mGUIPanels[2]->mMaterial = mGameSpeedActivatedPanelMaterial;
		mGUI->mGUIPanels[3]->mMaterial = mGameSpeedActivatedPanelMaterial;
		mGUI->mGUIPanels[4]->mMaterial = mGameSpeedActivatedPanelMaterial;
		mGUI->mGUIPanels[5]->mMaterial = mGameSpeedActivatedPanelMaterial;

		GetCursorPos(&mRawCursorPos);
		mGUI->AddCursor(mRawCursorPos, 40.0f, 40.0f);

		mCursorMaterial = Resources::sResources->GetMaterial("Cursor", "CursorTexture", "GUI", GUIIED, 2);
		mGUI->mCursor->mMaterial = mCursorMaterial;

		Toast::System::tSys->Print("Mouse cursor created");
	}

	void Toast::Game::Update(double deltaTime) 
	{
		// Handles the input and the event belonging to those keys
		mInput->ProcessInputs();

		for (auto i : mInput->mActiveKeyMap) 
		{
			switch (i.first) 
			{
			case Toast::GameCommands::Quit:
				Toast::System::tSys->mDone = true;
				break;
			case Toast::GameCommands::OrbitEast:
				Toast::System::tSys->mTheNextFrontier->mCamera->OrbitEast();
				break;
			case Toast::GameCommands::OrbitWest:
				Toast::System::tSys->mTheNextFrontier->mCamera->OrbitWest();
				break;
			case Toast::GameCommands::OrbitNorth:
				Toast::System::tSys->mTheNextFrontier->mCamera->OrbitNorth();
				break;
			case Toast::GameCommands::OrbitSouth:
				Toast::System::tSys->mTheNextFrontier->mCamera->OrbitSouth();
				break;
			case Toast::GameCommands::ToggleWireframe:
				Toast::System::tSys->mD3D->mWireframe = !Toast::System::tSys->mD3D->mWireframe;
				break;
			case Toast::GameCommands::ZoomIn:
				Toast::System::tSys->mTheNextFrontier->mCamera->ZoomIn();
				break;
			case Toast::GameCommands::ZoomOut:
				Toast::System::tSys->mTheNextFrontier->mCamera->ZoomOut();
				break;
			case Toast::GameCommands::ResetCursorOffsets:
				mGUI->mCursor->mMinX = 0.0f;
				mGUI->mCursor->mMaxX = Toast::System::tSys->mSettings["WindowSizeX"];
				mGUI->mCursor->mMinY = 0.0f;
				mGUI->mCursor->mMaxY = Toast::System::tSys->mSettings["WindowSizeY"];
				mGUI->mGUIPanels[0]->mTargeted = false;
				break;
			case Toast::GameCommands::LeftMouseClick:
				mGUI->mCursor->CheckRayIntersection2D(mGUI->mGUIPanels[0]);
				break;
			case Toast::GameCommands::RightMouseClick:
				mGUI->mCursor->HideCursor();
				break;
			case Toast::GameCommands::RightMousePressed:
				GetCursorPos(&mRawCursorPos);
				Toast::System::tSys->mTheNextFrontier->mCamera->TurnAround(static_cast<float>(mRawCursorPos.x) - static_cast<float>(mGUI->mCursor->mOldPos.x), static_cast<float>(mRawCursorPos.y) - static_cast<float>(mGUI->mCursor->mOldPos.y));
				break;
			case Toast::GameCommands::RightMouseRelease:
				mGUI->mCursor->ShowCursor();
				break;
			case Toast::GameCommands::IncreaseGameSpeed:
				if (mGameSpeedActiveIndex < 3) mGameSpeedActiveIndex++;
				break;
			case Toast::GameCommands::DecreaseGameSpeed:
				if (mGameSpeedActiveIndex > 0) mGameSpeedActiveIndex--;
				break;
			case Toast::GameCommands::ShowDebugWindow:
				mGUI->mGUIPanels[0]->mVisible = !mGUI->mGUIPanels[0]->mVisible;
				break;
			case Toast::GameCommands::TakeScreenshot:
				Toast::System::tSys->Print("Printscreen pressed");
				break;
			}
		}

		if (mGUI->mGUIPanels[2]->mClicked) 
		{ 
			mGameSpeedActiveIndex = 3;
			mGUI->mGUIPanels[2]->mClicked = false;
		}
		if (mGUI->mGUIPanels[3]->mClicked) 
		{
			mGameSpeedActiveIndex = 2;
			mGUI->mGUIPanels[3]->mClicked = false;
		}
		if (mGUI->mGUIPanels[4]->mClicked)
		{
			mGameSpeedActiveIndex = 1;
			mGUI->mGUIPanels[4]->mClicked = false;
		}
		if (mGUI->mGUIPanels[5]->mClicked)
		{
			mGameSpeedActiveIndex = 0;
			mGUI->mGUIPanels[5]->mClicked = false;
		}

		// Updates the color of the time GUI indication panels
		if (mGameSpeedActiveIndex == 3)
		{
			mGUI->mGUIPanels[2]->mMaterial = mGameSpeedActivatedPanelMaterial;
			mGUI->mGUIPanels[3]->mMaterial = mGameSpeedActivatedPanelMaterial;
			mGUI->mGUIPanels[4]->mMaterial = mGameSpeedActivatedPanelMaterial;
			mGUI->mGUIPanels[5]->mMaterial = mGameSpeedActivatedPanelMaterial;
		}
		else if (mGameSpeedActiveIndex == 2)
		{
			mGUI->mGUIPanels[2]->mMaterial = mGameSpeedDeactivatedPanelMaterial;
			mGUI->mGUIPanels[3]->mMaterial = mGameSpeedActivatedPanelMaterial;
			mGUI->mGUIPanels[4]->mMaterial = mGameSpeedActivatedPanelMaterial;
			mGUI->mGUIPanels[5]->mMaterial = mGameSpeedActivatedPanelMaterial;
		}
		else if (mGameSpeedActiveIndex == 1)
		{
			mGUI->mGUIPanels[2]->mMaterial = mGameSpeedDeactivatedPanelMaterial;
			mGUI->mGUIPanels[3]->mMaterial = mGameSpeedDeactivatedPanelMaterial;
			mGUI->mGUIPanels[4]->mMaterial = mGameSpeedActivatedPanelMaterial;
			mGUI->mGUIPanels[5]->mMaterial = mGameSpeedActivatedPanelMaterial;
		}
		else if (mGameSpeedActiveIndex == 0)
		{
			mGUI->mGUIPanels[2]->mMaterial = mGameSpeedDeactivatedPanelMaterial;
			mGUI->mGUIPanels[3]->mMaterial = mGameSpeedDeactivatedPanelMaterial;
			mGUI->mGUIPanels[4]->mMaterial = mGameSpeedDeactivatedPanelMaterial;
			mGUI->mGUIPanels[5]->mMaterial = mGameSpeedActivatedPanelMaterial;
		}
		else 
		{
			mGUI->mGUIPanels[2]->mMaterial = mGameSpeedDeactivatedPanelMaterial;
			mGUI->mGUIPanels[3]->mMaterial = mGameSpeedDeactivatedPanelMaterial;
			mGUI->mGUIPanels[4]->mMaterial = mGameSpeedDeactivatedPanelMaterial;
			mGUI->mGUIPanels[5]->mMaterial = mGameSpeedDeactivatedPanelMaterial;
		}

		//::System::tSys->Print("mGameSpeed: %f", mGameSpeed);

		// Updates the game time
		//mGameTime += deltaTime;

		// Updates the Camera in the game
		mCamera->Update(deltaTime);
		mCamera->UpdateAltitude();
		mCamera->SetMaxRotation((mCamera->GetFoVRadians() / 2.0f) + atanf(mMars->mRadius / mCamera->GetAltitude()));
		mCamera->CheckBoundaries();
		//Pitch, Yaw, Roll
		mCamera->Rotate(mCamera->mOrbitalAngleY + mCamera->mRotationY, mCamera->mOrbitalAngleXZ + mCamera->mRotationXZ + DirectX::XMConvertToRadians(180.0f), 0.0f);
		mCamera->UpdateOrbitalPosition();
		mCamera->CheckOrbitalAngleBoundaries();
		mCamera->UpdateMatrices();

		GetCursorPos(&mRawCursorPos);
		mGUI->mCursor->UpdatePos(mRawCursorPos);

		// Updates the position of the cursor and GUI panels that are moveable
		if (mGUI->mGUIPanels[0]->mTargeted)
		{
			mGUI->mGUIPanels[0]->UpdatePos(mGUI->mCursor->GetDeltaPos());
		}
	
		//Updates the debug window with new data
		std::string fpsString = "FPS: " + std::to_string(Toast::System::tSys->mFPS);
		mGUI->mGUIPanels[0]->mElements[0]->UpdateText(&fpsString);
		std::string altitudeString = "Altitude: " + std::to_string((int)(mCamera->GetAltitude() - mMars->mRadius)) + "km";
		mGUI->mGUIPanels[0]->mElements[1]->UpdateText(&altitudeString);
		std::string verticesString = "Vertices being rendered: " + std::to_string((int)(mCamera->GetAltitude() - mMars->mRadius)) + "km";
		mGUI->mGUIPanels[0]->mElements[2]->UpdateText(&verticesString);
		std::string orbitalAngleYString = "Orbital Angle Y: " + std::to_string(DirectX::XMConvertToDegrees(mCamera->mOrbitalAngleY));
		mGUI->mGUIPanels[0]->mElements[3]->UpdateText(&orbitalAngleYString);
		std::string orbitalAngleXZString = "Orbital Angle XZ: " + std::to_string(DirectX::XMConvertToDegrees(mCamera->mOrbitalAngleXZ));
		mGUI->mGUIPanels[0]->mElements[4]->UpdateText(&orbitalAngleXZString);
		std::string rotationXZString = "Rotation XZ: " + std::to_string(DirectX::XMConvertToDegrees(mCamera->mRotationXZ));
		mGUI->mGUIPanels[0]->mElements[5]->UpdateText(&rotationXZString);
		std::string maxRotationXZString = "Max Rotation XZ: " + std::to_string(DirectX::XMConvertToDegrees(mCamera->GetMaxRotation()));
		mGUI->mGUIPanels[0]->mElements[6]->UpdateText(&maxRotationXZString);
		std::string posXString = "Position X: " + std::to_string(static_cast<int>(mCamera->GetPosition().x));
		mGUI->mGUIPanels[0]->mElements[7]->UpdateText(&posXString);
		std::string posYString = "Y: " + std::to_string(static_cast<int>(mCamera->GetPosition().y));
		mGUI->mGUIPanels[0]->mElements[8]->UpdateText(&posYString);
		std::string posZString = "Z: " + std::to_string(static_cast<int>(mCamera->GetPosition().z));
		mGUI->mGUIPanels[0]->mElements[9]->UpdateText(&posZString);

		mMars->Update();
	}

	void Toast::Game::Stop()
	{
		delete mCamera;
		mCamera = nullptr;

		for (int i = 0; i < mObjects3D.size(); i++)
		{
			mObjects3D[i] = nullptr;
		}
		mObjects3D.clear();
	}
};