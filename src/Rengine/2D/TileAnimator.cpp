#include "PCH.h"
#include "TileAnimator.h"

#include "Rengine/2D/TileMap.h"
#include "Rengine/Resource/Factory.h"

namespace Rengine
{
	TileAnimator::TileAnimator(const TileAnimCallbackFn& callback):
		m_fnCallback(callback)
	{
	}


	TileAnimator::~TileAnimator()
	{
	}

	void TileAnimator::Update()
	{
		for (auto& it = Factory::TileSheets::Begin(); it != Factory::TileSheets::End(); it++)
		{
			TileSheet* sheet = it->second;
			for (unsigned int i = 0; i < sheet->m_Animations.size(); i++)
			{
				TileAnimation* anim = &sheet->m_Animations[i];
				if (Time::m_iTickCount > anim->GetLastUpdate() + (anim->GetCurrentFrame()->duration))
				{
					anim->NextFrame(Time::m_iTickCount);
					m_fnCallback(anim);
				}
			}
		}
	}
}

