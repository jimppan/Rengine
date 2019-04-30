#include "PCH.h"
#include "FontRenderer.h"

namespace Rengine
{
	FontRenderer::FontRenderer()
	{
		Initialize();
	}


	FontRenderer::~FontRenderer()
	{
	}

	void FontRenderer::Initialize()
	{
		if (!m_VAO.IsArrayValid())
			m_VAO.GenerateArrays(1);
		if (!m_VBO.IsBufferValid())
			m_VBO.GenerateBuffers(1);

		m_VAO.Bind();
		m_VBO.Bind();

		m_VAO.EnableAttribute(0);
		m_VAO.VertexAttribute(RAPI::VertexAttributeData(0, 2, RAPI::VertexDataType::FLOAT, false, sizeof(RAPI::VertexFont), (void*)offsetof(RAPI::VertexFont, position)));

		m_VAO.EnableAttribute(1);
		m_VAO.VertexAttribute(RAPI::VertexAttributeData(1, 2, RAPI::VertexDataType::FLOAT, false, sizeof(RAPI::VertexFont), (void*)offsetof(RAPI::VertexFont, uv)));

		m_VBO.Unbind();
		m_VAO.Unbind();
	}

	void FontRenderer::Begin(GlyphSortType sortType)
	{
		m_eGlyphSortType = sortType;
		m_FontBatches.clear();
		m_Glyphs.clear();
	}

	void FontRenderer::End()
	{
		m_GlyphPointers.resize(m_Glyphs.size());
		for (unsigned int i = 0; i < m_Glyphs.size(); i++)
			m_GlyphPointers[i] = &m_Glyphs[i];

		SortGlyphs();
		GenerateFontBatches();
	}

	void FontRenderer::Draw(Vector2f pos, Vector2f scale, const char* fmt, ...)
	{
		std::vector<char> temp;
		std::size_t length = 63;
		std::va_list args;
		while (temp.size() <= length)
		{
			temp.resize(length + 1);
			va_start(args, fmt);
			const auto status = std::vsnprintf(temp.data(), temp.size(), fmt, args);
			va_end(args);
			if (status < 0)
				throw std::runtime_error{ "string formatting error" };
			length = static_cast<std::size_t>(status);
		}
		std::string text{ temp.data(), length };

		std::string::const_iterator c;
		for (c = text.begin(); c != text.end(); c++)
		{
			Font::character_t* fchar = m_Font->GetCharacter(*c);

			float xpos = pos.x + fchar->bearing.x * scale.x;
			float ypos = pos.y + (fchar->size.y - fchar->bearing.y) * scale.y;

			float w = fchar->size.x * scale.x;
			float h = fchar->size.y * scale.y;

			FontGlyph glyph;
			glyph.texture = m_Font->GetTexture();

			glyph.bottomLeft.position = { xpos, ypos };
			glyph.bottomLeft.uv = { fchar->uv.x, fchar->uv.y };

			glyph.bottomRight.position = { xpos + w, ypos };
			glyph.bottomRight.uv = { fchar->uv.x + fchar->uv.z, fchar->uv.y };

			glyph.topRight.position = { xpos + w, ypos + h };
			glyph.topRight.uv = { fchar->uv.x + fchar->uv.z, fchar->uv.y + fchar->uv.w };

			glyph.topLeft.position = { xpos, ypos + h };
			glyph.topLeft.uv = { fchar->uv.x, fchar->uv.y + fchar->uv.w };

			pos.x += (fchar->advance >> 6) * scale.x;
			m_Glyphs.push_back(glyph);
		}
	}

	void FontRenderer::SortGlyphs()
	{
		switch (m_eGlyphSortType)
		{
		case GlyphSortType::BACK_TO_FRONT:
			std::stable_sort(m_GlyphPointers.begin(), m_GlyphPointers.end(), &Renderer::FrontToBackSort);
			break;
		case GlyphSortType::FRONT_TO_BACK:
			std::stable_sort(m_GlyphPointers.begin(), m_GlyphPointers.end(), &Renderer::BackToFrontSort);
			break;
		case GlyphSortType::TEXTURE:
			std::stable_sort(m_GlyphPointers.begin(), m_GlyphPointers.end(), &Renderer::TextureSort);
			break;
		}
	}

