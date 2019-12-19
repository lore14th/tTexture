#include "pch.h"
#include "OpenGLBuffer.h"

#include <glad/glad.h>

namespace tTexture::Renderer {

	static uint32_t ShaderDataTypeComponentCount(ShaderDataType type)
	{
		switch (type)
		{
			case ShaderDataType::None:		break;
			case ShaderDataType::Float:		return 1;
			case ShaderDataType::Float2:	return 2;
			case ShaderDataType::Float3:	return 3;
			case ShaderDataType::Float4:	return 4;
			case ShaderDataType::Mat3:		return 3 * 3;
			case ShaderDataType::Mat4:		return 4 * 4;
			case ShaderDataType::Int:		return 1;
			case ShaderDataType::Int2:		return 2;
			case ShaderDataType::Int3:		return 3;
			case ShaderDataType::Int4:		return 4;
			case ShaderDataType::Bool:		return 1;
		}
		TTEX_ASSERT(false, "Invalid ShaderDataType");
		return 0;
	}

	static GLenum GetOpenGLType(ShaderDataType type)
	{
		switch (type)
		{
			case ShaderDataType::None:		break;
			case ShaderDataType::Float:		return GL_FLOAT;
			case ShaderDataType::Float2:	return GL_FLOAT;
			case ShaderDataType::Float3:	return GL_FLOAT;
			case ShaderDataType::Float4:	return GL_FLOAT;
			case ShaderDataType::Mat3:		return GL_FLOAT;
			case ShaderDataType::Mat4:		return GL_FLOAT;
			case ShaderDataType::Int:		return GL_INT;
			case ShaderDataType::Int2:		return GL_INT;
			case ShaderDataType::Int3:		return GL_INT;
			case ShaderDataType::Int4:		return GL_INT;
			case ShaderDataType::Bool:		return GL_BOOL;
		}
		TTEX_CORE_ASSERT(false, "Invalid ShaderDataType");
		return 0;
	}


	OpenGLVertexBuffer::OpenGLVertexBuffer(VertexBufferLayout layout, uint32_t size)
		: m_RendererID(0), m_Size(size), m_Layout(layout)
	{
		glGenBuffers(1, &m_RendererID);
	}

	OpenGLVertexBuffer::~OpenGLVertexBuffer()
	{
		glDeleteBuffers(1, &m_RendererID);
	}

	void OpenGLVertexBuffer::BindVertexBufferLayout() const
	{
		uint32_t stride = m_Layout.GetStride();

		const std::vector<BufferElement>& elements = m_Layout.GetLayout();
		for (uint32_t i = 0; i < elements.size(); i++)
		{
			const BufferElement& element = elements[i];
			int32_t elementSize = ShaderDataTypeComponentCount(element.Type);
			GLenum elementType = GetOpenGLType(element.Type);

			glEnableVertexAttribArray(i);
			if (elementType == GL_INT)
				glVertexAttribIPointer(i, elementSize, elementType, stride, (const void*)(__int64)element.Offset);
			else
				glVertexAttribPointer(i, elementSize, elementType, element.Normalized, stride, (const void*)(__int64)element.Offset);
		}
	}

	void OpenGLVertexBuffer::SetData(void* buffer, uint32_t size, uint32_t offset)
	{
		m_Size = size;
		glBindBuffer(GL_ARRAY_BUFFER, m_RendererID);
		glBufferData(GL_ARRAY_BUFFER, size, buffer, GL_STATIC_DRAW);
	}

	void OpenGLVertexBuffer::Bind() const
	{
		glBindBuffer(GL_ARRAY_BUFFER, m_RendererID);
		BindVertexBufferLayout();
	}

	OpenGLIndexBuffer::OpenGLIndexBuffer(uint32_t size)
		: m_RendererID(0), m_Size(size), m_Count(0)
	{
		glGenBuffers(1, &m_RendererID);
	}

	OpenGLIndexBuffer::~OpenGLIndexBuffer()
	{
		glDeleteBuffers(1, &m_RendererID);
	}

	void OpenGLIndexBuffer::SetData(void* buffer, uint32_t size, uint32_t offset)
	{
		m_Size = size;
		m_Count = size / sizeof(uint32_t);

		glBindBuffer(GL_ELEMENT_ARRAY_BUFFER, m_RendererID);
		glBufferData(GL_ELEMENT_ARRAY_BUFFER, size, buffer, GL_STATIC_DRAW);
	}

	void OpenGLIndexBuffer::Bind() const
	{
		glBindBuffer(GL_ELEMENT_ARRAY_BUFFER, m_RendererID);
	}

}