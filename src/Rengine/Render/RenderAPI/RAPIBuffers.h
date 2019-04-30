#ifndef RENGINE_RAPI_BUFFERS_H
#define RENGINE_RAPI_BUFFERS_H

#include "Rengine/Render/RenderAPI/RAPIVertex.h"
#include "Rengine/Render/RenderAPI/RAPI.h"

namespace Rengine
{
	namespace RAPI
	{
		enum VertexDataType
		{
			BYTE = GL_BYTE,
			UBYTE = GL_UNSIGNED_BYTE,
			SHORT = GL_SHORT,
			USHORT = GL_UNSIGNED_SHORT,
			INT = GL_INT,
			UINT = GL_UNSIGNED_INT,
			FLOAT = GL_FLOAT,
		};

#define RAPI_VERTEX_ID_ASSERT(x) RENGINE_CORE_ASSERT(x != 0, "Ungenerated buffer was used!");
		class VertexBuffer
		{
		public:

			inline static void GenerateBuffers(VertexBuffer* buffer, unsigned int count)
			{
				GLCall(glGenBuffers(count, (unsigned int*)buffer));
			}

			inline static void DeleteBuffers(VertexBuffer* buffer, unsigned int count)
			{
				GLCall(glDeleteBuffers(count, (unsigned int*)buffer));
			}

			inline void GenerateBuffers(unsigned int count)
			{
				VertexBuffer::GenerateBuffers(this, count);
			}

			inline void DeleteBuffers(unsigned int count)
			{
				VertexBuffer::DeleteBuffers(this, count);
			}

			inline void Bind()
			{
				RAPI_VERTEX_ID_ASSERT(m_iBufferID);
				GLCall(glBindBuffer(GL_ARRAY_BUFFER, m_iBufferID));
			}

			inline static void Unbind()
			{
				GLCall(glBindBuffer(GL_ARRAY_BUFFER, 0));
			}

			inline bool IsBufferValid()
			{
				return m_iBufferID != 0;
			}

			inline void BufferData(void* data, unsigned int bytesize, DrawType drawType)
			{
				RAPI_VERTEX_ID_ASSERT(m_iBufferID);
				GLCall(glBufferData(GL_ARRAY_BUFFER, bytesize, data, drawType));
			}

			inline void BufferSubData(void* data, int offset,unsigned int bytesize)
			{
				RAPI_VERTEX_ID_ASSERT(m_iBufferID);
				GLCall(glBufferSubData(GL_ARRAY_BUFFER, offset, bytesize, data));
			}

			inline void* MapBuffer(int access)
			{
				RAPI_VERTEX_ID_ASSERT(m_iBufferID);
				GLCall(void* ret = glMapNamedBuffer(m_iBufferID, access));
				return ret;
			}

			inline void* MapBufferRange(int offset, unsigned int bytesize, int access)
			{
				RAPI_VERTEX_ID_ASSERT(m_iBufferID);
				GLCall(void* ret = glMapNamedBufferRange(m_iBufferID, offset, bytesize, access));
				return ret;
			}

			inline static bool UnmapBuffer()
			{
				GLCall(bool ret = glUnmapBuffer(GL_ARRAY_BUFFER));
				return ret;
			}

		private:
			unsigned int m_iBufferID = 0;
		};

		class VertexArray
		{
		public:

			inline static void GenerateArrays(VertexArray* arr, unsigned int count)
			{
				GLCall(glGenVertexArrays(count, (unsigned int*)arr));
			}

			inline static void DeleteArrays(VertexArray* arr, unsigned int count)
			{
				GLCall(glDeleteVertexArrays(count, (unsigned int*)arr));
			}

			inline void GenerateArrays(unsigned int count)
			{
				VertexArray::GenerateArrays(this, count);
			}

			inline void DeleteArrays(unsigned int count)
			{
				VertexArray::DeleteArrays(this, count);
			}

			inline void Bind()
			{
				RAPI_VERTEX_ID_ASSERT(m_iArrayID);
				GLCall(glBindVertexArray(m_iArrayID));
			}

			inline static void Unbind()
			{
				GLCall(glBindVertexArray(0));
			}

			inline void VertexAttribute(const VertexAttributeData& data)
			{
				RAPI_VERTEX_ID_ASSERT(m_iArrayID);
				GLCall(glVertexAttribPointer(data.index, data.size, data.type, data.normalized, data.stride, data.pointer));
			}

			inline void EnableAttribute(unsigned int attributeIndex)
			{
				RAPI_VERTEX_ID_ASSERT(m_iArrayID);
				GLCall(glEnableVertexAttribArray(attributeIndex));
			}

			inline bool IsArrayValid()
			{
				return m_iArrayID != 0;
			}
		private:
			unsigned int m_iArrayID = 0;
		};

		class IndexBuffer
		{
		public:

			inline static void GenerateBuffers(IndexBuffer* buffer, unsigned int count)
			{
				GLCall(glGenBuffers(count, (unsigned int*)buffer));
			}

			inline static void DeleteBuffers(IndexBuffer* buffer, unsigned int count)
			{
				GLCall(glDeleteBuffers(count, (unsigned int*)buffer));
			}

			inline void GenerateBuffers(unsigned int count)
			{
				IndexBuffer::GenerateBuffers(this, count);
			}

			inline void DeleteBuffers(unsigned int count)
			{
				IndexBuffer::DeleteBuffers(this, count);
			}

			inline void Bind()
			{
				RAPI_VERTEX_ID_ASSERT(m_iBufferID);
				GLCall(glBindBuffer(GL_ELEMENT_ARRAY_BUFFER, m_iBufferID));
			}

			inline static void Unbind()
			{
				GLCall(glBindBuffer(GL_ELEMENT_ARRAY_BUFFER, 0));
			}

			inline bool IsBufferValid()
			{
				return m_iBufferID != 0;
			}

			inline void BufferData(void* data, unsigned int bytesize, DrawType drawType)
			{
				RAPI_VERTEX_ID_ASSERT(m_iBufferID);
				GLCall(glBufferData(GL_ELEMENT_ARRAY_BUFFER, bytesize, data, drawType));
			}

		private:
			unsigned int m_iBufferID = 0;
		};
	}
}
#endif // RENGINE_RAPI_BUFFERS_H