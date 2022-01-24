#include "Includes/Texture.h"

//unsigned int Texture::i = 0;

Texture::Texture(const char* imageFilePath) :
	m_ImageFilePath(imageFilePath)
{
	stbi_set_flip_vertically_on_load(true);
	glGenTextures(1, &m_RendererID);
	glBindTexture(GL_TEXTURE_2D, m_RendererID);
	glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_WRAP_S, GL_REPEAT);
	glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_WRAP_T, GL_REPEAT);
	glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MIN_FILTER, GL_LINEAR_MIPMAP_LINEAR);
	glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MAG_FILTER, GL_LINEAR);

	unsigned char* data = stbi_load(m_ImageFilePath, &m_Width, &m_Height, &m_NrChannels, 0);
	if (data)
	{
		glTexImage2D(GL_TEXTURE_2D, 0, GL_RGBA, m_Width, m_Height, 0, GL_RGBA, GL_UNSIGNED_BYTE, data);
		glGenerateMipmap(GL_TEXTURE_2D);
	}
	else
	{
		std::cout << "Failed to load Texture! \n";
	}
	stbi_image_free(data);
	glBindTexture(GL_TEXTURE_2D, 0);
}

Texture::~Texture()
{
	glDeleteTextures(1, &m_RendererID);
}

void Texture::Bind() const
{
	//glActiveTexture(GL_TEXTURE1 + i);
	glBindTexture(GL_TEXTURE_2D, m_RendererID);
	//i += 1;
}

void Texture::UnBind() const
{
	glBindTexture(GL_TEXTURE_2D, 0);
}
