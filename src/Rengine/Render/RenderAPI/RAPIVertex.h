#ifndef RENGINE_RAPI_VERTEX_H
#define RENGINE_RAPI_VERTEX_H

#include "PCH.h"

typedef Vector3f Position;
typedef Vector2f Position2D;
typedef Vector3f Normal;
typedef Vector2f UV;

namespace Rengine
{
	namespace RAPI
	{
		struct Vertex
		{
			Position position;	// 3 * 4 bytes (floats) = 12 bytes
			UV uv;				// 2 * 4 bytes (floats) = 8 bytes
			ColorRGBA8 color;	// 4 * 1 bytes (chars)	= 4 bytes

			Vertex():
				position({0.0f, 0.0f, 0.0f}),
				uv({0.0f, 0.0f}),
				color({0, 0, 0, 0})
			{

			}

			Vertex(Position position,
				UV uv,
				ColorRGBA8 color) :
				position(position),
				uv(uv),
				color(color)
			{

			}

			inline void SetPos(const Vector3f& position)
			{
				this->position = position;
			}

			inline void SetUV(const Vector2f& uv)
			{
				this->uv = uv;
			}

			inline void SetColor(ColorRGBA8 color)
			{
				this->color = color;
			}
		};

		struct VertexFont
		{
			Position2D position;	// 2 * 4 bytes (floats) = 8 bytes
			UV uv;				// 2 * 4 bytes (floats) = 8 bytes

			VertexFont() :
				position({ 0.0f, 0.0f }),
				uv({ 0.0f, 0.0f })
			{

			}

			VertexFont(Position2D position,
				UV uv) :
				position(position),
				uv(uv)
			{

			}

			inline void SetPos(const Vector2f& position)
			{
				this->position = position;
			}

			inline void SetUV(const Vector2f& uv)
			{
				this->uv = uv;
			}
		};

		struct VertexPoint
		{
			Position position;	// 3 * 4 bytes (floats) = 12 bytes
			ColorRGBA8 color;	// 4 * 1 bytes (chars)	= 4 bytes

			VertexPoint() :
				position({ 0.0f, 0.0f, 0.0f }),
				color({ 0, 0, 0, 0 })
			{

			}

			VertexPoint(Position position,
				ColorRGBA8 color) :
				position(position),
				color(color)
			{

			}

			inline void SetPos(const Vector3f& position)
			{
				this->position = position;
			}

			inline void SetColor(ColorRGBA8 color)
			{
				this->color = color;
			}
		};

		struct VertexAttributeData
		{
		public:
			unsigned int index;
			int size;
			unsigned int type;
			bool normalized;
			int stride;
			void* pointer;

			VertexAttributeData(unsigned int _index = 0,
				int _size = 0,
				unsigned int _type = 0,
				bool _normalized = false,
				int _stride = 0,
				void* _pointer = nullptr) :
				index(_index),
				size(_size),
				type(_type),
				normalized(_normalized),
				stride(_stride),
				pointer(_pointer)
			{

			}
		};
	}
}

#endif // RENGINE_RAPI_VERTEX_H