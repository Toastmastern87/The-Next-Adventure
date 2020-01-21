#include "Time.h"

namespace Toast
{
	Time::Time()
	{
		mGameTimeSecs = 0;
		mGameTimeMins = 0;
		mGameTimeHours = 0;
		mGameTimeSols = 0;
		mGameTimeMarsYears = 0;

		mSpeed = 1.0f;
	}

	Time::~Time()
	{
	}

	void Time::Update(double deltaTime)
	{
		mGameTimeSecs += deltaTime * mSpeed;

		//Toast::System::tSys->Print("mGameTimeMS: %f", mGameTimeMS);

		if (mGameTimeSecs >= 60.0f)
		{
			mGameTimeSecs -= 60.0f;

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

	void Time::IncreaseGameSpeed()
	{
		if (mSpeed <= 100.0f)
		{
			mSpeed *= 10.0f;
		}
	}

	void Time::DecreaseGameSpeed()
	{
		if (mSpeed > 1.0f)
		{
			mSpeed /= 10.0f;
		}
	}

	void Time::SetGameSpeed(float speed)
	{
		mSpeed = speed;
	}

	std::string Time::GetTimeString()
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
			secsString = "0" + std::to_string(static_cast<int>(mGameTimeSecs));
		}
		else
		{
			secsString = std::to_string(static_cast<int>(mGameTimeSecs));
		}

		timeString = hoursString + ":" + minsString + ":" + secsString;

		return timeString;
	}
}