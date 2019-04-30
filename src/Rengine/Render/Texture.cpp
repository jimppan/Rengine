#include "PCH.h"
#include "Texture.h"

#include "Rengine/Render/RenderAPI/RAPI.h"
#include "Rengine/Render/RenderAPI/RAPIPng.h"

namespace Rengine
{
	Texture::Texture()
	{
		
	}

	Texture::Texture(const Texture& other):
		m_szFilePath(other.m_szFilePath),
		m_iWidth(other.m_iWidth),
		m_iHeight(other.m_iHeight),
		m_iBPP(other.m_iBPP),
		m_eFilterMode(other.m_eFilterMode)
	{
		if (other.m_pData)
		{
			m_pData = new unsigned char[other.GetDataSize()];
			memcpy(m_pData, other.m_pData, other.GetDataSize());

			if (m_iTextureID == 0)
				GLCall(glGenTextures(1, &m_iTextureID));

			GLCall(glBindTexture(GL_TEXTURE_2D, m_iTextureID));

			switch (m_eFilterMode)
			{
			case FilterMode::POINT:
				GLCall(glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MAG_FILTER, GL_NEAREST));
				GLCall(glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MIN_FILTER, GL_NEAREST));
				break;

			case FilterMode::LINEAR:
				GLCall(glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MAG_FILTER, GL_LINEAR));
				GLCall(glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MIN_FILTER, GL_LINEAR));
				break;
			}

			GLCall(glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_WRAP_S, GL_CLAMP_TO_EDGE));
			GLCall(glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_WRAP_T, GL_CLAMP_TO_EDGE));

			GLCall(glTexImage2D(GL_TEXTURE_2D, 0, GL_RGBA8, m_iWidth, m_iHeight, 0, GL_RGBA, GL_UNSIGNED_BYTE, m_pData));
			GLCall(glBindTexture(GL_TEXTURE_2D, 0));
		}
	}

	Texture::~Texture()
	{
		FreeImage();

		if (m_iTextureID != 0)
			GLCall(glDeleteTextures(1, &m_iTextureID));
	}

	bool Texture::LoadPNG(const std::string& path, bool keepData)
	{
		glPixelStorei(GL_UNPACK_ALIGNMENT, 4); // defualt is 4, 1 is used for fonts
		stbi_set_flip_vertically_on_load(1);

		// desired channels 4 for RGBA
		m_pData = stbi_load(path.c_str(), &m_iWidth, &m_iHeight, &m_iBPP, 4);

		if (!m_pData)
		{
			RENGINE_CORE_WARN("Missing texture \"{0}\"", path.c_str());
			return false;
		}

		LoadFromBuffer(GL_RGBA8, m_iWidth, m_iHeight, GL_RGBA, GL_UNSIGNED_BYTE, m_pData);

		if (!keepData)
			FreeImage();
		return true;
	}

	void Texture::LoadFromBuffer(int iformat, int width, int height, int format, int type, void* data)
	{
		if (m_iTextureID == 0)
			GLCall(glGenTextures(1, &m_iTextureID));

		GLCall(glBindTexture(GL_TEXTURE_2D, m_iTextureID));

		// GL_NEAREST TO SNAP TO NEAREST PIXEL
		GLCall(glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MIN_FILTER, GL_NEAREST));
		GLCall(glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MAG_FILTER, GL_NEAREST));
		GLCall(glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_WRAP_S, GL_CLAMP_TO_EDGE));
		GLCall(glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_WRAP_T, GL_CLAMP_TO_EDGE));

		GLCall(glTexImage2D(GL_TEXTURE_2D, 0, iformat, width, height, 0, format, type, data));
		GLCall(glBindTexture(GL_TEXTURE_2D, 0));
	}

	void Texture::Bind(unsigned int slot)
	{
		GLCall(glActiveTexture(GL_TEXTURE0 + slot));
		GLCall(glBindTexture(GL_TEXTURE_2D, m_iTextureID));
	}

	void Texture::Unbind()
	{
		GLCall(glBindTexture(GL_TEXTURE_2D, 0));
	}

	void Texture::SetFilterMode(const FilterMode& mode)
	{
		if (m_iTextureID == 0)
			return;

		switch (mode)
		{
		case FilterMode::POINT:
			GLCall(glBindTexture(GL_TEXTURE_2D, m_iTextureID));
			GLCall(glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MAG_FILTER, GL_NEAREST));
			GLCall(glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MIN_FILTER, GL_NEAREST));
			GLCall(glBindTexture(GL_TEXTURE_2D, 0));
			break;

		case FilterMode::LINEAR:
			GLCall(glBindTexture(GL_TEXTURE_2D, m_iTextureID));
			GLCall(glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MAG_FILTER, GL_LINEAR));
			GLCall(glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MIN_FILTER, GL_LINEAR));
			GLCall(glBindTexture(GL_TEXTURE_2D, 0));
			break;
		}
		m_eFilterMode = mode;
	}

	Rect Texture::NormalizeUV(const Recti& rect)
	{
		Rect newRect = rect;
		newRect.x /= m_iWidth;
		newRect.y /= m_iHeight;
		newRect.z /= m_iWidth;
		newRect.w /= m_iHeight;
		return newRect;
	}

	void Texture::Reserve(int x, int y, int bpp)
	{
		int count = x * y * bpp;
		m_pData = new unsigned char[count];
		m_iWidth = x;
		m_iHeight = y;
		m_iBPP = bpp;

		for (int i = 0; i < count; i++)
			m_pData[i] = 0;
	}

	ColorRGBA8 Texture::GetPixelRGBA(int x, int y) const
	{
		RENGINE_CORE_ASSERT(x >= 0 && y >= 0 && x < m_iWidth && y < m_iHeight, "Invalid texture coordinate");

		ColorRGBA8 ret;
		memcpy(&ret, &m_pData[(y * m_iWidth + x) * m_iBPP], sizeof(ColorRGBA8));
		return ret;
	}

	ColorRGB8 Texture::GetPixelRGB(int x, int y) const
	{
		RENGINE_CORE_ASSERT(x >= 0 && y >= 0 && x < m_iWidth && y < m_iHeight, "Invalid texture coordinate");

		ColorRGB8 ret;
		memcpy(&ret, &m_pData[(y * m_iWidth + x) * m_iBPP], sizeof(ColorRGB8));
		return ret;
	}

	unsigned char Texture::GetPixel(int x, int y) const
	{
		RENGINE_CORE_ASSERT(x >= 0 && y >= 0 && x < m_iWidth && y < m_iHeight, "Invalid texture coordinate");

		return m_pData[(y * m_iWidth + x) * m_iBPP];
	}

	void Texture::SetPixelRGBA(int x, int y, ColorRGBA8 color) const
	{
		RENGINE_CORE_ASSERT(x >= 0 && y >= 0 && x < m_iWidth && y < m_iHeight, "Invalid texture coordinate");

		memcpy(&m_pData[(y * m_iWidth + x) * m_iBPP], &color, sizeof(ColorRGBA8));
	}

	void Texture::SetPixelRGB(int x, int y, ColorRGB8 color) const
	{
		RENGINE_CORE_ASSERT(x >= 0 && y >= 0 && x < m_iWidth && y < m_iHeight, "Invalid texture coordinate");

		memcpy(&m_pData[(y * m_iWidth + x) * m_iBPP], &color, sizeof(ColorRGB8));
	}

	void Texture::SetPixel(int x, int y, unsigned char color) const
	{
		RENGINE_CORE_ASSERT(x >= 0 && y >= 0 && x < m_iWidth && y < m_iHeight, "Invalid texture coordinate");

		m_pData[(y * m_iWidth + x) * m_iBPP] = color;
	}

	void Texture::FreeImage()
	{
		if(m_pData)
			stbi_image_free(m_pData);
		m_pData = nullptr;
	}
}