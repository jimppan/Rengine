#include "PCH.h"
#include "Sprite.h"

namespace Rengine
{
	Sprite::Sprite():
		m_Rect(0.0f, 0.0f, 1.0f, 1.0f),
		m_UV(0.0f, 0.0f, 1.0f, 1.0f),
		m_flDepth(0.0f)
	{
	}


	Sprite::~Sprite()
	{
	}
}
