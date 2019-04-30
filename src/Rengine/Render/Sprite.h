#ifndef RENGINE_SPRITE_H
#define RENGINE_SPRITE_H

#include "Rengine/Render/Texture.h"

namespace Rengine
{
	class Sprite
	{
	public:
		Sprite();
		~Sprite();

		inline void     SetTexture(Texture* texture)              { m_Texture = texture; }
		inline void     SetRect(const Rect& rect)                 { m_Rect = rect; }
		inline void     SetUV(const Rect& uv)                     { m_UV = uv; }

		inline Texture* GetTexture()                              { return m_Texture; }
		inline Rect*    GetRect()                                 { return &m_Rect; }
		inline Rect*    GetUV()                                   { return &m_UV; }

		inline float    GetDepth() const                          { return m_flDepth; }
		inline void     SetDepth(const float& depth)              { m_flDepth = depth; }
	private:
		Texture* m_Texture;
		Rect m_Rect;
		Rect m_UV;

		float m_flDepth;
	};
}


#endif // RENGINE_SPRITE_H
