#include "PCH.h"
#include "LineRenderer.h"

namespace Rengine
{
	LineRenderer::LineRenderer()
	{
		Initialize();
	}


	LineRenderer::~LineRenderer()
	{
	}

	void LineRenderer::Initialize()
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

	void LineRenderer::Begin()
	{
		m_Lines.clear();
	}

	void LineRenderer::End()
	{
		GenerateLines();
	}

	void LineRenderer::GenerateLines()
	{
		if (m_Lines.empty())
			return;

		std::vector<RAPI::VertexPoint> vertices;
		vertices.resize(m_Lines.size() * 2);

		int currentVert = 0;

		for (unsigned int currentLine = 0; currentLine < m_Lines.size(); currentLine++)
		{
			vertices[currentVert].position = m_Lines[currentLine].start;
			vertices[currentVert].color = m_Lines[currentLine].color;
			currentVert++;

			vertices[currentVert].position = m_Lines[currentLine].end;
			vertices[currentVert].color = m_Lines[currentLine].color;
			currentVert++;
		}

		m_VBO.Bind();
		m_VBO.BufferData(nullptr, vertices.size() * sizeof(RAPI::VertexPoint), RAPI::DrawType::DYNAMIC);
		m_VBO.BufferSubData(vertices.data(), 0, vertices.size() * sizeof(RAPI::VertexPoint));
		m_VBO.Unbind();
	}

	void LineRenderer::Render(Camera* cam, ColorRGBA8 color, bool world)
	{
		GetShader()->Use();
		m_VAO.Bind();

		GetShader()->SetUniformMatrix4("P", 1, false, world ? &cam->GetCameraMatrix()[0][0] : &cam->GetProjectionMatrix()[0][0]);

		GetShader()->SetUniformVector4f("u_Color", color);
		glDrawArrays(GL_LINES, 0, m_Lines.size() * 2);

		m_VAO.Unbind();
		GetShader()->Unuse();
	}

	void LineRenderer::Render(Camera* cam, ColorRGBA32 color, bool world)
	{
		GetShader()->Use();
		m_VAO.Bind();

		GetShader()->SetUniformMatrix4("P", 1, false, world ? &cam->GetCameraMatrix()[0][0] : &cam->GetProjectionMatrix()[0][0]);

		GetShader()->SetUniformVector4f("u_Color", color);
		glDrawArrays(GL_LINES, 0, m_Lines.size() * 2);

		m_VAO.Unbind();
		GetShader()->Unuse();
	}
}