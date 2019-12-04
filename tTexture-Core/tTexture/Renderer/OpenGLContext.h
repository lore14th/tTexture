#pragma once

#include <glm/glm.hpp>

struct GLFWwindow;

namespace tTexture::Renderer {

	class OpenGLContext
	{
	public:
		OpenGLContext();
		~OpenGLContext();

		void Clear(const glm::vec4& color = glm::vec4(1, 0, 1, 1));
		void SwapBuffers(bool vsynch);

		void DrawIndexed(uint32_t count, bool depthTest);

		std::pair<uint32_t, uint32_t> GetWindowSize() const { return { m_WindowWidth, m_WindowHeight }; }

	private:
		void InitializeGLFW() const;
		void InitializeGLAD() const;

	private:
		GLFWwindow* m_Window;
		uint32_t m_WindowWidth, m_WindowHeight;

		static bool s_GLFWInitialized;
		static uint32_t s_ContextCount;
	};

}