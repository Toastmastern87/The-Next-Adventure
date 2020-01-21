#pragma once

#include <string>
#include "System.h"

namespace Toast
{
	class Time
	{
	public:
		Time();
		~Time();

		void Update(double);
		int GetGameTimeSecs() { return static_cast<int>(mGameTimeSecs); }
		int GetGameTimeMins() { return mGameTimeMins; }
		int GetGameTimeHours() { return mGameTimeHours; }
		int GetGameTimeSols() { return mGameTimeSols; }
		int GetGameTimeMarsYears() { return mGameTimeMarsYears; }
		float GetGameSpeed() { return mSpeed; }
		void IncreaseGameSpeed();
		void DecreaseGameSpeed();
		void SetGameSpeed(float);
		std::string GetTimeString();

	private:
		float mSpeed = 0.0f, mGameTimeSecs = 0.0f;
		int mGameTimeMins = 0, mGameTimeHours = 0, mGameTimeSols = 0, mGameTimeMarsYears = 0;
	};
}