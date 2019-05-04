#include "PCH.h"
#include "TileCollisionMap.h"

namespace Rengine
{
	TileCollisionMap::TileCollisionMap(int width, int height):
		m_iWidth(width),
		m_iHeight(height)
	{
		m_CollisionTiles.resize(width * height);
	}


	TileCollisionMap::~TileCollisionMap()
	{
	}

	TileCollision* TileCollisionMap::GetTile(int x, int y)
	{
		if (x >= m_iWidth || x < 0 || y >= m_iHeight || y < 0)
			return nullptr;

		int id = y * m_iWidth + x;
		return GetTile(id);
	}

	TileCollision* TileCollisionMap::GetTile(int id)
	{
		if (id < 0 || id >= (int)m_CollisionTiles.size())
			return nullptr;

		return &m_CollisionTiles[id];
	}
}
