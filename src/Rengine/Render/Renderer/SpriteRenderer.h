#ifndef RENGINE_SPRITE_RENDERER_H
#define RENGINE_SPRITE_RENDERER_H

#include "Rengine/Render/RenderAPI/RAPIBuffers.h"
#include "Rengine/Render/RenderAPI/RAPIShader.h"
#include "Rengine/Render/Sprite.h"
#include "Rengine/Render/Camera.h"
#include "Rengine/Render/Renderer/Renderer.h"

namespace Rengine
{
	class SpriteBatch
	{
	public:
		SpriteBatch(int offset, int vertCount, Texture* texture) :
			m_iOffset(offset),
			m_iNumVertices(vertCount),
			m_Texture(texture)
		{

		}

		unsigned int m_iOffset;
		unsigned int m_iNumVertices;
		Texture* m_Texture;
	};

	class SpriteRenderer
	{
	public:
		SpriteRenderer();
		~SpriteRenderer();

		void                 Initialize();
				             
		void                 Begin(GlyphSortType sortType = GlyphSortType::TEXTURE);
		void                 End();
				             
		void                 Draw(Sprite* sprite, const ColorRGBA8& color = {255, 255, 255, 255});
		void                 Draw(const Rect& rect, const Rect& uv, const ColorRGBA8& color, Texture* texture, const float& depth = 0.0f);
				             
		inline void          Render(Camera* cam) { Render(cam, ColorRGBA8{ 255, 255, 255, 255 }); }
		void                 Render(Camera* cam, ColorRGBA8 color);
		void                 Render(Camera* cam, ColorRGBA32 color);

		inline RAPI::Shader* GetShader() { return m_Shader; }
		inline void          SetShader(RAPI::Shader* shader) { m_Shader = shader; }
	private:
		void                 SortGlyphs();
		void                 GenerateSpriteBatches();
	public:
		std::vector<SpriteBatch> m_SpriteBatches;
	private:
		RAPI::VertexBuffer m_VBO;
		RAPI::VertexArray m_VAO;

		GlyphSortType m_eGlyphSortType;

		std::vector<SpriteGlyph*> m_GlyphPointers; // Only used for sorting
		std::vector<SpriteGlyph> m_Glyphs;

		RAPI::Shader* m_Shader = nullptr;
	};
}
#endif // RENGINE_SPRITE_RENDERER_H

