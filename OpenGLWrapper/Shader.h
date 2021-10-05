#pragma once
#include "Common.h"
#include <unordered_map>
class Shader
{
private:
	int m_RendererID;
	std::string m_FilePath;
	std::unordered_map<std::string, int>m_UniformLocationCache;
public:
	int programID;
	Shader() {}
	Shader(const std::string& filepath);
	~Shader();
	void Bind()const;
	void Unbind()const;
	const std::string& GetFilePath() { return m_FilePath; }
	//Uniform
	void SetUniform1f(const std::string& name, float v0);
	void SetUniform1i(const std::string& name, int val);
	void SetUniform2f(const std::string& name, float v0, float v1);
	void SetUniform3f(const std::string& name, float v0, float v1, float v2);
	void SetUniform3f(const std::string& name, const glm::vec3& vec);
	void SetUniform4f(const std::string& name, float v0, float v1, float v2, float v3);
	void SetUniform4f(const std::string& name, const glm::vec4&);
	void SetUniformMat4f(const std::string& name, const glm::mat4&);
	void SetUniformMat3f(const std::string& name, const glm::mat3& matrix);
	void SetUniformMat2f(const std::string& name, const glm::mat2& matrix);
private:
	int GetUniformLocation(const std::string& name);
};