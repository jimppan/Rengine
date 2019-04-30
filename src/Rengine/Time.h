#ifndef RENGINE_TIME_H
#define RENGINE_TIME_H

#include "PCH.h"

namespace Rengine
{
	class Time
	{
	public:
		static float m_flDeltaTime;
		static float m_flApplicationTime;
		static int   m_iTickCount; // Milliseconds
		static float m_flFPS;

		static const int NUM_SAMPLES = 50;
		static float m_FrameTimes[NUM_SAMPLES];
	private:
		static int m_iCurrentFrame;
		static float m_flPrevTime;
	public:
		inline static void CalculateTickCount() // Gets time passed in milliseconds
		{
			m_iTickCount = (int)glm::floor(m_flApplicationTime * 1000.0f);
		}

		static void CalculateFPS()
		{
			m_FrameTimes[m_iCurrentFrame % NUM_SAMPLES] = m_flDeltaTime;

			m_flPrevTime = m_flApplicationTime;

			int count = 0;
			m_iCurrentFrame++;

			if (m_iCurrentFrame < NUM_SAMPLES)
				count = m_iCurrentFrame;
			else
				count = NUM_SAMPLES;

			float frameTimeAvg = 0.0f;
			for (int i = 0; i < count; i++)
				frameTimeAvg += m_FrameTimes[i];

			frameTimeAvg /= count;

			if (frameTimeAvg > 0)
			{
				m_flFPS = 1.0f / frameTimeAvg;
			}
		}
	};

	class Timer
	{
	public:
		Timer()
		{
			Start();
		}

		inline void Start() { m_flStart = std::chrono::high_resolution_clock::now(); }

		// Returns milliseconds
		inline float GetMilliseconds() { return (m_flDuration = std::chrono::high_resolution_clock::now() - m_flStart).count() * 1000.0f; }
		inline float GetSeconds() { return (m_flDuration = std::chrono::high_resolution_clock::now() - m_flStart).count(); }

		std::chrono::duration<float> m_flDuration;
		std::chrono::time_point<std::chrono::steady_clock> m_flStart;
	};
}

#endif // RENGINE_TIME_H