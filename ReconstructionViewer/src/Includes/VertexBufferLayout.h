#pragma once

#include <iostream>
#include "glad/glad.h"
#include <vector>

struct VertexBufferElements
{
	unsigned int type;
	unsigned int count;
	unsigned char normalized;
	static unsigned int GetSizeOfType(unsigned int Type)
	{
		switch (Type)
		{
			case GL_FLOAT:			return 4;
			case  GL_UNSIGNED_INT:	return 4;
			case GL_DOUBLE:			return 8;
		}
		std::cout << "Type does not matches any of the provided types!\n";
		return 0;
	}
};
class vertexBufferLayout
{
private:
	std::vector<VertexBufferElements> m_Elements;
	unsigned int m_Stride;
public:
	typedef VertexBufferElements VBE;
	vertexBufferLayout() : m_Stride(0)
	{}
	
	void Push(unsigned int type, int count)
	{
		if (type == GL_FLOAT)
		{
			m_Elements.emplace_back(VBE{ GL_FLOAT,(unsigned int)count, GL_FALSE });
			m_Stride += VBE::GetSizeOfType(GL_FLOAT) * count;
		}
		else if (type == GL_UNSIGNED_INT)
		{
			m_Elements.emplace_back(VBE{ GL_UNSIGNED_INT,(unsigned int)count, GL_FALSE });
			m_Stride += VBE::GetSizeOfType(GL_UNSIGNED_INT) * count;
		}
		else if(type == GL_DOUBLE)
		{
			m_Elements.emplace_back(VBE{ GL_DOUBLE,(unsigned int)count, GL_TRUE });
			m_Stride += VBE::GetSizeOfType(GL_DOUBLE) * count;
		}
		else
		{
			std::cout << "Type does not matches any of the provided types!\n";
		}
	}

	unsigned int GetStride() const { return m_Stride; }
	const std::vector<VertexBufferElements>& GetElements() const 
	{ 
		return m_Elements;  
	}
};