#include "GameTime.h"

namespace Toast
{
	GameTime::GameTime()
	{
		mGameTimeMS = 0;
		mGameTimeSecs = 0;
		mGameTimeMins = 0;
		mGameTimeHours = 0;
		mGameTimeSols = 0;
		mGameTimeMarsYears = 0;

		mSpeed = 1;

		mOldTime = timeGetTime();
	}

	GameTime::~GameTime()
	{
	}

	void GameTime::Update()
	{
		mGameTimeMS += (timeGetTime() - mOldTime) * mSpeed;

		mOldTime = timeGetTime();

		if (mGameTimeMS >= 1000)
		{
			int numberOfSecs = mGameTimeMS / 1000;

			mGameTimeMS -= (numberOfSecs * 1000);

			mGameTimeSecs += numberOfSecs;
		}

		if (mGameTimeSecs >= 60)
		{
			mGameTimeSecs -= 60;

			mGameTimeMins++;
		}

		if (mGameTimeMins >= 60)
		{
			mGameTimeMins -= 60;

			mGameTimeHours++;
		}

		if ((mGameTimeHours >= 24) && (mGameTimeMins >= 35) && (mGameTimeSecs >= 39))
		{
			mGameTimeHours -= 24;
			mGameTimeMins -= 35;
			mGameTimeSecs -= 39;

			mGameTimeSols++;
		}

		if (mGameTimeSols >= 687)
		{
			mGameTimeSols -= 687;

			mGameTimeMarsYears++;
		}
	}

	void GameTime::IncreaseGameSpeed()
	{
		if (mSpeed <= 100)
		{
			mSpeed *= 10;
		}
	}

	void GameTime::DecreaseGameSpeed()
	{
		if (mSpeed > 1)
		{
			mSpeed /= 10;
		}
	}

	void GameTime::SetGameSpeed(float speed)
	{
		mSpeed = speed;
	}

	std::string GameTime::GetTimeString() 
	{
		std::string timeString, hoursString, minsString, secsString;

		if (mGameTimeHours < 10)
		{
			hoursString = "0" + std::to_string(mGameTimeHours);
		}
		else 
		{
			hoursString = std::to_string(mGameTimeHours);
		}

		if (mGameTimeMins < 10)
		{
			minsString = "0" + std::to_string(mGameTimeMins);
		}
		else
		{
			minsString = std::to_string(mGameTimeMins);
		}

		if (mGameTimeSecs < 10)
		{
			secsString = "0" + std::to_string(mGameTimeSecs);
		}
		else
		{
			secsString = std::to_string(mGameTimeSecs);
		}

		timeString = hoursString + ":" + minsString + ":" + secsString;

		return timeString;
	}
}