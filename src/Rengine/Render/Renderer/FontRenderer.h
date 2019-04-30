#ifndef RENGINE_FONT_RENDERER_H
#define RENGINE_FONT_RENDERER_H

#include "Rengine/Render/RenderAPI/RAPIBuffers.h"
#include "Rengine/Render/RenderAPI/RAPIShader.h"
#include "Rengine/Render/Font.h"
#include "Rengine/Render/Camera.h"
#include "Rengine/Render/Renderer/Renderer.h"


namespace Rengine
{
	class FontBatch
	{
	public:
		FontBatch(int offset, int vertCount, Texture* texture) :
			m_iOffset(offset),
			m_iNumVertices(vertCount),
			m_Texture(texture)
		{

		}

		unsigned int m_iOffset;
		unsigned int m_iNumVertices;
		Texture* m_Texture;
	};

	class FontRenderer
	{
	public:
		FontRenderer();
		~FontRenderer();

		void                 Initialize();

		void                 Begin(GlyphSortType sortType = GlyphSortType::TEXTURE);
		void                 End();

		void                 Draw(Vector2f pos, Vector2f scale, const char* fmt, ...);

		inline void          Render(Camera* cam, bool world = true) { Render(cam, ColorRGBA8{ 255, 255, 255, 255 }, world); }
		void                 Render(Camera* cam, ColorRGBA8 color, bool world = true);
		void                 Render(Camera* cam, ColorRGBA32 color, bool world = true);

		inline RAPI::Shader* GetShader() { return m_Shader; }
		inline void          SetShader(RAPI::Shader* shader) { m_Shader = shader; }

		inline Font::Font*   GetFont() { return m_Font; }
		inline void          SetFont(Font::Font* font) { m_Font = font; }
	private:
		void                 SortGlyphs();
		void                 GenerateFontBatches();
	public:
		std::vector<FontBatch> m_FontBatches;
	private:
		RAPI::VertexBuffer m_VBO;
		RAPI::VertexArray m_VAO;

		GlyphSortType m_eGlyphSortType;

		std::vector<FontGlyph*> m_GlyphPointers; // Only used for sorting
		std::vector<FontGlyph> m_Glyphs;

		RAPI::Shader* m_Shader = nullptr;
		Font::Font* m_Font = Font::Font::GetDeveloperFont();
	};
}
#endif // RENGINE_FONT_RENDERER_H

