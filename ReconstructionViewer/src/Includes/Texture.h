#pragma once

#include "Vendor/stb_image/stb_image.h"
#include "glad/glad.h"
#include <iostream>

class Texture
{
private:
	unsigned int m_RendererID;
	const char* m_ImageFilePath;
	int m_Height, m_Width, m_NrChannels; //no of channels
public:

	static unsigned int i;

	Texture(const char* imageFilePath);
	~Texture();
	void Bind() const;
	void UnBind() const;
};