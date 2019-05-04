#ifndef RENGINE_TILE_COLLISION_MAP_H
#define RENGINE_TILE_COLLISION_MAP_H

#include "Rengine/2D/TileCollision.h"

namespace Rengine
{
	class TileCollisionMap
	{
	public:
		TileCollisionMap(int width, int height);
		~TileCollisionMap();

		TileCollision* GetTile(int x, int y);
		TileCollision* GetTile(int id);

		inline int     GetWidth() const             { return m_iWidth; }
		inline int     GetHeight() const            { return m_iHeight; }

		inline void    SetWidth(const int& width)   { m_iWidth = width; }
		inline void    SetHeight(const int& height) { m_iHeight = height; }

	private:
		std::vector<TileCollision> m_CollisionTiles;

		int m_iWidth = 0;
		int m_iHeight = 0;
	};
}

#endif // RENGINE_TILE_COLLISION_MAP_H