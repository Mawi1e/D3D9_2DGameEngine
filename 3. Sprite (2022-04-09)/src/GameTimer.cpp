#include "GameTimer.h"

GameTimer::GameTimer() :
	m_DeltaTime(-1.0f), m_BaseTime(0), m_StopTime(0), m_PausedTime(0), m_CurrTime(0), m_PrevTime(0), m_IsStopped(false) {

	__int64 Frequency;
	QueryPerformanceFrequency((LARGE_INTEGER*)&Frequency);

	m_SecondsPerTick = 1.0f / (float)Frequency;
}

GameTimer::~GameTimer() {
}


void GameTimer::Tick() {
	__int64 currTime;

	if (m_IsStopped) {
		m_DeltaTime = 0.0f;
		return;
	}

	QueryPerformanceCounter((LARGE_INTEGER*)&currTime);
	m_CurrTime = currTime;
	m_DeltaTime = (m_CurrTime - m_PrevTime) * m_SecondsPerTick;
	m_PrevTime = currTime;

	if (m_DeltaTime < 0.0f) {
		m_DeltaTime = 0.0f;
	}
}

void GameTimer::Reset() {
	__int64 currTime;

	QueryPerformanceCounter((LARGE_INTEGER*)&currTime);
	m_BaseTime = m_PrevTime = currTime;
	m_StopTime = 0;
	m_IsStopped = false;
}

void GameTimer::Start() {
	if (m_IsStopped) {
		__int64 currTime;

		QueryPerformanceCounter((LARGE_INTEGER*)&currTime);
		m_PausedTime += (currTime - m_StopTime);
		m_StopTime = 0;
		m_PrevTime = currTime;

		m_IsStopped = false;
	}
}

void GameTimer::Stop() {
	if (!m_IsStopped) {
		__int64 currTime;

		QueryPerformanceCounter((LARGE_INTEGER*)&currTime);
		m_StopTime = currTime;

		m_IsStopped = true;
	}
}

FLOAT GameTimer::getDeltaTime() const {
	return m_DeltaTime;
}

FLOAT GameTimer::getTotalTime() const {
	if (m_IsStopped) {
		return ((m_StopTime - m_PausedTime) - m_BaseTime) * m_SecondsPerTick;
	}
	
	return ((m_CurrTime - m_PausedTime) - m_BaseTime) * m_SecondsPerTick;
}