	void FontRenderer::GenerateFontBatches()
	{
		if (m_Glyphs.empty())
			return;

		std::vector<RAPI::VertexFont> vertices;
		vertices.resize(m_Glyphs.size() * 6);

		int offset = 0;
		int currentVert = 0;
		m_FontBatches.emplace_back(offset, 6, m_GlyphPointers[0]->texture);

		vertices[currentVert++] = m_GlyphPointers[0]->bottomLeft;
		vertices[currentVert++] = m_GlyphPointers[0]->bottomRight;
		vertices[currentVert++] = m_GlyphPointers[0]->topRight;

		vertices[currentVert++] = m_GlyphPointers[0]->bottomLeft;
		vertices[currentVert++] = m_GlyphPointers[0]->topRight;
		vertices[currentVert++] = m_GlyphPointers[0]->topLeft;
		offset += 6;

		for (unsigned int currentGlyph = 1; currentGlyph < m_Glyphs.size(); currentGlyph++)
		{
			if (m_GlyphPointers[currentGlyph]->texture != m_GlyphPointers[currentGlyph - 1]->texture)
				m_FontBatches.emplace_back(offset, 6, m_GlyphPointers[currentGlyph]->texture);
			else
				m_FontBatches.back().m_iNumVertices += 6;

			vertices[currentVert++] = m_GlyphPointers[currentGlyph]->bottomLeft;
			vertices[currentVert++] = m_GlyphPointers[currentGlyph]->bottomRight;
			vertices[currentVert++] = m_GlyphPointers[currentGlyph]->topRight;

			vertices[currentVert++] = m_GlyphPointers[currentGlyph]->bottomLeft;
			vertices[currentVert++] = m_GlyphPointers[currentGlyph]->topRight;
			vertices[currentVert++] = m_GlyphPointers[currentGlyph]->topLeft;
			offset += 6;
		}

		m_VBO.Bind();
		m_VBO.BufferData(nullptr, vertices.size() * sizeof(RAPI::VertexFont), RAPI::DrawType::DYNAMIC);
		m_VBO.BufferSubData(vertices.data(), 0, vertices.size() * sizeof(RAPI::VertexFont));
		m_VBO.Unbind();
	}

	void FontRenderer::Render(Camera* cam, ColorRGBA8 color, bool world)
	{
		GetShader()->Use();
		m_VAO.Bind();

		GetShader()->SetUniformMatrix4("P", 1, false, world ? &cam->GetCameraMatrix()[0][0] : &cam->GetProjectionMatrix()[0][0]);

		for (unsigned int i = 0; i < m_FontBatches.size(); i++)
		{
			Texture* texture = m_FontBatches[i].m_Texture;
			texture->Bind(0);
			GetShader()->SetUniformInt("u_Texture", 0);
			GetShader()->SetUniformVector4f("u_Color", color);

			glDrawArrays(GL_TRIANGLES, m_FontBatches[i].m_iOffset, m_FontBatches[i].m_iNumVertices);
		}
		m_VAO.Unbind();
		GetShader()->Unuse();
	}

	void FontRenderer::Render(Camera * cam, ColorRGBA32 color, bool world)
	{
		GetShader()->Use();
		m_VAO.Bind();

		GetShader()->SetUniformMatrix4("P", 1, false, world ? &cam->GetCameraMatrix()[0][0] : &cam->GetProjectionMatrix()[0][0]);

		for (unsigned int i = 0; i < m_FontBatches.size(); i++)
		{
			Texture* texture = m_FontBatches[i].m_Texture;
			texture->Bind(0);
			GetShader()->SetUniformInt("u_Texture", 0);
			GetShader()->SetUniformVector4f("u_Color", color);

			glDrawArrays(GL_TRIANGLES, m_FontBatches[i].m_iOffset, m_FontBatches[i].m_iNumVertices);
		}
		m_VAO.Unbind();
		GetShader()->Unuse();
	}
}

