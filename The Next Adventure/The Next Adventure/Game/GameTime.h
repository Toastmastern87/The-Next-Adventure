#pragma once

#include "../Engine/System.h"
#include <string>

namespace Toast
{
	class GameTime
	{
	public:
		GameTime();
		~GameTime();

		void Update(float);
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
		int mGameTimeMS = 0, mGameTimeSecs = 0, mGameTimeMins = 0, mGameTimeHours = 0, mGameTimeSols = 0, mGameTimeMarsYears = 0, mSpeed = 0;
	};
}
