#ifndef RENGINE_TILE_COLLISION_H
#define RENGINE_TILE_COLLISION_H

namespace Rengine
{
	enum TileCollisionType : unsigned char
	{
		CLEAR = 0,            // Entire tile is traversable              
		RECT,                 // Uses m_CollisionRect for collision

		LEFT,                 // Left side is blocked from current tile
		RIGHT,                // Right side is blocked from current tile
		TOP,                  // Top is blocked from current tile
		BOTTOM,               // Bottom is blocked from current tile

		DIAGONAL_BL_TR,       // Diagonal bottom left to top right is blocked (Can traverse on both sides of the tile)
		DIAGONAL_BR_TL,       // Diagonal bottom right to top left is blocked (Can traverse on both sides of the tile)

		DIAGONAL_BL,          // Bottom left is blocked
		DIAGONAL_BR,          // Bottom right is blocked
		DIAGONAL_TR,          // Top right is blocked
		DIAGONAL_TL,          // Top left is blocked

		TIP_BL,               // Bottom left tip is blocked
		TIP_BR,               // Bottom right tip is blocked
		TIP_TR,				  // Top left tip is blocked
		TIP_TL,				  // Top right tip is blocked

		MAX
	};

	class TileCollision
	{
	public:
		TileCollision(TileCollisionType collisionType = TileCollisionType::CLEAR);
		~TileCollision();

		inline unsigned char      GetCollisionType() const                        { return m_CollisionType; }
		void                      SetCollisionType(unsigned char type);

		inline Rect               GetCollisionRect() const { return m_CollisionRect; }
		inline void               SetCollisionRect(const Rect& rect) { m_CollisionRect = rect; }

	private:
		unsigned char m_CollisionType = TileCollisionType::CLEAR;
		Rect m_CollisionRect = { 4.0f, 4.0f, 8.0f, 8.0f };
	};
}

#endif // RENGINE_TILE_COLLISION_H