#ifndef RENGINE_MATH_H
#define RENGINE_MATH_H
#include <glm/glm.hpp>
#include <glm/gtc/type_ptr.hpp>
#include <glm/gtc/matrix_transform.hpp>

typedef glm::mat4 Matrix4f;

typedef glm::ivec2 Vector2i;
typedef glm::ivec3 Vector3i;
typedef glm::ivec4 Vector4i;

typedef glm::uvec2 Vector2u;
typedef glm::uvec3 Vector3u;
typedef glm::uvec4 Vector4u;

typedef glm::vec2 Vector2f;
typedef glm::vec3 Vector3f;
typedef glm::vec4 Vector4f;

typedef glm::vec3 ColorRGB32;
typedef glm::vec4 ColorRGBA32;

typedef glm::vec<3, unsigned char> ColorRGB8;
typedef glm::vec<4, unsigned char> ColorRGBA8;

typedef glm::vec4  Rect;
typedef glm::ivec4 Recti;

namespace Rengine
{
	class Math
	{
	public:
		static inline ColorRGBA8 ColorRGBA32ToColorRGBA8(const ColorRGBA32& color)
		{
			return ColorRGBA8((unsigned char)(color.x * 255), (unsigned char)(color.y * 255), (unsigned char)(color.z * 255), (unsigned char)(color.a * 255));
		}

		static inline ColorRGBA32 ColorRGBA8ToColorRGBA32(const ColorRGBA8& color)
		{
			return ColorRGBA32(color.x / 255.0f, color.y / 255.0f, color.z / 255.0f, color.a / 255.0f);
		}

		static inline ColorRGB8 ColorRGB32ToColorRGB8(const ColorRGB32& color)
		{
			return ColorRGB8((unsigned char)(color.x * 255), (unsigned char)(color.y * 255), (unsigned char)(color.z * 255));
		}

		static inline ColorRGB32 ColorRGB8ToColorRGB32(const ColorRGB8& color)
		{
			return ColorRGB32(color.x / 255.0f, color.y / 255.0f, color.z / 255.0f);
		}
	};
}

#endif // RENGINE_MATH_H