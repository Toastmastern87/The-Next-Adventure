#include "System.h"

static LRESULT CALLBACK WndProc(HWND, UINT, WPARAM, LPARAM);

#pragma warning( disable : 4996)

namespace Toast {
	FILE* System::logFile = nullptr;
	System* System::tSys = nullptr;

	void Toast::System::Run()
	{
		MSG msg;
		bool result;

		mD3D = new Toast::D3D();
		mTheNextFrontier = new Toast::Game();
		mResources = new Toast::Resources();

		result = mD3D->Init();
		if (!result)
		{
			MessageBox(mWindow, "Couldn't initiate the D3D object", "Error", MB_OK);
		}

		mTheNextFrontier->Init();

		Timer timer;
		timer.Init();

		double trackerTime = 0;
		int numSamples = 0;
		double frameTimeAdd = 0, tickTimeAdd = 0;

		ZeroMemory(&msg, sizeof(MSG));

		while (!tSys->mDone)
		{
			double deltaTime = timer.GetDeltaTimeReset();
			trackerTime += deltaTime;

			while (PeekMessage(&msg, NULL, 0, 0, PM_REMOVE))
			{
				TranslateMessage(&msg);
				DispatchMessage(&msg);

				if (msg.message == WM_QUIT)
				{
					tSys->mDone = true;
				}

				if (msg.message == WM_LBUTTONUP)
				{
					tSys->mTheNextFrontier->mInput->mMouse[0] = -1;
				}

				if (msg.message == WM_LBUTTONDOWN)
				{
					if (tSys->mTheNextFrontier->mInput->mMouse[0] <= 0)
					{
						tSys->mTheNextFrontier->mInput->mMouse[0] = 2;
					}
				}

				if (msg.message == WM_RBUTTONUP)
				{
					tSys->mTheNextFrontier->mInput->mMouse[1] = -1;
				}

				if (msg.message == WM_RBUTTONDOWN)
				{
					if (tSys->mTheNextFrontier->mInput->mMouse[1] <= 0)
					{
						tSys->mTheNextFrontier->mInput->mMouse[1] = 2;
					}
				}

				if (msg.message == WM_MOUSEWHEEL) 
				{
					int16_t delta = GET_WHEEL_DELTA_WPARAM(msg.wParam);

					if (delta < 0)
					{
						tSys->mTheNextFrontier->mInput->mMouse[4] = 1;
					}
					else if (delta > 0)
					{
						tSys->mTheNextFrontier->mInput->mMouse[3] = 1;
					}
				}

				if (msg.message == WM_KEYDOWN) 
				{
					if (msg.wParam > 256) 
					{
						continue;
					}

					if (tSys->mTheNextFrontier->mInput->mKeys[msg.wParam] <= 0) 
					{
						tSys->mTheNextFrontier->mInput->mKeys[msg.wParam] = 2;
					}
				}

				if (msg.message == WM_KEYUP)
				{
					if (msg.wParam > 256)
					{
						continue;
					}

					tSys->mTheNextFrontier->mInput->mKeys[msg.wParam] = -1;
				}

				//Get mouse location
				GetCursorPos(&tSys->mTheNextFrontier->mInput->mCursorPos);
			}

			mTheNextFrontier->Update(deltaTime);

			//Keyboard
			for (size_t i = 0; i < 256; i++)
			{
				int8_t currentKey = tSys->mTheNextFrontier->mInput->mKeys[i];

				if (currentKey == 2) //pressed
				{
					tSys->mTheNextFrontier->mInput->mKeys[i] = 1;
				}
				else if (currentKey == -1) //released
				{
					tSys->mTheNextFrontier->mInput->mKeys[i] = 0;
				}
			}

			//Mouse
			for (size_t i = 0; i < 2; i++)
			{
				int8_t currentButton = tSys->mTheNextFrontier->mInput->mMouse[i];

				if (currentButton == 2) //pressed
				{
					tSys->mTheNextFrontier->mInput->mMouse[i] = 1;
				}
				else if (currentButton == -1) //released
				{
					tSys->mTheNextFrontier->mInput->mMouse[i] = 0;
				}
			}

			// Resets the scroll wheel
			tSys->mTheNextFrontier->mInput->mMouse[3] = 0;
			tSys->mTheNextFrontier->mInput->mMouse[4] = 0;

			numSamples++;
			double tDelta = timer.GetDeltaTime();

			mD3D->Draw(*mTheNextFrontier);

			double frameDelta = (timer.GetDeltaTime() - tDelta);
			frameTimeAdd += frameDelta;
			tickTimeAdd += deltaTime;

			if (trackerTime >= 1.0)
			{
				mFPS = (int)(trackerTime / (tickTimeAdd / (double)numSamples));

				System::Print("Avg FPS: %3i  Avg Frametime: %.4f   Avg Tick Time: %.4f", (int)(trackerTime / (tickTimeAdd / (double)numSamples)), frameTimeAdd / (double)numSamples, tickTimeAdd - frameTimeAdd);
				trackerTime = 0;
				frameTimeAdd = 0;
				tickTimeAdd = 0;
				numSamples = 0;
			}
		}

		ShowCursor(true);

		return;
	}
}

