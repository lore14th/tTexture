#pragma once

#include "Core/Core.h"

namespace tTexture::Renderer {

	enum class ShaderDataType
	{
		None = 0,
		Float, Float2, Float3, Float4,
		Mat3, Mat4,
		Int, Int2, Int3, Int4,
		Bool,
	};

	static uint32_t ShaderDataTypeSize(ShaderDataType type)
	{
		switch (type)
		{
		case ShaderDataType::None:		break;
		case ShaderDataType::Float:		return 4;
		case ShaderDataType::Float2:	return 2 * 4;
		case ShaderDataType::Float3:	return 3 * 4;
		case ShaderDataType::Float4:	return 4 * 4;
		case ShaderDataType::Mat3:		return 3 * 3 * 4;
		case ShaderDataType::Mat4:		return 4 * 4 * 4;
		case ShaderDataType::Int:		return 4;
		case ShaderDataType::Int2:		return 2 * 4;
		case ShaderDataType::Int3:		return 3 * 4;
		case ShaderDataType::Int4:		return 4 * 4;
		case ShaderDataType::Bool:		return 1;
		}
		TTEX_ASSERT(false, "Unknown ShaderDataType");
		return 0;
	}

	struct BufferElement
	{
		std::string Name;
		ShaderDataType Type;
		uint32_t Offset;
		uint32_t Size;
		bool Normalized;

		BufferElement(ShaderDataType type, const std::string& name, bool normalized = false)
			: Name(name), Type(type), Offset(0), Size(ShaderDataTypeSize(type)), Normalized(normalized)
		{
		}

	};

	class VertexBufferLayout
	{
	public:
		VertexBufferLayout() = default;

		VertexBufferLayout(const std::vector<BufferElement>& elements)
			: m_Elements(elements), m_Stride(0)
		{
			CalculateOffsetAndStride();
		}


		VertexBufferLayout(const std::initializer_list<BufferElement>& elements)
			: m_Elements(elements), m_Stride(0)
		{
			CalculateOffsetAndStride();
		}


		inline const std::vector<BufferElement>& GetLayout() const { return m_Elements; }
		inline uint32_t GetStride() const { return m_Stride; }

	private:
		void CalculateOffsetAndStride()
		{
			m_Stride = 0;
			for (auto& element : m_Elements)
			{
				element.Offset = m_Stride;
				m_Stride += element.Size;
			}
		}

	private:
		std::vector<BufferElement> m_Elements;
		uint32_t m_Stride;
	};

	class OpenGLVertexBuffer
	{
	public:
		OpenGLVertexBuffer(VertexBufferLayout layout, uint32_t size);
		~OpenGLVertexBuffer();

		void SetData(void* buffer, uint32_t size, uint32_t offset = 0);
		void Bind() const;

		uint32_t GetSize() const { return m_Size; }
	private:
		void BindVertexBufferLayout() const;

	private:
		uint32_t m_RendererID;
		uint32_t m_Size;

		VertexBufferLayout m_Layout;
	};

	class OpenGLIndexBuffer
	{
	public:
		OpenGLIndexBuffer(uint32_t size);
		~OpenGLIndexBuffer();

		void SetData(void* buffer, uint32_t size, uint32_t offset = 0);
		void Bind() const;
		
		uint32_t GetSize() const { return m_Size; }
		uint32_t GetCount() const { return m_Count; }

	private:
		uint32_t m_RendererID;
		uint32_t m_Size;
		uint32_t m_Count;
	};

}