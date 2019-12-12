#include "pch.h"
#include "OpenGLContext.h"

#include <GLFW/glfw3.h>
#include <glad/glad.h>

namespace tTexture::Renderer {

	bool OpenGLContext::s_GLFWInitialized = false;
	uint32_t OpenGLContext::s_ContextCount = 0;

	static void GLFWErrorCallback(int error, const char* description)
	{
		TTEX_CORE_ERROR("GLFW Error ({0}): {1}", error, description);
	}

	static std::string glErrorTypeToString(GLenum type)
	{
		switch (type)
		{
			case GL_DEBUG_TYPE_ERROR: return "GL_DEBUG_TYPE_ERROR";
			case GL_DEBUG_TYPE_DEPRECATED_BEHAVIOR: return "GL_DEBUG_TYPE_DEPRECATED_BEHAVIOR";
			case GL_DEBUG_TYPE_UNDEFINED_BEHAVIOR: return "GL_DEBUG_TYPE_UNDEFINED_BEHAVIOR";
			case GL_DEBUG_TYPE_PORTABILITY: return "GL_DEBUG_TYPE_PORTABILITY";
			case GL_DEBUG_TYPE_PERFORMANCE: return "GL_DEBUG_TYPE_PERFORMANCE";
			case GL_DEBUG_TYPE_MARKER: return "GL_DEBUG_TYPE_MARKER";
			case GL_DEBUG_TYPE_PUSH_GROUP: return "GL_DEBUG_TYPE_PUSH_GROUP";
			case GL_DEBUG_TYPE_POP_GROUP: return "GL_DEBUG_TYPE_POP_GROUP";
			case GL_DEBUG_TYPE_OTHER: return "GL_DEBUG_TYPE_OTHER";
		}
		return "Unknown Error";
	}

	static void OpenGLCallback(GLenum source, GLenum type, GLuint id, GLenum severity, GLsizei length, const GLchar* message, const void* userParam)
	{
		switch (severity)
		{
			case GL_DEBUG_SEVERITY_LOW:				TTEX_CORE_TRACE("[{0}] GL INFO - {1}:\n\t{2}", id, glErrorTypeToString(type), std::string(message));	TTEX_CORE_ASSERT(false, ""); break;
			case GL_DEBUG_SEVERITY_MEDIUM:			TTEX_CORE_WARN("[{0}] GL INFO - {1}:\n\t{2}", id, glErrorTypeToString(type), std::string(message));		TTEX_CORE_ASSERT(false, ""); break;
			case GL_DEBUG_SEVERITY_HIGH:			TTEX_CORE_ERROR("[{0}] GL INFO - {1}:\n\t{2}", id, glErrorTypeToString(type), std::string(message));	TTEX_CORE_ASSERT(false, ""); break;
			case GL_DEBUG_SEVERITY_NOTIFICATION:	return;
		}
	}

	OpenGLContext::OpenGLContext()
		: m_Window(nullptr), m_WindowWidth(1), m_WindowHeight(1)
	{
		InitializeGLFW();
		TTEX_CORE_ASSERT(s_GLFWInitialized, "OpenGLContext:GLFW not initilized");

		m_Window = glfwCreateWindow(m_WindowWidth, m_WindowHeight, "TTexRenderer", nullptr, nullptr);
		TTEX_CORE_ASSERT(m_Window, "OpenGLContext:Failed to create Window");

		glfwMakeContextCurrent(m_Window);
		glfwHideWindow(m_Window);
		
		InitializeGLAD();
		s_ContextCount++;
	}

	OpenGLContext::~OpenGLContext()
	{
		s_ContextCount--;
		if (s_ContextCount == 0)
		{
			glfwTerminate();
			s_GLFWInitialized = false;
		}
	}

	void OpenGLContext::SwapBuffers(bool vsynch)
	{
		glfwSwapInterval(vsynch);
		glfwSwapBuffers(m_Window);
	}

	void OpenGLContext::InitializeGLFW() const
	{
		if (!s_GLFWInitialized)
		{
			s_GLFWInitialized = glfwInit();
			TTEX_CORE_ASSERT(s_GLFWInitialized, "Failed to initialize GLFW");
			glfwSetErrorCallback(GLFWErrorCallback);

			glfwWindowHint(GLFW_CONTEXT_VERSION_MAJOR, 4);
			glfwWindowHint(GLFW_CONTEXT_VERSION_MINOR, 6);
			glfwWindowHint(GLFW_SAMPLES, 4);	
		#ifdef TTEX_DEBUG
			glfwWindowHint(GLFW_OPENGL_DEBUG_CONTEXT, true);
		#endif
		}
	}

	void OpenGLContext::InitializeGLAD() const
	{
		bool gladInit = gladLoadGLLoader((GLADloadproc)glfwGetProcAddress);
		TTEX_CORE_ASSERT(gladInit, "Failed to initialize GLAD");

		glEnable(GL_MULTISAMPLE);
		glEnable(GL_CULL_FACE);
		glCullFace(GL_CCW);

#if TTEX_DEBUG
		glEnable(GL_DEBUG_OUTPUT_SYNCHRONOUS);
		glDebugMessageCallback(OpenGLCallback, nullptr);
		glDebugMessageControl(GL_DONT_CARE, GL_DONT_CARE, GL_DEBUG_SEVERITY_LOW, 0, nullptr, GL_TRUE);
#endif
	}

	void OpenGLContext::Clear(const glm::vec4& color)
	{
		glClearColor(color.r, color.g, color.b, color.a);
		glClear(GL_COLOR_BUFFER_BIT | GL_DEPTH_BUFFER_BIT);
	}

	void OpenGLContext::DrawIndexed(uint32_t count)
	{
		glDrawElements(GL_TRIANGLES, count, GL_UNSIGNED_INT, nullptr);
	}

}
