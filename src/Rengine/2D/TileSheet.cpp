#include "PCH.h"
#include "TileSheet.h"

namespace Rengine
{
	TileSheet::TileSheet()
	{

	}

	TileSheet::~TileSheet()
	{

	}

	// X from left
	// Y from bottom
	Recti TileSheet::GetTileRect(int x, int y)
	{
		Recti rect;
		rect.x = x * GetTileWidth();
		rect.y = y * GetTileHeight();
		rect.z = GetTileWidth();
		rect.w = GetTileHeight();
		return rect;
	}

	// Starts counting from top to down
	Recti TileSheet::GetTileRect(int rid)
	{
		Vector2i coord = IndexToCoordinate(rid);
		return GetTileRect(coord.x, coord.y);
	}

	Vector2i TileSheet::IndexToCoordinate(int id)
	{
		Vector2i ret;
		int ogY = (int)glm::floor(id / GetTileCountX());
		ret.y = glm::abs(ogY - GetTileCountY()) - 1;
		ret.x = id - ogY * GetTileCountX();
		return ret;
	}

	void TileSheet::AddTileAnimation(const TileAnimation& animation)
	{
		m_Animations.push_back(TileAnimation(animation));
	}

	TileAnimation* TileSheet::GetTileAnimation(int rid)
	{
		for (unsigned int i = 0; i < m_Animations.size(); i++)
		{
			if (m_Animations[i].GetStartTileID() == rid)
				return &m_Animations[i];
		}
		return nullptr;
	}

	Recti TileSheet::GetTileAnimationRect(int rid)
	{
		TileAnimation* anim = GetTileAnimation(rid);
		if (!anim)
			return Recti(0.0f, 0.0f, 1.0f, 1.0f);

		tile_animation_frame_t* frame = anim->GetCurrentFrame();

		return GetTileRect(frame->tile_id);
	}
	
	int TileAnimation::NextFrame(int currentTick)
	{
		m_iCurrentFrame += m_iFramesToSkip;
		if (m_bBounce)
		{
			if (m_iCurrentFrame >= (int)m_Frames.size())
			{
				m_iCurrentFrame--;
				m_iFramesToSkip = -m_iFramesToSkip;
			}
			else if (m_iCurrentFrame < 0)
			{
				m_iCurrentFrame++;
				m_iFramesToSkip = -m_iFramesToSkip;
			}
		}
		else
		{
			if (m_iCurrentFrame >= (int)m_Frames.size())
				m_iCurrentFrame = 0;
		}
		m_iLastUpdate = currentTick;
		return m_iCurrentFrame;
	}
}