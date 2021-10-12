#include "Texture.h"
#include "Common.h"
#include "vendor/stb_image/stb_image.h"
/// <summary>
/// format GL_RGBA8
/// </summary>
/// <param name="_FilePath"></param>
Texture::Texture(std::string _FilePath)
	:m_FilePath(_FilePath), m_Height(0), m_RendererID(0), m_Width(0), m_BPP(0), m_LocalBuffer(nullptr)
{
	if (_FilePath.find(".bmp") == _FilePath.npos)
		stbi_set_flip_vertically_on_load(1);
	m_LocalBuffer = stbi_load(_FilePath.c_str(), &m_Width, &m_Height, &m_BPP, 4);
	gc(glGenTextures(1, &m_RendererID));
	gc(glBindTexture(GL_TEXTURE_2D, m_RendererID));

	gc(glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MIN_FILTER, GL_LINEAR));
	gc(glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MAG_FILTER, GL_LINEAR));
	gc(glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_WRAP_S, GL_CLAMP_TO_EDGE));
	gc(glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_WRAP_T, GL_CLAMP_TO_EDGE));

	gc(glTexImage2D(GL_TEXTURE_2D, 0, GL_RGBA8, m_Width, m_Height, 0, GL_RGBA, GL_UNSIGNED_BYTE, m_LocalBuffer));
	gc(glBindTexture(GL_TEXTURE_2D, 0));
	if (m_LocalBuffer)
	{
		stbi_image_free(m_LocalBuffer);
	}
}

/// <summary>
/// Generate arbitrary size buffer
/// Format : GL_RGBA32F
/// </summary>
Texture::Texture(uint heigth, uint witdth, GLenum antiAliasing, GLenum format)
	:m_Height(heigth), m_Width(witdth)
{
	gc(glActiveTexture(GL_TEXTURE0));
	gc(glGenTextures(1, &m_RendererID));
	gc(glBindTexture(GL_TEXTURE_2D, m_RendererID));

	gc(glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MIN_FILTER, GL_NEAREST));
	gc(glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MAG_FILTER, GL_NEAREST));
	gc(glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_WRAP_S, GL_CLAMP_TO_EDGE));
	gc(glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_WRAP_T, GL_CLAMP_TO_EDGE));

	gc(glTexImage2D(GL_TEXTURE_2D, 0, GL_RGBA32F, witdth, heigth, 0, GL_RGBA, GL_FLOAT,
		NULL));
	gc(glBindImageTexture(0, m_RendererID, 0, GL_FALSE, 0, GL_WRITE_ONLY, GL_RGBA32F));
}
/// <summary>
/// Format ARGB
/// </summary>
Texture::Texture(ARGBColor* buffer, GLenum antiAliasing)
{
	m_Height = Window_Height;
	m_Width = Window_Width;
	m_LocalBuffer = (unsigned char*)buffer;
	gc(glGenTextures(1, &m_RendererID));
	gc(glBindTexture(GL_TEXTURE_2D, m_RendererID));

	gc(glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MIN_FILTER, GL_NEAREST));
	gc(glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MAG_FILTER, GL_NEAREST));
	gc(glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_WRAP_S, GL_CLAMP_TO_EDGE));
	gc(glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_WRAP_T, GL_CLAMP_TO_EDGE));

	gc(glTexImage2D(GL_TEXTURE_2D, 0, GL_RGBA8, m_Width, m_Height, 0, GL_RGBA, GL_UNSIGNED_BYTE, m_LocalBuffer));
	gc(glBindTexture(GL_TEXTURE_2D, 0));
}

Texture::~Texture()
{
	gc(glDeleteTextures(1, &m_RendererID));
}

void Texture::Bind(unsigned int slot) const
{
	gc(glActiveTexture(GL_TEXTURE0 + slot));
	gc(glBindTexture(GL_TEXTURE_2D, m_RendererID));
}

void Texture::Unbind() const
{
	gc(glBindTexture(GL_TEXTURE_2D, 0));
}
