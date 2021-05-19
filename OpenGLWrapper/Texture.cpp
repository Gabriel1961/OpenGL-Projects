#include "Texture.h"
#include "Common.h"
#include "vendor/stb_image/stb_image.h"
Texture::Texture(const char* _FilePath)	
	:m_FilePath(_FilePath),m_Height(0),m_RendererID(0),m_Width(0),m_BPP(0),m_LocalBuffer(nullptr)
{
	stbi_set_flip_vertically_on_load(1);
	m_LocalBuffer = stbi_load(_FilePath, &m_Width, &m_Height, &m_BPP, 4);
	gc(glGenTextures(1, &m_RendererID));
	gc(glBindTexture(GL_TEXTURE_2D, m_RendererID));

	gc(glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MIN_FILTER, GL_LINEAR));
	gc(glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MAG_FILTER, GL_LINEAR));
	gc(glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_WRAP_S, GL_CLAMP));
	gc(glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_WRAP_T, GL_CLAMP));
	
	glTexImage2D(GL_TEXTURE_2D, 0, GL_RGBA8, m_Width, m_Height, 0, GL_RGBA, GL_UNSIGNED_BYTE, m_LocalBuffer);
	gc(glBindTexture(GL_TEXTURE_2D,0));
	if (m_LocalBuffer)
	{
		stbi_image_free(m_LocalBuffer);
	}
}

Texture::~Texture()
{
	gc(glDeleteTextures(1,&m_RendererID));
}

void Texture::Bind(unsigned int slot) const
{
	gc(glActiveTexture(GL_TEXTURE0 + slot));
	gc(glBindTexture(GL_TEXTURE_2D, m_RendererID));
}

void Texture::Unbind() const
{

}
