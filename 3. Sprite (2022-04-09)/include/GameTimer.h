#pragma once

#include <Windows.h>
#include <windowsx.h>

class GameTimer {
public:
	GameTimer();
	GameTimer& operator=(const GameTimer&) = delete;
	GameTimer(const GameTimer&) = delete;
	~GameTimer();

	void Tick();
	void Reset();
	void Start();
	void Stop();

	FLOAT getDeltaTime() const;
	FLOAT getTotalTime() const;

private:
	FLOAT m_DeltaTime, m_SecondsPerTick;
	__int64 m_BaseTime, m_StopTime, m_PausedTime, m_CurrTime, m_PrevTime;
	BOOL m_IsStopped;

};