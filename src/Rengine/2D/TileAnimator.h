#ifndef RENGINE_TILE_ANIMATOR_H
#define RENGINE_TILE_ANIMATOR_H

#include "Rengine/2D/TileSheet.h"

namespace Rengine
{
	class CPlayer;
	class TileMap;
	class TileAnimator
	{
	public:
		using TileAnimCallbackFn = std::function<void(TileAnimation*)>;

		TileAnimator(const TileAnimCallbackFn& callback);
		~TileAnimator();

		inline void SetCallback(const TileAnimCallbackFn& callback) { m_fnCallback = callback; }

		void Update();
	private:
		TileAnimCallbackFn m_fnCallback;
		//void Update(Camera* camera);
	};
}

#endif // RENGINE_TILE_ANIMATOR_H