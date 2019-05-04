#ifndef RENGINE_TILE_COLLISION_H
#define RENGINE_TILE_COLLISION_H

namespace Rengine
{
	enum class TileCollisionType : unsigned char
	{
		CLEAR = 0,            // Entire tile is traversable              
		BLOCKED,              // Entire tile is blocked

		HALF_LEFT,            // Left half is blocked
		HALF_RIGHT,           // Right half is blocked
		HALF_TOP,             // Top half is blocked
		HALF_BOTTOM,          // Bottom half is blocked

		DIAGONAL_BL,          // Bottom left is blocked
		DIAGONAL_BR,          // Bottom right is blocked
		DIAGONAL_TR,          // Top right is blocked
		DIAGONAL_TL,          // Top left is blocked

		TIP_BL,               // Bottom left tip is blocked
		TIP_BR,               // Bottom right tip is blocked
		TIP_TR,				  // Top left tip is blocked
		TIP_TL,				  // Top right tip is blocked

		LEFT,                 // Left side is blocked from current tile
		RIGHT,                // Right side is blocked from current tile
		TOP,                  // Top is blocked from current tile
		BOTTOM,               // Bottom is blocked from current tile

		MIDDLE_H,             // Middle is blocked horizontally               (Can traverse on both sides of the tile)
		MIDDLE_V,             // Middle is blocked vertically                 (Can traverse on both sides of the tile)

		DIAGONAL_BL_TR,       // Diagonal bottom left to top right is blocked (Can traverse on both sides of the tile)
		DIAGONAL_BR_TL        // Diagonal bottom right to top left is blocked (Can traverse on both sides of the tile)
	};

	class TileCollision
	{
	public:
		TileCollision(const TileCollisionType& collisionType = TileCollisionType::CLEAR);
		~TileCollision();

		inline TileCollisionType  GetCollisionType() const                        { return m_CollisionType; }
		inline void               SetCollisionType(const TileCollisionType& type) { m_CollisionType = type; }

	private:
		TileCollisionType m_CollisionType = TileCollisionType::CLEAR;
	};
}

#endif // RENGINE_TILE_COLLISION_H