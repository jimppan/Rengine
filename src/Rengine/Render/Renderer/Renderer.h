#ifndef RENGINE_RENDERER_H
#define RENGINE_RENDERER_H

#include "Rengine/Render/Texture.h"

namespace Rengine
{
	class TileAnimation;
	enum class GlyphSortType
	{
		NONE,
		FRONT_TO_BACK,
		BACK_TO_FRONT,
		TEXTURE
	};

	struct BaseGlyph
	{
		BaseGlyph() {}
		BaseGlyph(Texture* _texture, float _depth) :
			texture(_texture),
			depth(_depth)
		{

		}

		Texture* texture = nullptr;
		float depth = 0.0f;
	};

	struct QuadGlyph
	{
		QuadGlyph() {}
		QuadGlyph(const Rect& _rect, const ColorRGBA8& _color) :
			bottomLeft({ _rect.x, _rect.y, 0.0f }, _color),
			bottomRight({ _rect.x + _rect.z, _rect.y, 0.0f }, _color),
			topRight({ _rect.x + _rect.z, _rect.y + _rect.w, 0.0f }, _color),
			topLeft({ _rect.x, _rect.y + _rect.w, 0.0f }, _color)
		{

		}

		RAPI::VertexPoint bottomLeft;
		RAPI::VertexPoint bottomRight;
		RAPI::VertexPoint topRight;
		RAPI::VertexPoint topLeft;
	};

	struct TriangleGlyph
	{
		TriangleGlyph() {}
		TriangleGlyph(const Vector2f point1, const Vector2f point2, const Vector2f point3, const ColorRGBA8& _color) :
			point1({ point1.x, point1.y, 0.0f }, _color),
			point2({ point2.x, point2.y, 0.0f }, _color),
			point3({ point3.x, point3.y, 0.0f }, _color)
		{

		}

		RAPI::VertexPoint point1;
		RAPI::VertexPoint point2;
		RAPI::VertexPoint point3;
	};


	struct SpriteGlyph : public BaseGlyph
	{
		SpriteGlyph() {}
		SpriteGlyph(Texture* _texture, const Rect& _rect, const Rect& _uv, const ColorRGBA8& _color, float _depth) :
			BaseGlyph(_texture, _depth),
			bottomLeft({ _rect.x, _rect.y, 0.0f},                    { _uv.x, _uv.y },                 _color),
			bottomRight({ _rect.x + _rect.z, _rect.y, 0.0f },        { _uv.x + _uv.z, _uv.y },         _color),
			topRight({ _rect.x + _rect.z, _rect.y + _rect.w, 0.0f }, { _uv.x + _uv.z, _uv.y + _uv.w }, _color),
			topLeft({ _rect.x, _rect.y + _rect.w, 0.0f },            { _uv.x, _uv.y + _uv.w},          _color)
		{
	
		}
		RAPI::Vertex bottomLeft;
		RAPI::Vertex bottomRight;
		RAPI::Vertex topRight;
		RAPI::Vertex topLeft;
	};

	struct TileGlyph : public SpriteGlyph
	{
		TileGlyph() {}
		TileGlyph(Texture* _texture, const Rect& _rect, const Rect& _uv, const ColorRGBA8& _color, float _depth, int _layerIndex, int _tileIndex, TileAnimation* _anim) :
			SpriteGlyph(_texture, _rect, _uv, _color, _depth),
			layerIndex(_layerIndex),
			tileIndex(_tileIndex),
			animation(_anim)
		{

		}

		int layerIndex = 0;
		int tileIndex = 0;
		TileAnimation* animation = nullptr;
	};

	struct FontGlyph : public BaseGlyph
	{
		FontGlyph() {}
		FontGlyph(Texture* _texture, const Rect& _rect, const Rect& _uv, float _depth) :
			BaseGlyph(_texture, _depth),
			bottomLeft({ _rect.x, _rect.y }, { _uv.x, _uv.y }),
			bottomRight({ _rect.x + _rect.z, _rect.y }, { _uv.x + _uv.z, _uv.y }),
			topRight({ _rect.x + _rect.z, _rect.y + _rect.w }, { _uv.x + _uv.z, _uv.y + _uv.w }),
			topLeft({ _rect.x, _rect.y + _rect.w }, { _uv.x, _uv.y + _uv.w })
		{

		}

		RAPI::VertexFont bottomLeft;
		RAPI::VertexFont bottomRight;
		RAPI::VertexFont topRight;
		RAPI::VertexFont topLeft;
	};

	class Renderer
	{
	public:
		inline static bool FrontToBackSort(BaseGlyph* first, BaseGlyph* second)
		{
			return first->depth < second->depth;
		}

		inline static bool BackToFrontSort(BaseGlyph* first, BaseGlyph* second)
		{
			return first->depth > second->depth;
		}

		inline static bool TextureSort(BaseGlyph* first, BaseGlyph* second)
		{
			return first->texture->GetTextureID() > second->texture->GetTextureID();
		}
	private:
		Renderer();
	};
}
#endif // RENGINE_RENDERER_H