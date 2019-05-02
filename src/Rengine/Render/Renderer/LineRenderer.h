#ifndef RENGINE_LINE_RENDERER_H
#define RENGINE_LINE_RENDERER_H

#include "Rengine/Render/RenderAPI/RAPIBuffers.h"
#include "Rengine/Render/RenderAPI/RAPIShader.h"
#include "Rengine/Render/Camera.h"

#define RENGINE_LINE_SHADER    "../Rengine/res/shaders/Line.shader"

namespace Rengine
{
	struct Line
	{
		Line()
		{

		}

		Line(const Vector3f& _start, const Vector3f& _end, const ColorRGBA8& _color):
			start(_start),
			end(_end),
			color(_color)
		{

		}

		Vector3f start;
		Vector3f end;
		ColorRGBA8 color;
	};

	class LineRenderer
	{
	public:
		LineRenderer();
		~LineRenderer();

		void                 Initialize();

		void                 Begin();
		void                 End();

		inline void          Draw(const Vector3f& start, const Vector3f& end, const ColorRGBA8& color) { m_Lines.emplace_back(start, end, color); }
		inline void          Draw(const Vector2f& start, const Vector2f& end, const ColorRGBA8& color) { Draw({ start.x, start.y, 0.0f }, { end.x, end.y, 0.0f }, color); }

		inline void          Render(Camera* cam, bool world = true) { Render(cam, ColorRGBA8{ 255, 255, 255, 255 }, world); }
		void                 Render(Camera* cam, ColorRGBA8 color, bool world = true);
		void                 Render(Camera* cam, ColorRGBA32 color, bool world = true);

		inline RAPI::Shader* GetShader() { return m_Shader; }
		inline void          SetShader(RAPI::Shader* shader) { m_Shader = shader; }
	private:
		void                 GenerateLines();
	private:
		std::vector<Line> m_Lines;

		RAPI::VertexBuffer m_VBO;
		RAPI::VertexArray m_VAO;

		RAPI::Shader* m_Shader = nullptr;
	};
}

#endif // RENGINE_LINE_RENDERER_H