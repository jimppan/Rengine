#include "PCH.h"
#include "RAPIShader.h"

namespace Rengine
{
	namespace RAPI
	{
		Shader::Shader(const std::string& filepath) :
			m_nFlags(GetUnlitDefinition()),
			m_nMatFlags(ShaderMaterialFlags::NONE)
		{
			CreateShader(filepath);
		}

		Shader::Shader()
		{
		}


		Shader::~Shader()
		{
			glDeleteProgram(m_iProgram);
		}

		unsigned int Shader::CompileShader(unsigned int type, const std::string& source)
		{
			GLCall(unsigned int id = glCreateShader(type));
			const char* src = source.c_str();
			GLCall(glShaderSource(id, 1, &src, nullptr));
			GLCall(glCompileShader(id));

			int result;
			GLCall(glGetShaderiv(id, GL_COMPILE_STATUS, &result));

			if (result == GL_FALSE)
			{
				int length;
				GLCall(glGetShaderiv(id, GL_INFO_LOG_LENGTH, &length));
				char* message = (char*)alloca(length * sizeof(char));
				GLCall(glGetShaderInfoLog(id, length, &length, message));
				RENGINE_CORE_ERROR("Failed to compile {0} shader!\n{1}\n", (type == GL_VERTEX_SHADER ? "vertex" : "fragment"), message);
				GLCall(glDeleteShader(id));
				return 0;
			}

			return id;
		}

		void Shader::CreateShader(const std::string& filepath)
		{
			ShaderSource source = ParseShader(filepath);
			int vertexShader = CompileShader(GL_VERTEX_SHADER, source.vertexSource);
			int fragmentShader = CompileShader(GL_FRAGMENT_SHADER, source.fragmentSource);

			m_iProgram = glCreateProgram();
			GLCall(glAttachShader(m_iProgram, vertexShader));
			GLCall(glAttachShader(m_iProgram, fragmentShader));
			GLCall(glLinkProgram(m_iProgram));

			char infoLog[512];

			int success;
			GLCall(glGetProgramiv(m_iProgram, GL_LINK_STATUS, &success));
			if (!success)
			{
				glGetProgramInfoLog(m_iProgram, 512, NULL, infoLog);
				RENGINE_CORE_ASSERT(false, "Could not link shader {0}", filepath.c_str());
			}

			GLCall(glValidateProgram(m_iProgram));
			
			GLCall(glGetProgramiv(m_iProgram, GL_VALIDATE_STATUS, &success));
			if (!success)
			{
				glGetProgramInfoLog(m_iProgram, 512, NULL, infoLog);
				RENGINE_CORE_ASSERT(false, "Could not link shader {0}", filepath.c_str());
			}
			
			GLCall(glDeleteShader(vertexShader));
			GLCall(glDeleteShader(fragmentShader));
		}

		void Shader::Use()
		{
			glUseProgram(m_iProgram);
		}

		void Shader::Unuse()
		{
			glUseProgram(0);
		}

		int Shader::GetUniformLocation(const std::string& variable)
		{
			RENGINE_CORE_ASSERT(m_iProgram, "Shader program was not valid.");

			if (m_UniformLocations.find(variable) != m_UniformLocations.end())
				return m_UniformLocations[variable];

			int location = glGetUniformLocation(m_iProgram, variable.c_str());
			if (location == -1)
				RENGINE_CORE_ERROR("Error: Shader uniform {0} could not be found", variable);

			m_UniformLocations[variable] = location;
			return location;
		}

		ShaderSource Shader::ParseShader(const std::string& filepath)
		{
			std::ifstream stream(filepath);
			RENGINE_CORE_ASSERT(stream.is_open(), "Could not open shader file {0}", filepath.c_str());
			RENGINE_CORE_INFO("Loaded shader {0}.", filepath.c_str());

			enum class ShaderType
			{
				NONE = -1,
				VERTEX = 0,
				FRAGMENT = 1
			};

			std::string line;
			std::stringstream ss[2];
			ShaderType type = ShaderType::NONE;
			while (getline(stream, line))
			{
				if (line.find("#shader") != std::string::npos)
				{
					if (line.find("vertex") != std::string::npos)
					{
						type = ShaderType::VERTEX;
					}
					else if (line.find("fragment") != std::string::npos)
					{
						type = ShaderType::FRAGMENT;
					}
				}
				else
				{
					ss[(int)type] << line << '\n';
				}
			}

			return { ss[0].str(), ss[1].str() };
		}
	}
}