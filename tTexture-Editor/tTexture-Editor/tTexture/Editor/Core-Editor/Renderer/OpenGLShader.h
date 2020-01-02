#pragma once

#include <glm/glm.hpp>

namespace tTexture::Renderer {

	struct ShaderProgramSource
	{
		std::string VertexSource;
		std::string FragmentSource;
	};

	class OpenGLShader
	{
	public:
		OpenGLShader(const std::string& filepath);
		~OpenGLShader();

		void Bind() const;

		// set uniforms
		void SetUniform1i(const std::string& name, int32_t value);
		void SetUniform1f(const std::string& name, float value);
		void SetUniform4f(const std::string& name, float f0, float f1, float f2, float f3);
		void SetUniformMat4f(const std::string& name, const glm::mat4& matrix);

	private:
		ShaderProgramSource ParseShader(const std::string& filepath);
		uint32_t CompileShader(uint32_t type, const std::string& source);
		int32_t CreateShader(const std::string& vertexShader, const std::string& fragmentShader);
		int32_t GetUniformLocation(const std::string& name);

	private:
		std::string m_Filepath;
		uint32_t m_RendererID;
		// TODO: caching uniforms
		std::unordered_map<std::string, int32_t> m_UniformLocationCache;
	};

}
