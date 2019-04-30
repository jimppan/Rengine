#include "PCH.h"
#include "RAPI.h"

namespace Rengine
{
	namespace RAPI
	{
		void RAPI::GLClearError()
		{
			while (glGetError() != GL_NO_ERROR);
		}

		bool RAPI::GLLogCall(const char* function, const char* file, int line)
		{
			while (GLenum error = glGetError())
			{
				switch (error)
				{
				case GL_INVALID_ENUM:
					RENGINE_CORE_ERROR("[OpenGL ERROR] Invalid ENUM ({0})", error);
					RENGINE_CORE_ERROR("Function call: {0} in {1} on line {2}", function, file, line);
					break;
				default:
					RENGINE_CORE_ERROR("[OpenGL ERROR] Undefined error code ({0})", error);
					RENGINE_CORE_ERROR("Function call: {0} in {1} on line {2}", function, file, line);
					break;
				}
				return false;
			}
			return true;
		}

		void RAPI::ClearColor(const ColorRGBA32& color)
		{
			glClearColor(color.r, color.g, color.b, color.a);
		}


		void RAPI::ClearWindow(const unsigned int& bitfield)
		{
			glClear(bitfield);
		}

		void RAPI::SetViewport(const Rect& rect)
		{
			glViewport((int)rect.x, (int)rect.y, (int)rect.z, (int)rect.w);
		}
	}
}