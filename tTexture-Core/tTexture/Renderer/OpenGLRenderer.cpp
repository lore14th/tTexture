#include "pch.h"
#include "OpenGLRenderer.h"

#include <GLFW/glfw3.h>
#include <glad/glad.h>

namespace tTexture {

	bool OpenGLRenderer::s_GLFW = false;
	uint32_t OpenGLRenderer::s_RendererCount = 0;

	static void GLFWErrorCallback(int error, const char* description)
	{
		TTEX_CORE_ERROR("GLFW Error ({0}): {1}", error, description);
	}

	OpenGLRenderer::OpenGLRenderer()
	{
		if (!s_GLFW)
		{
			s_GLFW = glfwInit();
			TTEX_CORE_ASSERT(s_GLFW, "Failed to initialize GLFW");
			glfwSetErrorCallback(GLFWErrorCallback);

			glfwWindowHint(GLFW_CONTEXT_VERSION_MAJOR, 4);
			glfwWindowHint(GLFW_CONTEXT_VERSION_MINOR, 6);
			glfwWindowHint(GLFW_SAMPLES, 4);
		#ifdef TTEX_DEBUG
			glfwWindowHint(GLFW_OPENGL_DEBUG_CONTEXT, true);
		#endif
		}

		m_Window = glfwCreateWindow(1, 1, "TTexRenderer", nullptr, nullptr);
		glfwMakeContextCurrent(m_Window);
		glfwHideWindow(m_Window);

		bool gladInit = gladLoadGLLoader((GLADloadproc)glfwGetProcAddress);
		TTEX_CORE_ASSERT(gladInit, "Failed to initialize GLAD");

		Init();
		s_RendererCount++;
	}

	OpenGLRenderer::~OpenGLRenderer()
	{
		s_RendererCount--;
		if (s_RendererCount == 0)
		{
			glfwTerminate();
			s_GLFW = false;
		}
	}

	void OpenGLRenderer::Init()
	{
		// Create a default vertex array
		glCreateVertexArrays(1, &m_Vao);
		glBindVertexArray(m_Vao);



	}

	void OpenGLRenderer::Shutdown()
	{
		glDeleteVertexArrays(1, &m_Vao);
	}

}

