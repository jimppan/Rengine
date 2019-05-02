#ifndef RENGINE_FONT_H
#define RENGINE_FONT_H

//#define FONT_DEVELOPER "../Rengine/res/fonts/arial.ttf"
#define RENGINE_FONT_DEVELOPER "../Rengine/res/fonts/ProggyTiny.ttf"
#define RENGINE_FONT_SHADER    "../Rengine/res/shaders/Font.shader"

#include "ft2build.h"
#include "freetype/freetype.h" // FT_FREETYPE_H

#include "Rengine/Render/Texture.h"

namespace Rengine
{
	namespace Font
	{
		/*struct character_t
		{
			Texture* texture;
			Vector2i size;           // size of glyph
			Vector2i bearing;        // offset from baseline to left/top of glyph
			unsigned int advance;    // offset to advance to next glyph
		};*/

		struct character_t
		{
			Rect uv;
			Vector2i size;           // size of glyph
			Vector2i bearing;        // offset from baseline to left/top of glyph
			unsigned int advance;    // offset to advance to next glyph
		};

		class Font
		{
		public:
			Font();
			Font(const std::string& path, int pxwidth, int pxheight);
			
		public:
			static void           Initialize();

			void                  LoadCharacters(int desiredTexWidth);

			
			inline void           SetPixelSize(int width, int height) { FT_Set_Pixel_Sizes(m_Face, width, height); }
			inline character_t*   GetCharacter(char c)                { return &m_Characters[c]; }
			inline Texture*       GetTexture()                        { return m_FontTexture; }

			static inline Font*   GetDeveloperFont()                 { return &s_DeveloperFont; }
		private:
			static FT_Face LoadFont(const std::string& path, int pxwidth, int pxheight);
		private:
			std::map<char, character_t> m_Characters;
			Texture* m_FontTexture;
			FT_Face m_Face;
			std::string m_szFont;

			static FT_Library s_FreeTypeLib;

			static Font s_DeveloperFont;
		};
	}
}

#endif // RENGINE_FONT_H

