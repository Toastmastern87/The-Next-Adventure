#include "GameInput.h"

namespace Toast 
{
	// BindInfo struct
	BindInfo::BindInfo() : keyCode(0), keyState(KeyState::JUST_RELEASED) {};
	BindInfo::BindInfo(const unsigned int keyCode, const KeyState keyState) : keyCode(keyCode), keyState(keyState) {};

	// GameCommand struct
	GameCommand::GameCommand() : name(L""), chord(0), type() {};
	GameCommand::GameCommand(const std::wstring& name, const unsigned int keyCode, const Toast::KeyState keyState, const Toast::GameCommandsType type) : name(name), type(type)
	{
		chord.push_back(BindInfo(keyCode, keyState));
	};
	GameCommand::GameCommand(const std::wstring& name, const BindInfo& bi, const Toast::GameCommandsType type) : name(name), type(type)
	{
		chord.push_back(bi);
	};
	GameCommand::GameCommand(const std::wstring& name, const std::vector<BindInfo>& chord, const GameCommandsType type) : name(name), chord(chord), type(type) {};

	GameInput::GameInput()
	{
	}

	GameInput::~GameInput()
	{
		for (auto i : mKeyMap) 
		{
			delete i.second;
		}
		mKeyMap.clear();

		for (auto i : mActiveKeyMap)
		{
			delete i.second;
		}
		mActiveKeyMap.clear();
	}

	void GameInput::SetDefaultKeyMap() 
	{
		mKeyMap.clear();

		mKeyMap[Toast::GameCommands::Quit] = new Toast::GameCommand(L"Quit", Toast::Keyboard::ESC, Toast::KeyState::JUST_PRESSED, Toast::GameCommandsType::KeyboardCommand);
		mKeyMap[Toast::GameCommands::OrbitEast] = new Toast::GameCommand(L"OrbitEast", Toast::Keyboard::D, Toast::KeyState::PRESSED, Toast::GameCommandsType::KeyboardCommand);
		mKeyMap[Toast::GameCommands::OrbitWest] = new Toast::GameCommand(L"OrbitWest", Toast::Keyboard::A, Toast::KeyState::PRESSED, Toast::GameCommandsType::KeyboardCommand);
		mKeyMap[Toast::GameCommands::OrbitNorth] = new Toast::GameCommand(L"OrbitNorth", Toast::Keyboard::W, Toast::KeyState::PRESSED, Toast::GameCommandsType::KeyboardCommand);
		mKeyMap[Toast::GameCommands::OrbitSouth] = new Toast::GameCommand(L"OrbitSouth", Toast::Keyboard::S, Toast::KeyState::PRESSED, Toast::GameCommandsType::KeyboardCommand);
		mKeyMap[Toast::GameCommands::ToggleWireframe] = new Toast::GameCommand(L"ToggleWireframe", Toast::Keyboard::F2, Toast::KeyState::JUST_PRESSED, Toast::GameCommandsType::KeyboardCommand);
		mKeyMap[Toast::GameCommands::ZoomIn] = new Toast::GameCommand(L"ZoomIn", Toast::Mouse::SCROLL_UP, Toast::KeyState::PRESSED, Toast::GameCommandsType::MouseCommand);
		mKeyMap[Toast::GameCommands::ZoomOut] = new Toast::GameCommand(L"ZoomOut", Toast::Mouse::SCROLL_DOWN, Toast::KeyState::PRESSED, Toast::GameCommandsType::MouseCommand);
		mKeyMap[Toast::GameCommands::ResetCursorOffsets] = new Toast::GameCommand(L"ResetCursorOffsets", Toast::Mouse::LEFT, Toast::KeyState::JUST_RELEASED, Toast::GameCommandsType::MouseCommand);
		mKeyMap[Toast::GameCommands::LeftMouseClick] = new Toast::GameCommand(L"LeftMouseClick", Toast::Mouse::LEFT, Toast::KeyState::JUST_PRESSED, Toast::GameCommandsType::MouseCommand);
		mKeyMap[Toast::GameCommands::RightMouseClick] = new Toast::GameCommand(L"RightMouseClick", Toast::Mouse::RIGHT, Toast::KeyState::JUST_PRESSED, Toast::GameCommandsType::MouseCommand);
		mKeyMap[Toast::GameCommands::RightMousePressed] = new Toast::GameCommand(L"RightMousePress", Toast::Mouse::RIGHT, Toast::KeyState::PRESSED, Toast::GameCommandsType::MouseCommand);
		mKeyMap[Toast::GameCommands::RightMouseRelease] = new Toast::GameCommand(L"RightMouseRelease", Toast::Mouse::RIGHT, Toast::KeyState::JUST_RELEASED, Toast::GameCommandsType::MouseCommand);
		mKeyMap[Toast::GameCommands::IncreaseGameSpeed] = new Toast::GameCommand(L"IncreaseGameSpeed", Toast::Keyboard::NUMPADADD, Toast::KeyState::JUST_PRESSED, Toast::GameCommandsType::KeyboardCommand);
		mKeyMap[Toast::GameCommands::DecreaseGameSpeed] = new Toast::GameCommand(L"DecreaseGameSpeed", Toast::Keyboard::NUMPADSUB, Toast::KeyState::JUST_PRESSED, Toast::GameCommandsType::KeyboardCommand);
		mKeyMap[Toast::GameCommands::ShowDebugWindow] = new Toast::GameCommand(L"ShowDebugWindow", Toast::Keyboard::F3, Toast::KeyState::JUST_PRESSED, Toast::GameCommandsType::KeyboardCommand);
		mKeyMap[Toast::GameCommands::TakeScreenshot] = new Toast::GameCommand(L"TakeScreenshot", Toast::Keyboard::PRINTSCREEN, Toast::KeyState::JUST_PRESSED, Toast::GameCommandsType::KeyboardCommand);
	}

	void GameInput::ProcessInputs()
	{
		mNewMousePos = DirectX::XMFLOAT2(static_cast<float>(mCursorPos.x), static_cast<float>(mCursorPos.y));

		// Updates the active key map
		bool isActive = false;
		mActiveKeyMap.clear();

		for (auto i : mKeyMap)
		{
			isActive = true;

			// Checks that all keys are clicked for that game command
			for (auto j : i.second->chord)
			{
				if (i.second->type == Toast::GameCommandsType::KeyboardCommand)
				{
					if (mKeys[j.keyCode] != j.keyState)
					{
						isActive = false;
						break;
					}
				}

				else if (i.second->type == Toast::GameCommandsType::MouseCommand)
				{
					if (mMouse[j.keyCode] != j.keyState)
					{
						isActive = false;
						break;
					}
				}
			}

			if (isActive)
			{
				mActiveKeyMap.insert(std::pair<GameCommands, GameCommand*>(i.first, i.second));
			}
		}

		DirectX::XMStoreFloat2(&mDeltaMousePos, DirectX::XMLoadFloat2(&mNewMousePos) - DirectX::XMLoadFloat2(&mOldMousePos));

		mOldMousePos = mNewMousePos;
	}
}