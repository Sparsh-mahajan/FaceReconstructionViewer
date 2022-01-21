#pragma once

#include "Includes/VertexArray.h"

VertexArray::VertexArray()
{
	glGenVertexArrays(1, &m_RenderID);
}

VertexArray::~VertexArray()
{
	glDeleteBuffers(1, &m_RenderID);
}

void VertexArray::AddBuffer(const VertexBuffer& vb,const vertexBufferLayout& layout)
{
	Bind();
	vb.Bind();
	const auto& elements = layout.GetElements();
	unsigned int offset = 0;
	for (unsigned int i = 0; i < elements.size(); i++)
	{
		auto& element = elements[i];
		glVertexAttribPointer(i, element.count, element.type, element.normalized, layout.GetStride(),(void*)offset);
		glEnableVertexAttribArray(i);
		offset += element.count * VertexBufferElements::GetSizeOfType(element.type);
	}
}

void VertexArray::Bind() const
{
	glBindVertexArray(m_RenderID);
}

void VertexArray::UnBind() const
{
	glBindVertexArray(0);
}
