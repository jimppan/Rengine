#include "PCH.h"
#include "Font.h"

#include "Rengine/Render/RenderAPI/RAPI.h"

#include "Rengine/Resource/Factory.h"

namespace Rengine
{
	namespace Font
	{
		FT_Library Font::s_FreeTypeLib;

		Font       Font::s_DeveloperFont;

		Font::Font()
		{

		}

		Font::Font(const std::string& path, int pxwidth, int pxheight):
			m_szFont(path)
		{
			m_Face = LoadFont(path, pxwidth, pxheight);
		}

		void Font::Initialize()
		{
			FT_Error err = FT_Init_FreeType(&s_FreeTypeLib);
			RENGINE_CORE_ASSERT(!err, "Could not initialize FreeType Library");

			RENGINE_CORE_INFO("Initialized FreeType!");
			// Developer font
			s_DeveloperFont = Font(FONT_DEVELOPER, 0, 16);
			s_DeveloperFont.LoadCharacters(64);

			RENGINE_CORE_INFO("Loaded developer face.");
		}

		FT_Face Font::LoadFont(const std::string& path, int pxwidth, int pxheight)
		{
			FT_Face face;
			FT_Error err = FT_New_Face(s_FreeTypeLib, path.c_str(), 0, &face);
			RENGINE_CORE_ASSERT(!err, "Could not create face");

			FT_Set_Pixel_Sizes(face, pxwidth, pxheight);
			return face;
		}

		void Font::LoadCharacters(int desiredTexWidth)
		{
			glPixelStorei(GL_UNPACK_ALIGNMENT, 1);
			RENGINE_WARN("LGYPHS: {0}, {1}, {2}, {3}", m_Face->size->face->bbox.xMin, m_Face->size->face->bbox.yMin, m_Face->size->face->bbox.xMax, m_Face->size->face->bbox.yMax);
			
			unsigned int tallestGlyph = 0;
			unsigned int widestGlyph = 0;
			unsigned int totalGlyphWidth = 0;
			unsigned int divisor = desiredTexWidth;

			for (unsigned char c = 0; c < 128; c++)
			{
				if (FT_Load_Char(m_Face, c, FT_LOAD_RENDER))
				{
					RENGINE_WARN("Font::LoadCharacters: Failed to load character '{0}' ({1})", c, m_szFont.c_str());
					continue;
				}

				if (m_Face->glyph->bitmap.rows > tallestGlyph)
					tallestGlyph = m_Face->glyph->bitmap.rows;
				if (m_Face->glyph->bitmap.width > widestGlyph)
					widestGlyph = m_Face->glyph->bitmap.width;

				totalGlyphWidth += m_Face->glyph->bitmap.width;
			}

			unsigned int texHeight = ((totalGlyphWidth / divisor) * tallestGlyph) + tallestGlyph;
			unsigned int texWidth = desiredTexWidth;
	
			Texture* texture = new Texture();
			texture->Reserve((int)texWidth, (int)texHeight, 1);

			for (unsigned char c = 0; c < 128; c++)
			{
				if (FT_Load_Char(m_Face, c, FT_LOAD_RENDER))
				{
					RENGINE_WARN("Font::LoadCharacters: Failed to load character '{0}' ({1})", c, m_szFont.c_str());
					continue;
				}

				FT_GlyphSlot glyph = m_Face->glyph;

				// copy over the glyph
				static int widthOffset = 0;
				static int heightOffset = 0;

				if (widthOffset + glyph->bitmap.width >= texWidth)
				{
					widthOffset = 0;
					heightOffset += tallestGlyph;
				}

				for (unsigned int x = 0; x < glyph->bitmap.width; x++)
				{
					for (unsigned int y = 0; y < glyph->bitmap.rows; y++)
					{
						int index = y * glyph->bitmap.width + x;

						int xTexPos = x + widthOffset;

						// Invert y position here so we do not have to constantly invert the UV.y scale
						int yTexPos = ((glyph->bitmap.rows - y) - 1) + heightOffset;

						texture->SetPixel(xTexPos, yTexPos, glyph->bitmap.buffer[index]);
					}
				}

				character_t fchar;
				fchar.uv.x = (float)widthOffset / (float)texWidth;
				fchar.uv.y = (float)heightOffset / (float)texHeight;

				fchar.uv.z = (float)glyph->bitmap.width / (float)texWidth;
				fchar.uv.w = (float)glyph->bitmap.rows / (float)texHeight;

				fchar.size = { glyph->bitmap.width, glyph->bitmap.rows };
				fchar.bearing = { glyph->bitmap_left, glyph->bitmap_top };
				fchar.advance = glyph->advance.x;

				m_Characters[c] = fchar;

				widthOffset += glyph->bitmap.width;
			}


			texture->LoadFromBuffer(GL_RED,
				texWidth,
				texHeight,
				GL_RED, GL_UNSIGNED_BYTE, texture->Data());

			m_FontTexture = Factory::Textures::AddTexture(m_szFont.c_str(), texture);

			/*glPixelStorei(GL_UNPACK_ALIGNMENT, 1);
			RENGINE_WARN("TEST: {0}", m_Face->size->metrics.ascender);
			RENGINE_WARN("TEST1: {0}", m_Face->size->metrics.descender);
			RENGINE_WARN("TEST2: {0}", m_Face->size->metrics.height);
			RENGINE_WARN("TEST3: {0}", m_Face->size->metrics.max_advance);
			RENGINE_WARN("TEST4: {0}", m_Face->size->metrics.x_ppem);
			RENGINE_WARN("TEST5: {0}", m_Face->size->metrics.y_ppem);
			RENGINE_WARN("TEST6: {0}", m_Face->size->metrics.x_scale);
			RENGINE_WARN("TEST7: {0}", m_Face->size->metrics.y_scale);
			//RENGINE_WARN("LGYPHS: {0}, {1}, {2}, {3}", m_Face->size->face->bbox.xMin, m_Face->size->face->bbox.yMin, m_Face->size->face->bbox.xMax, m_Face->size->face->bbox.yMax);
			for (unsigned char c = 0; c < 128; c++)
			{
				if (FT_Load_Char(m_Face, c, FT_LOAD_RENDER))
				{
					RENGINE_WARN("Font::LoadCharacters: Failed to load character '{0}' ({1})", c, m_szFont.c_str());
					continue;
				}

				FT_GlyphSlot glyph = m_Face->glyph;

				Texture* texture = new Texture();
				RENGINE_WARN("ROWS: {0}, WIDTH: {1}", glyph->bitmap.rows, glyph->bitmap.width);
				texture->LoadFromBuffer(GL_RED,
					glyph->bitmap.width,
					glyph->bitmap.rows,
					GL_RED, GL_UNSIGNED_BYTE, glyph->bitmap.buffer);

				character_t fchar;
				fchar.texture = texture;
				fchar.size = { glyph->bitmap.width, glyph->bitmap.rows };
				fchar.bearing = { glyph->bitmap_left, glyph->bitmap_top };
				fchar.advance = glyph->advance.x;

				m_Characters[c] = fchar;
			}*/
		}
	}
}

