#include "PCH.h"
#include "SpriteRenderer.h"

namespace Rengine
{
	SpriteRenderer::SpriteRenderer()
	{
		Initialize();
	}


	SpriteRenderer::~SpriteRenderer()
	{
	}

	void SpriteRenderer::Initialize()
	{
		if(!m_VAO.IsArrayValid())
			m_VAO.GenerateArrays(1);
		if(!m_VBO.IsBufferValid())
			m_VBO.GenerateBuffers(1);

		m_VAO.Bind();
		m_VBO.Bind();

		m_VAO.EnableAttribute(0);
		m_VAO.VertexAttribute(RAPI::VertexAttributeData(0, 3, RAPI::VertexDataType::FLOAT, false, sizeof(RAPI::Vertex), (void*)offsetof(RAPI::Vertex, position)));

		m_VAO.EnableAttribute(1);
		m_VAO.VertexAttribute(RAPI::VertexAttributeData(1, 2, RAPI::VertexDataType::FLOAT, false, sizeof(RAPI::Vertex), (void*)offsetof(RAPI::Vertex, uv)));

		m_VAO.EnableAttribute(2);
		m_VAO.VertexAttribute(RAPI::VertexAttributeData(2, 4, RAPI::VertexDataType::UBYTE, true, sizeof(RAPI::Vertex), (void*)offsetof(RAPI::Vertex, color)));

		m_VBO.Unbind();
		m_VAO.Unbind();
	}

	void SpriteRenderer::Begin(GlyphSortType sortType)
	{
		m_eGlyphSortType = sortType;
		m_SpriteBatches.clear();
		m_Glyphs.clear();
	}

	void SpriteRenderer::End()
	{
		m_GlyphPointers.resize(m_Glyphs.size());
		for (unsigned int i = 0; i < m_Glyphs.size(); i++)
			m_GlyphPointers[i] = &m_Glyphs[i];

		SortGlyphs();
		GenerateSpriteBatches();
	}

	/* UV
	,{ 0.0f, 0.0f }
	{ 1.0f, 0.0f },
	{ 1.0f, 1.0f },
	,{ 0.0f, 1.0f }

	*/
	void SpriteRenderer::Draw(Sprite* sprite, const ColorRGBA8& color)
	{
		m_Glyphs.emplace_back(sprite->GetTexture(), *sprite->GetRect(), *sprite->GetUV(), color, sprite->GetDepth());
	}

	void SpriteRenderer::Draw(const Rect& rect, 
		                   const Rect& uv, 
		                   const ColorRGBA8& color, 
		                   Texture* texture, 
		                   const float& depth)
	{
		m_Glyphs.emplace_back(texture, rect, uv, color, depth);
	}

	void SpriteRenderer::SortGlyphs()
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

	void SpriteRenderer::GenerateSpriteBatches()
	{
		if (m_Glyphs.empty())
			return;

		std::vector<RAPI::Vertex> vertices;
		vertices.resize(m_Glyphs.size() * 6);

		int offset = 0;
		int currentVert = 0;
		m_SpriteBatches.emplace_back(offset, 6, m_GlyphPointers[0]->texture);

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
				m_SpriteBatches.emplace_back(offset, 6, m_GlyphPointers[currentGlyph]->texture);
			else
				m_SpriteBatches.back().m_iNumVertices += 6;

			vertices[currentVert++] = m_GlyphPointers[currentGlyph]->bottomLeft;
			vertices[currentVert++] = m_GlyphPointers[currentGlyph]->bottomRight;
			vertices[currentVert++] = m_GlyphPointers[currentGlyph]->topRight;

			vertices[currentVert++] = m_GlyphPointers[currentGlyph]->bottomLeft;
			vertices[currentVert++] = m_GlyphPointers[currentGlyph]->topRight;
			vertices[currentVert++] = m_GlyphPointers[currentGlyph]->topLeft;
			offset += 6;
		}

		m_VBO.Bind();
		m_VBO.BufferData(nullptr, vertices.size() * sizeof(RAPI::Vertex), RAPI::DrawType::DYNAMIC);
		m_VBO.BufferSubData(vertices.data(), 0, vertices.size() * sizeof(RAPI::Vertex));
		m_VBO.Unbind();
	}

	void SpriteRenderer::Render(Camera* cam, ColorRGBA8 color)
	{
		GetShader()->Use();
		m_VAO.Bind();

		GetShader()->SetUniformMatrix4("P", 1, false, &cam->GetCameraMatrix()[0][0]);

		for (unsigned int i = 0; i < m_SpriteBatches.size(); i++)
		{
			Texture* texture = m_SpriteBatches[i].m_Texture;
			texture->Bind(0);
			GetShader()->SetUniformInt("u_Texture", 0);
			GetShader()->SetUniformVector4f("u_Color", color);

			glDrawArrays(GL_TRIANGLES, m_SpriteBatches[i].m_iOffset, m_SpriteBatches[i].m_iNumVertices);
		}
		m_VAO.Unbind();
		GetShader()->Unuse();
	}

	void SpriteRenderer::Render(Camera* cam, ColorRGBA32 color)
	{
		GetShader()->Use();
		m_VAO.Bind();

		GetShader()->SetUniformMatrix4("P", 1, false, &cam->GetCameraMatrix()[0][0]);

		for (unsigned int i = 0; i < m_SpriteBatches.size(); i++)
		{
			Texture* texture = m_SpriteBatches[i].m_Texture;
			texture->Bind(0);
			GetShader()->SetUniformInt("u_Texture", 0);
			GetShader()->SetUniformVector4f("u_Color", color);

			glDrawArrays(GL_TRIANGLES, m_SpriteBatches[i].m_iOffset, m_SpriteBatches[i].m_iNumVertices);
		}
		m_VAO.Unbind();
		GetShader()->Unuse();
	}
}

