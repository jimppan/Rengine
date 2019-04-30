#ifndef RENGINE_RAPI_SHADER_H
#define RENGINE_RAPI_SHADER_H

#include "PCH.h"
#include "Rengine/Render/RenderAPI/RAPI.h"

namespace Rengine
{
	namespace RAPI
	{
		class ShaderSource
		{
		public:
			std::string vertexSource;
			std::string fragmentSource;

			ShaderSource(const std::string& vertexSource, const std::string& fragmentSource)
			{
				this->vertexSource = vertexSource;
				this->fragmentSource = fragmentSource;
			}
		};

		class Shader
		{
		public:

			enum class ShaderFlags : unsigned short
			{
				NONE = 0,
				PVM = 1,
				MATERIAL = 2,
				LIT = 4,
				UV = 8,
			};

			friend ShaderFlags operator |(ShaderFlags lflg, ShaderFlags rflg)
			{
				return static_cast<ShaderFlags>(static_cast<unsigned short>(lflg) | static_cast<unsigned short>(rflg));
			}

			enum class ShaderMaterialFlags
			{
				NONE = 0,
				DIFFUSE_MAP = 1,
				SPECULAR_MAP = 2,
				NORMAL_MAP = 4,
				EMISSIVE_MAP = 8,
				COLOR = 16
			};

			class ShaderDefinition
			{
				ShaderFlags m_nFlags;
				ShaderMaterialFlags m_nMatFlags;

				ShaderDefinition(ShaderFlags& shaderflags, ShaderMaterialFlags& matflags) :
					m_nFlags(shaderflags),
					m_nMatFlags(matflags)
				{

				}
			};

			inline static ShaderFlags GetLitDefinition()
			{
				return ShaderFlags::PVM | ShaderFlags::LIT | ShaderFlags::UV | ShaderFlags::MATERIAL;
			}

			inline static ShaderFlags GetUnlitDefinition()
			{
				return ShaderFlags::PVM | ShaderFlags::UV | ShaderFlags::MATERIAL;
			}

			inline static ShaderFlags GetUnlitColorDefinition()
			{
				return ShaderFlags::PVM | ShaderFlags::MATERIAL;
			}

			inline static ShaderFlags GetVertexColorDefinition()
			{
				return ShaderFlags::PVM | ShaderFlags::MATERIAL;
			}


			Shader();
			Shader(const std::string& filepath);
			~Shader();

			ShaderFlags m_nFlags;
			ShaderMaterialFlags m_nMatFlags;

			void CreateShader(const std::string& filepath);

			void Use();
			static void Unuse();

			int GetUniformLocation(const std::string& variable);

			// Matrices
			inline void SetUniformMatrix4(const char* variable, const int& count, const bool& transpose, const float* value) { glUniformMatrix4fv(GetUniformLocation(variable), count, transpose, value); }
			inline void SetUniformMatrix3(const char* variable, const int& count, const bool& transpose, const float* value) { glUniformMatrix3fv(GetUniformLocation(variable), count, transpose, value); }
			inline void SetUniformMatrix2(const char* variable, const int& count, const bool& transpose, const float* value) { glUniformMatrix2fv(GetUniformLocation(variable), count, transpose, value); }

			// Vectors floats
			inline void SetUniformVector4f(const char* variable, const Vector4f& data)                                       { glUniform4f(GetUniformLocation(variable), data.x, data.y, data.z, data.w); }
			inline void SetUniformVector4f(const char* variable, ColorRGBA8& data)                                           { glUniform4f(GetUniformLocation(variable), data.r / 255.0f, data.g / 255.0f, data.b / 255.0f, data.a / 255.0f); }
																						                                     
			inline void SetUniformVector3f(const char* variable, const Vector3f& data)                                       { glUniform3f(GetUniformLocation(variable), data.x, data.y, data.z); }
			inline void SetUniformVector3f(const char* variable, const ColorRGB8& data)                                      { glUniform3f(GetUniformLocation(variable), data.r / 255.0f, data.g / 255.0f, data.b / 255.0f); }
																						                                     
			inline void SetUniformVector2f(const char* variable, const Vector2f& data)                                       { glUniform2f(GetUniformLocation(variable), data.x, data.y); }
																						                                     
			 // Vector ints																                                     
			inline void SetUniformVector4i(const char* variable, const Vector4i& data)                                       { glUniform4i(GetUniformLocation(variable), data.x, data.y, data.z, data.w); }
			inline void SetUniformVector3i(const char* variable, const Vector3i& data)                                       { glUniform3i(GetUniformLocation(variable), data.x, data.y, data.z); }
			inline void SetUniformVector2i(const char* variable, const Vector2i& data)                                       { glUniform2i(GetUniformLocation(variable), data.x, data.y); }
																					                                         
			// Vectors unsigned ints												                                         
			inline void SetUniformVector4u(const char* variable, const Vector4u& data)                                       { glUniform4ui(GetUniformLocation(variable), data.x, data.y, data.z, data.w); }
			inline void SetUniformVector3u(const char* variable, const Vector3u& data)                                       { glUniform3ui(GetUniformLocation(variable), data.x, data.y, data.z); }
			inline void SetUniformVector2u(const char* variable, const Vector2u& data)                                       { glUniform2ui(GetUniformLocation(variable), data.x, data.y); }

			// Basic
			inline void SetUniformFloat(const char* variable, const float& x)                                                { glUniform1f(GetUniformLocation(variable), x); }
			inline void SetUniformInt(const char* variable, const int& x)                                                    { glUniform1i(GetUniformLocation(variable), x); }
			inline void SetUniformUnsignedInt(const char* variable, const unsigned int& x)                                   { glUniform1ui(GetUniformLocation(variable), x); }
			inline void SetUniformBool(const char* variable, const bool& x)                                                  { glUniform1i(GetUniformLocation(variable), x); }

			inline void AddAttribute(const std::string& attribute) { glBindAttribLocation(m_iProgram, m_iAttributeCount++, attribute.c_str()); }
			static ShaderSource ParseShader(const std::string& filepath);
		private:
			unsigned int CompileShader(unsigned int type, const std::string& source);
			//ShaderSource ParseShader(const std::string& filepath);

			unsigned int m_iProgram = 0;
			unsigned int m_iAttributeCount = 0;
			std::unordered_map<std::string, int> m_UniformLocations;
		};
	}
}

#endif // RENGINE_RAPI_SHADER_H