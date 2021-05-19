#pragma once
#include "Common.h"
#include <unordered_map>
class Shader
{
private:
	int m_RendererID;
	const char* m_FilePath;
	std::unordered_map<std::string, int>m_UniformLocationCache;
public:
	int programID;
	Shader() {}
	Shader(const char* filepath);
	~Shader();
	void Bind()const;
	void Unbind()const;
	const char* GetFilePath() { return m_FilePath; }
	//Uniform
	void SetUniform1f(std::string  name, float v0);
	void SetUniform2f(std::string  name, float v0, float v1);
	void SetUniform3f(std::string name, float v0, float v1, float v2);
	void SetUniform3f(std::string name, glm::vec3 vec);
	void SetUniform4f(std::string name, float v0, float v1, float v2, float v3);
	void SetUniformMat4f(std::string name, glm::mat4&);
	void SetUniformMat3f(std::string name, glm::mat3& matrix);
	void SetUniformMat2f(std::string name, glm::mat2& matrix);
private:
	int GetUniformLocation(const std::string& name);
};