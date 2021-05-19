#include "VertexArray.h"
#include "VertexBufferLayout.h"
VertexArray::VertexArray()
{
	gc(glGenVertexArrays(1, &m_RendererID));
}

VertexArray::~VertexArray()
{
	gc(glDeleteVertexArrays(1, &m_RendererID));
}

void VertexArray::AddLayout(VertexBuffer& _vb, VertexBufferLayout& _vbl)
{
	Bind();
	const auto& elements= _vbl.GetAttribArray();
	int offset=0;
	for (int i = 0; i < elements.size(); i++) {
		const auto& element = elements[i];
		gc(glEnableVertexAttribArray(i));
		gc(glVertexAttribPointer(i,element.m_Count, element.m_Type, element.m_Normalized, _vbl.GetStride(), (const void*)offset));
		offset += element.m_Count*Attribute::GetTypeSize(element.m_Type);
	}
}

void VertexArray::Bind() const
{
	gc(glBindVertexArray(m_RendererID));
}

void VertexArray::Unbind() const
{
	gc(glBindVertexArray(0));
}

