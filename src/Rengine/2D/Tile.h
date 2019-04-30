#ifndef RENGINE_TILE_H
#define RENGINE_TILE_H

#include "Rengine/2D/TileSheet.h"

namespace Rengine
{
	class TileAnimation;
	class Tile
	{
	public:
		Tile();
		Tile(TileSheet* sheet, Rect rect, Rect uv, ColorRGBA8 color, TileAnimation* anim);
		Tile(const Tile& other);


		~Tile();

		inline void           SetTileSheet(TileSheet* sheet)           { m_TileSheet = sheet; }
		inline void           SetRect(const Rect& rect)                { m_Rect = rect; }
		inline void           SetUV(const Recti& uv)                   { m_UV = uv; }
		inline void           SetColor(const ColorRGBA8& color)        { m_Color = color; }
		inline void           SetAnimation(TileAnimation* anim)        { m_Animation = anim; }

		inline TileSheet*     GetTileSheet()                           { return m_TileSheet; }
		inline Rect*          GetRect()                                { return &m_Rect; }
		inline Recti*         GetUV()                                  { return &m_UV; }
		inline ColorRGBA8*    GetColor()                               { return &m_Color; }
		inline TileAnimation* GetAnimation()                           { return m_Animation; }

		inline Rect           GetNormalizedUV() const                  { return m_TileSheet->GetTexture()->NormalizeUV(m_UV); }
		ColorRGBA8            GetPixel(int x, int y) const;

	private:
		TileSheet* m_TileSheet = nullptr;
	
		Rect m_Rect;
		Recti m_UV;
		ColorRGBA8 m_Color;
		TileAnimation* m_Animation = nullptr;
	};
}

#endif // RENGINE_TILE_H