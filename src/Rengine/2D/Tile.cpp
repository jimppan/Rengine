#include "PCH.h"
#include "Tile.h"

namespace Rengine
{
	Tile::Tile()
	{
	}

	Tile::Tile(
		TileSheet* sheet = nullptr, 
		Rect rect = { 0.0f, 0.0f, 1.0f, 1.0f }, 
		Rect uv = { 0.0f, 0.0f, 1.0f, 1.0f }, 
		ColorRGBA8 color = { 255, 255, 255, 255 },
		TileAnimation* anim = nullptr) :
		m_TileSheet(sheet),
		m_Rect(rect),
		m_UV(uv),
		m_Color(color),
		m_Animation(anim)
	{

	}

	Tile::Tile(const Tile& other):
		m_TileSheet(other.m_TileSheet),
		m_Rect(other.m_Rect),
		m_UV(other.m_UV),
		m_Color(other.m_Color),
		m_Animation(other.m_Animation)
	{

	}

	Tile::~Tile()
	{
	}

	ColorRGBA8 Tile::GetPixel(int x, int y) const
	{
		RENGINE_CORE_ASSERT(m_TileSheet->GetTexture(), "Invalid tile texture");
		return m_TileSheet->GetTexture()->GetPixelRGBA(m_UV.x + x, m_UV.y + y);
	}
}
