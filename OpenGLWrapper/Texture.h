#pragma once
#include <string>
#include <Common.h>
#include <../ScreenSize.h>
class Texture
{
private:
	unsigned int m_RendererID;
	std::string m_FilePath;
	unsigned char* m_LocalBuffer;
	int m_Height, m_Width, m_BPP;
public:
	Texture(std::string _FilePath);
	Texture();
	Texture(uint heigth, uint witdth, GLenum format = GL_RGBA32F, GLenum antiAliasing = GL_NEAREST);
	Texture(ARGBColor* buffer, GLenum antiAliasing = GL_NEAREST);
	~Texture();

	void Bind(unsigned int slot = 0) const;
	void Unbind() const;

	int  GetHeight()const { return m_Height; }
	int  GetWidth()const { return m_Width; }
};
