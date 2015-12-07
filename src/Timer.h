#ifndef TIMER_H
#define TIMER_H

#include <iostream>
#include <stdio.h>

class Timer {

public:

	Timer(float s, float ct = 0.0f, float st = 0.0f, float tt = 1.0f);

	~Timer();

	void update(float);

	void start() { mIsActive = true; }

	void stop() { mIsActive = false; }

	void reset();

	void setTimer(float s, float ct = 0.0f, float st = 0.0f, float tt = 1.0f);

	bool isActive() { return mIsActive; }

	bool isComplete() { return mIsComplete; }

	float getCurrentTime() { return mCurrentTime; }

private:

	float mSpeed;

	float mCurrentTime;

	float mStartTime;

	float mTargetTime;

	bool mIsActive = false;

	bool mIsComplete = false;
};

#endif // TIMER_H