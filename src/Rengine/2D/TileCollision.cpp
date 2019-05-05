#include "PCH.h"
#include "TileCollision.h"

namespace Rengine
{
	TileCollision::TileCollision(TileCollisionType collisionType):
		m_CollisionType(collisionType)
	{
	}


	TileCollision::~TileCollision()
	{
	}

	void TileCollision::SetCollisionType(unsigned char type)
	{
		m_CollisionType = type >= TileCollisionType::MAX ? TileCollisionType::CLEAR : type;
	}
}

