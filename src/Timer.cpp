#include "Timer.h"

Timer::Timer(float s, float ct, float st, float tt)
	: mSpeed(s), mCurrentTime(ct), mStartTime(st), mTargetTime(tt) { }

void Timer::update(float dt) {

	if(mSpeed >= 0.0f && mCurrentTime < mTargetTime)
		mCurrentTime += dt * mSpeed;
	else if (mSpeed <= 0.0f && mCurrentTime > mTargetTime)
		mCurrentTime += dt * mSpeed;
	else {
		reset();
		mIsComplete = true;
	}
}

void Timer::reset() {

	float mCurrentTime = 0.0f;

	bool mIsActive = false;
}

void Timer::setTimer(float s, float ct, float st, float tt) {
	mSpeed = s;
	mCurrentTime = ct;
	mStartTime = st;
	mTargetTime = tt;
}