#include "pch.h"
#include "OpenGLShader.h"

#include <fstream>
#include <glad/glad.h>

#include <tTexture/tTexture-Core.h>

namespace tTexture::Renderer {

	OpenGLShader::OpenGLShader(const std::string& filepath)
		: m_RendererID(0), m_Filepath(filepath)
	{
		ShaderProgramSource source = ParseShader(m_Filepath);
		m_RendererID = CreateShader(source.VertexSource, source.FragmentSource);
	}

	OpenGLShader::~OpenGLShader()
	{
		glDeleteProgram(m_RendererID);
	}

	void OpenGLShader::Bind() const
	{
		glUseProgram(m_RendererID);
	}

	void OpenGLShader::SetUniform1i(const std::string& name, int32_t value)
	{
		glUniform1i(GetUniformLocation(name), value);
	}

	void OpenGLShader::SetUniform1f(const std::string& name, float value)
	{
		glUniform1f(GetUniformLocation(name), value);
	}

	void OpenGLShader::SetUniform4f(const std::string& name, float f0, float f1, float f2, float f3)
	{
		glUniform4f(GetUniformLocation(name), f0, f1, f2, f3);
	}

	void OpenGLShader::SetUniformMat4f(const std::string& name, const glm::mat4& matrix)
	{
		glUniformMatrix4fv(GetUniformLocation(name), 1, GL_FALSE, &matrix[0][0]);
	}

	tTexture::Renderer::ShaderProgramSource OpenGLShader::ParseShader(const std::string& filepath)
	{
		enum class ShaderType { NONE = -1, VERTEX = 0, FRAGMENT = 1 };

		std::ifstream stream(filepath);
		if (stream.is_open())
		{
			std::string line;
			std::stringstream ss[2];
			ShaderType type = ShaderType::NONE;

			while (getline(stream, line))
			{
				if (line.find("#type") != std::string::npos)
				{
					if (line.find("vertex") != std::string::npos)
						type = ShaderType::VERTEX;
					else if (line.find("fragment") != std::string::npos)
						type = ShaderType::FRAGMENT;
				}
				else
					ss[(int32_t)type] << line << '\n';
			}

			stream.close();
			return { ss[0].str(), ss[1].str() };
		}
		else
		{
			TTEX_ASSERT(false, "File does not exist");
			return { "", "" };
		}
	}

	uint32_t OpenGLShader::CompileShader(uint32_t type, const std::string& source)
	{
		uint32_t id = glCreateShader(type);
		const char* src = source.c_str();
		glShaderSource(id, 1, &src, nullptr);
		glCompileShader(id);
		// ERROR HADLING
		int result;
		glGetShaderiv(id, GL_COMPILE_STATUS, &result);

		if (result == GL_FALSE)
		{
			int length;
			glGetShaderiv(id, GL_INFO_LOG_LENGTH, &length);
			// allocation on the stack dynamically
			char* message = (char*)alloca(length * sizeof(char));
			glGetShaderInfoLog(id, length, &length, message);
			std::cout << "Failed to compile" << (type == GL_VERTEX_SHADER ? "vertex" : "fragment") << "shader!" << std::endl;
			std::cout << message << std::endl;
			glDeleteShader(id);
			return 0;
		}
		return id;
	}

	int32_t OpenGLShader::CreateShader(const std::string& vertexShader, const std::string& fragmentShader)
	{
		uint32_t program = glCreateProgram();
		// compile shades
		uint32_t vs = CompileShader(GL_VERTEX_SHADER, vertexShader);
		uint32_t fs = CompileShader(GL_FRAGMENT_SHADER, fragmentShader);

		// attaches shaders to the program
		glAttachShader(program, vs);
		glAttachShader(program, fs);

		// link and validate program
		glLinkProgram(program);
		glValidateProgram(program);

		// delete intermediates files
		glDeleteShader(vs);
		glDeleteShader(fs);

		return program;
	}

	int32_t OpenGLShader::GetUniformLocation(const std::string& name)
	{
		if (m_UniformLocationCache.find(name) != m_UniformLocationCache.end())
			return m_UniformLocationCache[name];

		int32_t location = glGetUniformLocation(m_RendererID, name.c_str());

		if (location == -1)
		{
			TTEX_CORE_WARN("OpenGLShader:Uniform {0} does not exist", name);
		}

		m_UniformLocationCache[name] = location;
		return location;
	}

}
