#include "PCH.h"
#include "TileMapRenderer.h"

#include "Rengine/2D/Tile.h"

#include "Rengine/Time.h"

namespace Rengine
{
	TileMapRenderer::TileMapRenderer()
	{
		Initialize();
	}


	TileMapRenderer::~TileMapRenderer()
	{
	}

	void TileMapRenderer::Initialize()
	{
		if (!m_VAO.IsArrayValid())
			m_VAO.GenerateArrays(1);
		if (!m_VBO.IsBufferValid())
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

	void TileMapRenderer::UpdateTile(int layerId, int tileid, Tile* tile)
	{
		if (!tile->GetTileSheet())
			return;

		int index = GetGlyphIndexByTileId(layerId, tileid);
		
		int offset = index * (sizeof(RAPI::Vertex) * 6);

		Rect rect = *tile->GetRect();
		Rect uv = tile->GetNormalizedUV();
		ColorRGBA8 color = *tile->GetColor();

		RAPI::Vertex vertices[6];
		
		vertices[0].color = color;
		vertices[1].color = color;
		vertices[2].color = color;
		vertices[3].color = color;
		vertices[4].color = color;
		vertices[5].color = color;

		vertices[0].position.x = rect.x;
		vertices[0].position.y = rect.y;

		vertices[0].uv.x = uv.x;
		vertices[0].uv.y = uv.y;

		vertices[1].position.x = rect.x + rect.z;
		vertices[1].position.y = rect.y;

		vertices[1].uv.x = uv.x + uv.z;
		vertices[1].uv.y = uv.y;

		vertices[2].position.x = rect.x + rect.z;
		vertices[2].position.y = rect.y + rect.w;

		vertices[2].uv.x = uv.x + uv.z;
		vertices[2].uv.y = uv.y + uv.w;

		vertices[3] = vertices[0]; // Copy bottom left to second triangle
		vertices[4] = vertices[2]; // Copy top right to second triangle

		vertices[5].position.x = rect.x;
		vertices[5].position.y = rect.y + rect.w;

		vertices[5].uv.x = uv.x;
		vertices[5].uv.y = uv.y + uv.w;

		m_VBO.Bind();
		m_VBO.BufferSubData(&vertices, offset, sizeof(RAPI::Vertex) * 6);
		m_VBO.Unbind();
	}

	void TileMapRenderer::Begin(GlyphSortType sortType)
	{
		m_eGlyphSortType = sortType;
		m_TileBatches.clear();
		m_Glyphs.clear();
	}

	void TileMapRenderer::End()
	{
		m_GlyphPointers.resize(m_Glyphs.size());
		for (unsigned int i = 0; i < m_Glyphs.size(); i++)
			m_GlyphPointers[i] = &m_Glyphs[i];

		// 1. Sort tiles by layer
		// 2. Sort tiles by texture
		// 3. Sort tiles by animation

		SortGlyphLayers();
		SortGlyphsTextures();

		GenerateTileBatches();
	}

	void TileMapRenderer::GenerateTileBatches()
	{
		if (m_Glyphs.empty())
			return;

		std::vector<RAPI::Vertex> vertices;
		vertices.resize(m_Glyphs.size() * 6);

		int offset = 0;
		int currentVert = 0;
		m_TileBatches.emplace_back(offset, 6, m_GlyphPointers[0]->texture, m_GlyphPointers[0]->animation);

		vertices[currentVert++] = m_GlyphPointers[0]->bottomLeft;
		vertices[currentVert++] = m_GlyphPointers[0]->bottomRight;
		vertices[currentVert++] = m_GlyphPointers[0]->topRight;

		vertices[currentVert++] = m_GlyphPointers[0]->bottomLeft;
		vertices[currentVert++] = m_GlyphPointers[0]->topRight;
		vertices[currentVert++] = m_GlyphPointers[0]->topLeft;
		offset += 6;

		for (unsigned int currentGlyph = 1; currentGlyph < m_Glyphs.size(); currentGlyph++)
		{
			if(m_GlyphPointers[currentGlyph]->texture != m_GlyphPointers[currentGlyph - 1]->texture)
				m_TileBatches.emplace_back(offset, 6, m_GlyphPointers[currentGlyph]->texture, m_GlyphPointers[currentGlyph]->animation);
			else
				m_TileBatches.back().m_iNumVertices += 6;

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

	void TileMapRenderer::Draw(Tile* tile, int layerIndex, int tileIndex, TileAnimation* anim)
	{
		m_Glyphs.emplace_back(tile->GetTileSheet()->GetTexture(), *tile->GetRect(), tile->GetNormalizedUV(), *tile->GetColor(), 0.0f, layerIndex, tileIndex, anim);
	}

	void TileMapRenderer::SortGlyphLayers()
	{
		switch (m_eGlyphSortType)
		{
		case GlyphSortType::BACK_TO_FRONT:
			std::stable_sort(m_GlyphPointers.begin(), m_GlyphPointers.end(), TileMapRenderer::LayerFrontToBackSort);
			break;
		case GlyphSortType::FRONT_TO_BACK:
			std::stable_sort(m_GlyphPointers.begin(), m_GlyphPointers.end(), TileMapRenderer::LayerBackToFrontSort);
			break;
		}
	}

	void TileMapRenderer::SortGlyphsTextures()
	{
		if (m_GlyphPointers.size() < 2)
			return;

		int lastIndex = 0;
		for (unsigned int i = 1; i < m_GlyphPointers.size(); i++)
		{
			int index = i - 1;
			if (m_GlyphPointers[lastIndex]->layerIndex != m_GlyphPointers[i]->layerIndex)
			{
				// Sort by textures
				std::stable_sort(m_GlyphPointers.begin() + lastIndex, m_GlyphPointers.begin() + index, Renderer::TextureSort);

				// Sort by animation id
				// DOESNT WORK ON DEBUG ???
				std::stable_sort(m_GlyphPointers.begin() + lastIndex, m_GlyphPointers.begin() + index, TileMapRenderer::AnimationSort);
				lastIndex = index;
			}
		}
	}

	bool TileMapRenderer::LayerFrontToBackSort(TileGlyph* first, TileGlyph* second)
	{
		return first->layerIndex < second->layerIndex;
	}

	bool TileMapRenderer::LayerBackToFrontSort(TileGlyph* first, TileGlyph* second)
	{
		return first->layerIndex > second->layerIndex;
	}

	bool TileMapRenderer::AnimationSort(TileGlyph* first, TileGlyph* second)
	{
		return first->animation > second->animation;
	}

	int TileMapRenderer::GetGlyphIndexByTileId(int layerId, int tileId)
	{
		//TODO: Use unordered_map for these indices
		for (unsigned int i = 0; i < m_GlyphPointers.size(); i++)
		{
			if (m_GlyphPointers[i]->layerIndex == layerId && m_GlyphPointers[i]->tileIndex == tileId)
				return i;
		}
		return -1;
	}

	void TileMapRenderer::Render(Camera* cam, ColorRGBA8 color, Vector3f pos)
	{
		GetShader()->Use();
		m_VAO.Bind();

		Matrix4f m_ModelMatrix = Matrix4f(1.0f);
		m_ModelMatrix = glm::translate(m_ModelMatrix, pos);
		m_ModelMatrix = glm::rotate(m_ModelMatrix, 0.0f, Vector3f(1.0f, 0.0f, 0.0f));
		m_ModelMatrix = glm::rotate(m_ModelMatrix, 0.0f, Vector3f(0.0f, 1.0f, 0.0f));
		m_ModelMatrix = glm::rotate(m_ModelMatrix, 0.0f, Vector3f(0.0f, 0.0f, 1.0f));
		m_ModelMatrix = glm::scale(m_ModelMatrix, { 1.0f, 1.0f, 1.0f });

		Matrix4f PVM = Matrix4f(1.0f);
		PVM = cam->GetCameraMatrix() * m_ModelMatrix;

		GetShader()->SetUniformMatrix4("P", 1, false, &PVM[0][0]);

		for (unsigned int i = 0; i < m_TileBatches.size(); i++)
		{
			Texture* texture = m_TileBatches[i].m_Texture;
			texture->Bind(0);
			GetShader()->SetUniformInt("u_Texture", 0);
			GetShader()->SetUniformVector4f("u_Color", color);

			Vector2f uv = { 0.0f, 0.0f };
			TileAnimation* anim = m_TileBatches[i].m_Animation;
			if (anim)
			{
				// This is the target tile we want for our animation
				Recti animUv = anim->GetTileSheet()->GetTileRect(anim->GetCurrentFrame()->tile_id);
				Rect animUvNorm = anim->GetTileSheet()->GetTexture()->NormalizeUV(animUv);

				// This is the tiles original UV coord
				Recti animStartUv = anim->GetTileSheet()->GetTileRect(anim->GetStartTileID());
				Rect animStartUvNorm = anim->GetTileSheet()->GetTexture()->NormalizeUV(animStartUv);

				uv.x = (animUvNorm.x - animStartUvNorm.x);
				uv.y = (animUvNorm.y - animStartUvNorm.y);
			}
			GetShader()->SetUniformVector2f("u_UVOffset", uv);

			glDrawArrays(GL_TRIANGLES, m_TileBatches[i].m_iOffset, m_TileBatches[i].m_iNumVertices);
		}
		m_VAO.Unbind();
		GetShader()->Unuse();
	}

	void TileMapRenderer::Render(Camera* cam, ColorRGBA32 color, Vector3f pos)
	{
		GetShader()->Use();
		m_VAO.Bind();

		Matrix4f m_ModelMatrix = Matrix4f(1.0f);
		m_ModelMatrix = glm::translate(m_ModelMatrix, pos);
		m_ModelMatrix = glm::rotate(m_ModelMatrix, 0.0f, Vector3f(1.0f, 0.0f, 0.0f));
		m_ModelMatrix = glm::rotate(m_ModelMatrix, 0.0f, Vector3f(0.0f, 1.0f, 0.0f));
		m_ModelMatrix = glm::rotate(m_ModelMatrix, 0.0f, Vector3f(0.0f, 0.0f, 1.0f));
		m_ModelMatrix = glm::scale(m_ModelMatrix, { 1.0f, 1.0f, 1.0f });

		Matrix4f PVM = Matrix4f(1.0f);
		PVM = cam->GetCameraMatrix() * m_ModelMatrix;

		GetShader()->SetUniformMatrix4("P", 1, false, &PVM[0][0]);

		for (unsigned int i = 0; i < m_TileBatches.size(); i++)
		{
			Texture* texture = m_TileBatches[i].m_Texture;
			texture->Bind(0);
			GetShader()->SetUniformInt("u_Texture", 0);
			GetShader()->SetUniformVector4f("u_Color", color);

			Vector2f uv = { 0.0f, 0.5f };
			TileAnimation* anim = m_TileBatches[i].m_Animation;
			if (anim)
			{
				// This is the target tile we want for our animation
				Recti animUv =    anim->GetTileSheet()->GetTileAnimationRect(anim->GetCurrentFrame()->tile_id);
				Rect animUvNorm = anim->GetTileSheet()->GetTexture()->NormalizeUV(animUv);

				// This is the tiles original UV coord
				Recti animStartUv = anim->GetTileSheet()->GetTileAnimationRect(anim->GetStartTileID());
				Rect animStartUvNorm = anim->GetTileSheet()->GetTexture()->NormalizeUV(animStartUv);

				uv.x = animUvNorm.x - animStartUvNorm.x;
				uv.x = animUvNorm.y - animStartUvNorm.y;
			}
			GetShader()->SetUniformVector2f("u_UVOffset", uv);

			glDrawArrays(GL_TRIANGLES, m_TileBatches[i].m_iOffset, m_TileBatches[i].m_iNumVertices);
		}
		m_VAO.Unbind();
		GetShader()->Unuse();
	}
}