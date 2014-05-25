#pragma once

class MDTimer
{
private:
	static LARGE_INTEGER   m_startTime;
	static LARGE_INTEGER   m_ticksPerSecond;
public:
	static float delta;
private:
	MDTimer() {}
public:
	
	static void Init()
	{
		if (QueryPerformanceFrequency(&m_ticksPerSecond))
		{
			QueryPerformanceCounter(&m_startTime);
		}
		// first time update is necessery
		Update();
	}
	static float GetDelta(unsigned long elapsedFrames = 1)
	{
		static LARGE_INTEGER s_lastTime = m_startTime;
		LARGE_INTEGER currentTime;
		QueryPerformanceCounter(&currentTime);
		float seconds =  ((float)currentTime.QuadPart - (float)s_lastTime.QuadPart) / (float)m_ticksPerSecond.QuadPart;
		s_lastTime = currentTime;
		return seconds;
	}
	static float GetFPS(unsigned long elapsedFrames = 1)
	{
		static LARGE_INTEGER s_lastTime = m_startTime;
		LARGE_INTEGER currentTime;
		QueryPerformanceCounter(&currentTime);
		float fps = (float)elapsedFrames * (float)m_ticksPerSecond.QuadPart / ((float)currentTime.QuadPart - (float)s_lastTime.QuadPart);
		s_lastTime = currentTime;
		return fps;
	}
	static void Update()
	{
		static MDTimer timer;
		delta = timer.GetDelta();
	}
};
float MDTimer::delta = 0;
LARGE_INTEGER MDTimer::m_startTime;
LARGE_INTEGER MDTimer::m_ticksPerSecond;