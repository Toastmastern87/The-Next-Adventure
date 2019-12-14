#pragma once

#include <Windows.h>
#include <fstream>
#include <map>
#include <string>
#include "D3D.h"
#include <time.h>
#include <array>
#include <sys/timeb.h>
#include "../Game/Game.h"

#define CLEAN(x) if(x){x->Release();x=nullptr;}

namespace Toast {
	struct Timer
	{
	public:
		Timer() 
		{
			Init();
		}

		void Init() 
		{
			QueryPerformanceFrequency(&mFreq);
			QueryPerformanceCounter(&mOldT);
			QueryPerformanceCounter(&mNewT);
		}

		void StartTime() 
		{
			QueryPerformanceCounter(&mNewT);
		}

		double GetDeltaTimeMicro()
		{
			LARGE_INTEGER temp;
			QueryPerformanceCounter(&temp);
			return  static_cast<double>((temp.QuadPart - mOldT.QuadPart) / (mFreq.QuadPart / 1000000));
		}

		double GetDeltaTimeMS()
		{
			LARGE_INTEGER temp;
			QueryPerformanceCounter(&temp);
			return static_cast<double>((temp.QuadPart - mOldT.QuadPart) / (mFreq.QuadPart / 1000));
		}

		double GetDeltaTimeMicroReset()
		{
			QueryPerformanceCounter(&mNewT);
			double delta = static_cast<double>((mNewT.QuadPart - mOldT.QuadPart) / (mFreq.QuadPart / 1000000));
			mOldT = mNewT;
		
			return delta;
		}

		double GetDeltaTimeMSReset()
		{
			QueryPerformanceCounter(&mNewT);
			double delta = static_cast<double>((mNewT.QuadPart - mOldT.QuadPart) / (mFreq.QuadPart / 1000));
			mOldT = mNewT;
			return delta;
		}

		double GetDeltaTimeReset()
		{
			return  static_cast<double>(GetDeltaTimeMicroReset() / 1000000.0);
		}

		double GetDeltaTime()
		{
			return  static_cast<double>(GetDeltaTimeMicro() / 1000000.0);
		}

	private:
		LARGE_INTEGER mOldT, mNewT;
		LARGE_INTEGER mFreq;
	};

	class D3D;
	class Game;
	class Resources;

	class System
	{
	public:
		static FILE* logFile;
		static void Print(const char* message, ...);

		static Toast::System* tSys;

		System() {};
		void Run();

		Toast::D3D *mD3D = nullptr;
		Toast::Game *mTheNextFrontier = nullptr;
		Toast::Resources *mResources = nullptr;
		HWND mWindow = nullptr;
		HINSTANCE mHInstance = nullptr;
		bool mDone = false;
		int mFPS = 0;
		std::map<std::string, float> mSettings;
	};
};