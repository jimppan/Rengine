#ifndef RENGINE_TEXTURE_H
#define RENGINE_TEXTURE_H

#include "PCH.h"

namespace Rengine
{
	class Texture
	{
	public:

		enum FilterMode
		{
			POINT,
			LINEAR
		};

		Texture();
		Texture(const Texture& other);
		~Texture();

		void                      LoadFromBuffer(int iformat, int width, int height, int format, int type, void* data);

		bool                      LoadPNG(const std::string& path, bool keepData = true);

		//GL_NEAREST - Point
		//GL_LINEAR - Blur
		void                      SetFilterMode(const FilterMode& mode);
		

		

		void                      Bind(unsigned int slot = 0);
		static void               Unbind();

		inline const unsigned int GetTextureID() const            { return m_iTextureID; }

		inline int                GetDataSize() const             { return m_iWidth * m_iHeight * m_iBPP; }

		inline int                GetWidth() const                { return m_iWidth; }
		inline int                GetHeight() const               { return m_iHeight; }
		inline int                GetBytesPerPixel() const        { return m_iBPP; }
		inline FilterMode         GetFilterMode() const           { return m_eFilterMode; }


		ColorRGBA8                GetPixelRGBA(int x, int y) const;
		ColorRGB8                 GetPixelRGB(int x, int y) const;
		unsigned char             GetPixel(int x, int y) const;

		void                      SetPixelRGBA(int x, int y, ColorRGBA8 color) const;
		void                      SetPixelRGB(int x, int y, ColorRGB8 color) const;
		void                      SetPixel(int x, int y, unsigned char color) const;

		Rect                      NormalizeUV(const Recti& rect);

		// bytes per pixel
		void                      Reserve(int x, int y, int bpp);
		inline unsigned char*     Data()                          { return m_pData; }

	private:
		void FreeImage();
	private:
		std::string m_szFilePath;
		unsigned char* m_pData = nullptr;
		int m_iWidth = 0;
		int m_iHeight = 0;
		int m_iBPP = 0;
		FilterMode m_eFilterMode = FilterMode::POINT;

		unsigned int m_iTextureID = 0;
	};
}

#endif // RENGINE_TEXTURE_H