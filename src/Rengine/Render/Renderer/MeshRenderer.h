#ifndef RENGINE_MESH_RENDERER_H
#define RENGINE_MESH_RENDERER_H

#include "Rengine/Render/RenderAPI/RAPIBuffers.h"
#include "Rengine/Render/RenderAPI/RAPIShader.h"
#include "Rengine/Render/Camera.h"
#include "Rengine/Render/Renderer/Renderer.h"

namespace Rengine
{
	class MeshRenderer
	{
	public:
		MeshRenderer();
		~MeshRenderer();

		void                 Initialize();

		void                 Begin();
		void                 End();

		void                 Draw(const Rect& rect, const ColorRGBA8& color);
		void                 Draw(const Vector2f& point1, const Vector2f point2, const Vector2f point3, const ColorRGBA8& color);

		inline void          Render(Camera* cam) { Render(cam, ColorRGBA8{ 255, 255, 255, 255 }); }
		void                 Render(Camera* cam, ColorRGBA8 color);
		void                 Render(Camera* cam, ColorRGBA32 color);

		inline RAPI::Shader* GetShader() { return m_Shader; }
		inline void          SetShader(RAPI::Shader* shader) { m_Shader = shader; }
	private:
		void                 GenerateMeshBatch();
	private:
		RAPI::VertexBuffer m_VBO;
		RAPI::VertexArray m_VAO;

		std::vector<QuadGlyph> m_QuadGlyphs;
		std::vector<TriangleGlyph> m_TriangleGlyphs;

		RAPI::Shader* m_Shader = nullptr;
	};
}
#endif // RENGINE_MESH_RENDERER_H

