#pragma once
#include <string>
class Texture
{
private:
	unsigned int m_RendererID;
	std::string m_FilePath;
	unsigned char* m_LocalBuffer;
	int m_Height, m_Width, m_BPP;
public:
	Texture(const char* _FilePath);
	~Texture();

	void Bind(unsigned int slot=0) const;
	void Unbind() const;

	int  GetHeight()const { return m_Height; };
	int  GetWidth()const { return m_Width; };
};