#pragma once

#define DIRECTINPUT_VERSION 0x0800

#pragma comment(lib, "dinput8.lib")
#pragma comment(lib, "dxguid.lib")

#include "../Engine/System.h"
#include <dinput.h>

namespace Toast
{
	enum GameCommandsType : int8_t
	{
		MouseCommand = 0,
		KeyboardCommand = 1
	};

	enum GameCommands
	{
		Quit,
		OrbitEast,
		OrbitWest,
		OrbitNorth,
		OrbitSouth,
		ToggleWireframe,
		ZoomIn,
		ZoomOut,
		MoveWindow,
		ResetCursorOffsets,
		LeftMouseClick,
		IncreaseGameSpeed,
		DecreaseGameSpeed,
		ShowDebugWindow
	};

	enum KeyState : int8_t
	{
		JUST_RELEASED = -1,
		RELEASED = 0,
		PRESSED = 1,
		JUST_PRESSED = 2
	};

	enum Keyboard
	{
		ESC = 0x1B,
		F2 = 0x71,
		F3 = 0x72,
		A = 0x41,
		D = 0x44,
		S = 0x53,
		W = 0x57,
		NUMPADADD = 0x6B,
		NUMPADSUB = 0x6D
	};

	enum Mouse
	{
		LEFT = 0,
		RIGHT = 1,
		MIDDLE = 2,
		SCROLL_UP = 3,
		SCROLL_DOWN = 4
	};

	struct BindInfo
	{
	private:
		const unsigned int keyCode;
		const KeyState keyState;

	public:
		BindInfo();
		BindInfo(const unsigned int keyCode, const KeyState keyState);
		~BindInfo() {};

		friend struct GameCommand;
		friend class GameInput;
	};

	struct GameCommand
	{
	private:
		std::wstring name;
		std::vector<BindInfo> chord;
		const GameCommandsType type;

	public:
		GameCommand();
		GameCommand(const std::wstring& name, const unsigned int keyCode, const KeyState keyState, const GameCommandsType type);
		GameCommand(const std::wstring& name, const BindInfo& bi, const GameCommandsType type);
		GameCommand(const std::wstring& name, const std::vector<BindInfo>& chord, const GameCommandsType type);
		~GameCommand() {};

		friend class GameInput;
	};

	class GameInput
	{
	public:
		GameInput();
		~GameInput();

		void ProcessInputs();
		void SetDefaultKeyMap();

		signed char mKeys[256];
		signed char mMouse[5];
		POINT mCursorPos;
		std::array<int8_t, 4> mMouseKeys;
		std::unordered_map<GameCommands, GameCommand*> mKeyMap;
		std::unordered_map<GameCommands, GameCommand*> mActiveKeyMap;

		DirectX::XMFLOAT2 mDeltaMousePos = DirectX::XMFLOAT2(0.0f, 0.0f);
		DirectX::XMFLOAT2 mNewMousePos = DirectX::XMFLOAT2(0.0f, 0.0f);
		DirectX::XMFLOAT2 mOldMousePos = DirectX::XMFLOAT2(0.0f, 0.0f);
	};
}