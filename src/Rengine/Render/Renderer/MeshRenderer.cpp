#include "PCH.h"
#include "MeshRenderer.h"

namespace Rengine
{
	MeshRenderer::MeshRenderer()
	{
		Initialize();
	}


	MeshRenderer::~MeshRenderer()
	{
	}

	void MeshRenderer::Initialize()
	{
		if (!m_VAO.IsArrayValid())
			m_VAO.GenerateArrays(1);
		if (!m_VBO.IsBufferValid())
			m_VBO.GenerateBuffers(1);

		m_VAO.Bind();
		m_VBO.Bind();

		m_VAO.EnableAttribute(0);
		m_VAO.VertexAttribute(RAPI::VertexAttributeData(0, 3, RAPI::VertexDataType::FLOAT, false, sizeof(RAPI::VertexPoint), (void*)offsetof(RAPI::VertexPoint, position)));

		m_VAO.EnableAttribute(1);
		m_VAO.VertexAttribute(RAPI::VertexAttributeData(1, 4, RAPI::VertexDataType::UBYTE, true, sizeof(RAPI::VertexPoint), (void*)offsetof(RAPI::VertexPoint, color)));

		m_VBO.Unbind();
		m_VAO.Unbind();
	}

	void MeshRenderer::Begin()
	{
		m_QuadGlyphs.clear();
		m_TriangleGlyphs.clear();
	}

	void MeshRenderer::End()
	{
		GenerateMeshBatch();
	}

	void MeshRenderer::Draw(const Rect & rect, const ColorRGBA8 & color)
	{
		m_QuadGlyphs.emplace_back(rect, color);
	}
	
	void MeshRenderer::Draw(const Vector2f& point1, const Vector2f point2, const Vector2f point3, const ColorRGBA8& color)
	{
		m_TriangleGlyphs.emplace_back(point1, point2, point3, color);
	}

	void MeshRenderer::GenerateMeshBatch()
	{
		if (m_QuadGlyphs.empty() && m_TriangleGlyphs.empty())
			return;

		std::vector<RAPI::VertexPoint> vertices;
		vertices.resize(m_QuadGlyphs.size() * 6 + m_TriangleGlyphs.size() * 3);

		int currentVert = 0;

		for (unsigned int currentGlyph = 0; currentGlyph < m_QuadGlyphs.size(); currentGlyph++)
		{
			vertices[currentVert++] = m_QuadGlyphs[currentGlyph].bottomLeft;
			vertices[currentVert++] = m_QuadGlyphs[currentGlyph].bottomRight;
			vertices[currentVert++] = m_QuadGlyphs[currentGlyph].topRight;

			vertices[currentVert++] = m_QuadGlyphs[currentGlyph].bottomLeft;
			vertices[currentVert++] = m_QuadGlyphs[currentGlyph].topRight;
			vertices[currentVert++] = m_QuadGlyphs[currentGlyph].topLeft;
		}

		for (unsigned int currentGlyph = 0; currentGlyph < m_TriangleGlyphs.size(); currentGlyph++)
		{
			vertices[currentVert++] = m_TriangleGlyphs[currentGlyph].point1;
			vertices[currentVert++] = m_TriangleGlyphs[currentGlyph].point2;
			vertices[currentVert++] = m_TriangleGlyphs[currentGlyph].point3;
		}

		m_VBO.Bind();
		m_VBO.BufferData(nullptr, vertices.size() * sizeof(RAPI::VertexPoint), RAPI::DrawType::DYNAMIC);
		m_VBO.BufferSubData(vertices.data(), 0, vertices.size() * sizeof(RAPI::VertexPoint));
		m_VBO.Unbind();
	}

	void MeshRenderer::Render(Camera * cam, ColorRGBA8 color)
	{
		GetShader()->Use();
		m_VAO.Bind();

		GetShader()->SetUniformMatrix4("P", 1, false, &cam->GetCameraMatrix()[0][0]);

		GetShader()->SetUniformVector4f("u_Color", color);

		glDrawArrays(GL_TRIANGLES, 0, m_QuadGlyphs.size() * 6 + m_TriangleGlyphs.size() * 3);

		m_VAO.Unbind();
		GetShader()->Unuse();
	}

	void MeshRenderer::Render(Camera * cam, ColorRGBA32 color)
	{
		GetShader()->Use();
		m_VAO.Bind();

		GetShader()->SetUniformMatrix4("P", 1, false, &cam->GetCameraMatrix()[0][0]);

		GetShader()->SetUniformVector4f("u_Color", color);

		glDrawArrays(GL_TRIANGLES, 0, m_QuadGlyphs.size() * 6 + m_TriangleGlyphs.size() * 3);

		m_VAO.Unbind();
		GetShader()->Unuse();
	}
}

