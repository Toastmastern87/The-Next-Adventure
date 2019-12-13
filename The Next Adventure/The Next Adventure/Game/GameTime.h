#pragma once

#pragma comment(lib, "winmm.lib")

#include <Windows.h>
#include <string>
//#include <mmsystem.h>
//#include <fstream>

namespace Toast
{
	class GameTime
	{
	public:
		GameTime();
		~GameTime();

		void Update();
		int GetGameTimeMS() { return mGameTimeMS; }
		int GetGameTimeSecs() { return mGameTimeSecs; }
		int GetGameTimeMins() { return mGameTimeMins; }
		int GetGameTimeHours() { return mGameTimeHours; }
		int GetGameTimeSols() { return mGameTimeSols; }
		int GetGameTimeMarsYears() { return mGameTimeMarsYears; }
		int GetGameSpeed() { return mSpeed; }
		void IncreaseGameSpeed();
		void DecreaseGameSpeed();
		void SetGameSpeed(float);
		std::string GetTimeString();

	private:
		int mGameTimeMS, mGameTimeSecs, mGameTimeMins, mGameTimeHours, mGameTimeSols, mGameTimeMarsYears, mSpeed;
		unsigned long mOldTime;
	};
}
