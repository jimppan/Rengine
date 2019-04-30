#include "PCH.h"
#include "Time.h"

namespace Rengine
{
	float Time::m_flDeltaTime = 0.0f;
	float Time::m_flApplicationTime = 0.0f;
	float Time::m_flFPS = 0;

	float Time::m_FrameTimes[NUM_SAMPLES];
	int   Time::m_iCurrentFrame = 0;
	float Time::m_flPrevTime = 0.0f;
	int   Time::m_iTickCount = 0;
}