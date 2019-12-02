#pragma once

struct GLFWwindow;

namespace tTexture {

	class OpenGLRenderer
	{
	public:
		OpenGLRenderer();
		~OpenGLRenderer();

		void Init();
		void Shutdown();

	private:
		uint32_t m_Vao, m_Vbo, m_Ibo;
		uint32_t m_Shader;

		GLFWwindow* m_Window;

		static bool s_GLFW;
		static uint32_t s_RendererCount;
	};

}