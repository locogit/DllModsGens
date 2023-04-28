#pragma once
#include <functional>
#include <iostream>


//Timer* timer = new Timer(1.0f);
//timer->GetOnComplete() = function;
//timer->GetOnUpdate() = function2; // function2(float dt)
//timer->Start();

//timer->Update(deltaTime);
// two ways of updating
//timer->GetDeltaTime() = deltaTime;
//timer->Update();

//timer->Reset(); //restart timer

//timer->Stop(); // pause timer, use start to play again

//timer->IsFinished(); // self explanatory

//timer->GetTimescale() = timescale; // multiplier for the timer's speed

class Timer {
	float dur = 0; // seconds
	float timeElasped = 0; // also seconds

	std::function<void()> completeFunction = nullptr;
	std::function<void(float& dt)> updateFunction = nullptr;

	bool run = false;

	bool finish = false;

	float timeScale = 1;

	float timerDeltaTime = 0;

public:
	float& GetDuration() {
		return dur;
	}

	float GetTimeElasped() {
		return timeElasped;
	}

	float& GetTimescale() {
		return timeScale;
	}

	float& GetDeltaTime() {
		return timerDeltaTime;
	}

	bool IsFinished() {
		return finish;
	}

	std::function<void()>& GetOnComplete() {
		return completeFunction;
	}

	std::function<void(float& dt)>& GetOnUpdate() {
		return updateFunction;
	}

	void Start() {
		if (finish) {
			Reset();
			return;
		}
		run = true;
	}

	void Reset() {
		finish = false;
		timeElasped = 0;
		Start();
	}

	void Stop() {
		run = false;
	}

	void Update() {
		if (!run) { return; }

		if (updateFunction != nullptr)
		{
			updateFunction(timerDeltaTime);
		}

		printf("\n[Timer] Delta Time: %f Time Elasped: %f", GetDeltaTime(), GetTimeElasped());

		if (timeElasped < dur) {
			timeElasped += timerDeltaTime * timeScale;
		}
		else {
			timeElasped = dur;
			run = false;
			finish = true;
			printf("\n[Timer] Timer Completed!");
			completeFunction();
		}
	}

	void Update(float dt) {
		timerDeltaTime = dt;
		Update();
	}

	Timer() {
		dur = 0;
		timeElasped = 0;
		finish = false;
	}

	Timer(float duration) {
		dur = duration;
		timeElasped = 0;
		finish = false;
	}

	Timer(float duration, std::function<void()> onComplete) {
		dur = duration;
		timeElasped = 0;
		completeFunction = onComplete;
		finish = false;
	}

	Timer(float duration, std::function<void()> onComplete, std::function<void(float dt)> onUpdate) {
		dur = duration;
		timeElasped = 0;
		completeFunction = onComplete;
		updateFunction = onUpdate;
		finish = false;
	}

	~Timer() {
		dur = 0;
		timeElasped = 0;
		completeFunction = nullptr;
		updateFunction = nullptr;
		run = false;
		finish = true;
	}
};