LRESULT CALLBACK WndProc(HWND hwnd, UINT umessage, WPARAM wparam, LPARAM lparam)
{
	switch (umessage)
	{
	case WM_DESTROY:
	{
		PostQuitMessage(0);
		return 0;
	}

	case WM_CLOSE:
	{
		PostQuitMessage(0);
		return 0;
	}
	}

	return DefWindowProc(hwnd, umessage, wparam, lparam);
}

int WINAPI WinMain(HINSTANCE hInstance, HINSTANCE hPrevInstance, LPSTR lpCmdLine, int nCmdShow)
{
	WNDCLASSEX wc;
	int screenWidth, screenHeight;

	Toast::System::logFile = fopen("log.txt", "w+");
	Toast::System::tSys = new Toast::System();
	Toast::System* tSys = Toast::System::tSys;

	// Reads the configuration file
	std::ifstream configFile("config.ini");
	std::string line;

	tSys->mHInstance = GetModuleHandle(NULL);

	if (configFile.is_open())
	{
		while (std::getline(configFile, line))
		{
			size_t cIndex = line.find(" ", 0);
			if (cIndex != std::string::npos)
			{
				tSys->mSettings[line.substr(0, cIndex)] = std::stof(line.substr(cIndex + 1, line.size() - (cIndex + 1)));
			}
		}

		configFile.close();
	}

	wc.style = CS_HREDRAW | CS_VREDRAW | CS_OWNDC;
	wc.lpfnWndProc = WndProc;
	wc.cbClsExtra = 0;
	wc.cbWndExtra = 0;
	wc.hInstance = tSys->mHInstance;
	wc.hIcon = LoadIcon(NULL, IDI_WINLOGO);
	wc.hIconSm = wc.hIcon;
	wc.hCursor = LoadCursor(NULL, IDC_ARROW);
	wc.hbrBackground = (HBRUSH)GetStockObject(BLACK_BRUSH);
	wc.lpszMenuName = NULL;
	wc.lpszClassName = "Mars - The Next Frontier";
	wc.cbSize = sizeof(WNDCLASSEX);

	RegisterClassEx(&wc);

	screenWidth = GetSystemMetrics(SM_CXSCREEN);
	screenHeight = GetSystemMetrics(SM_CYSCREEN);

	if (tSys->mSettings.find("Fullscreen")->second == 1)
	{
		HWND desktop = GetDesktopWindow();
		RECT dSize;
		GetWindowRect(desktop, &dSize);

		tSys->mSettings.find("WindowSizeX")->second = static_cast<float>(dSize.right);
		tSys->mSettings.find("WindowSizeY")->second = static_cast<float>(dSize.bottom);

		tSys->mWindow = CreateWindowEx(NULL, "Mars - The Next Frontier", "Mars - The Next Frontier", WS_POPUP, dSize.left, dSize.top, dSize.right, dSize.bottom, NULL, NULL, tSys->mHInstance, NULL);
	}
	else
	{
		tSys->mWindow = CreateWindowEx(NULL, "Mars - The Next Frontier", "Mars - The Next Frontier", WS_OVERLAPPEDWINDOW, static_cast<int>(tSys->mSettings.find("WindowPosX")->second), static_cast<int>(tSys->mSettings.find("WindowPosY")->second), static_cast<int>(tSys->mSettings.find("WindowSizeX")->second), static_cast<int>(tSys->mSettings.find("WindowSizeY")->second), NULL, NULL, tSys->mHInstance, NULL);
	}

	ShowWindow(tSys->mWindow, nCmdShow);
	SetForegroundWindow(tSys->mWindow);
	SetFocus(tSys->mWindow);

	ShowCursor(false);

	tSys->Run();

	fclose(Toast::System::logFile);

	delete tSys;
	return 0;
}

void Toast::System::Print(const char* message, ...)
{
	size_t strLength = strlen(message);
	int fmtMsgSize = static_cast<int>(strLength < 128 ? 256 : strLength * 2);
	char* buffer = new char[fmtMsgSize];
	memset(buffer, 0, fmtMsgSize);
	std::string timestamp;
	char* msg = new char[fmtMsgSize];
	timeb t;
	ftime(&t);
	strftime(msg, fmtMsgSize, "[%T", localtime(&t.time));
	timestamp.insert(0, msg);
	timestamp.append(":");
	memset(msg, 0, 4);
	short msVal = t.millitm;
	timestamp.append(itoa(t.millitm, msg, 10));
	timestamp.append(msVal < 10 ? "00] " : (msVal < 100 ? "0] " : "] "));
	va_list args;
	va_start(args, message);
	vsnprintf(buffer, fmtMsgSize, message, args);
	//snprintf(buffer, fmtMsgSize, message, args);
	va_end(args);
	timestamp.append(buffer);
	timestamp.append("\n");
	printf("%s", timestamp.c_str());
	delete[] buffer;
	delete[] msg;
	if (logFile)
	{
		fwrite(timestamp.c_str(), timestamp.size(), 1, logFile);
		fflush(logFile);
	}
}