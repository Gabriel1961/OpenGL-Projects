#include "Common.h"
#include "Shader.h"
#include "fstream"
#include "sstream"
#include "string"
void ShaderCompilationErrorCheck(unsigned int shaderID)
{
	int CompileStatus;
	gc(glGetShaderiv(shaderID, GL_COMPILE_STATUS, &CompileStatus));
	if (CompileStatus == GL_FALSE) {
		int LogLenght;
		int Type;
		gc(glGetShaderiv(shaderID, GL_SHADER_TYPE, &Type);
		gc(glGetShaderiv(shaderID, GL_INFO_LOG_LENGTH, &LogLenght));
		char* Message = (char*)alloca(sizeof(char)*LogLenght));
		gc(glGetShaderInfoLog(shaderID, LogLenght, &LogLenght, Message));
		std::cout << "[Shader Error] Failed to compile " << (Type == GL_VERTEX_SHADER ? "Vertex Shader - " : "Fragment Shader - ") << Message << '\n';
	}
}
void ShaderLinkErrorCheck(unsigned int program)
{
	int ProgramLink;
	glGetShaderiv(program, GL_LINK_STATUS, &ProgramLink);
	if (ProgramLink == GL_FALSE)
	{
		char Message[1024];
		int size;
		glGetShaderInfoLog(program, size, &size, Message);
		std::cout << "Failed to link program " << Message << std::endl;
	}
}
unsigned int CreateProgam(std::string filepath)
{
	gc(unsigned int program = glCreateProgram());
	gc(unsigned int vs = glCreateShader(GL_VERTEX_SHADER));
	gc(unsigned int fs = glCreateShader(GL_FRAGMENT_SHADER));

	enum class e_Shader {
		none = -1, vs = 1, fs = 2
	}ShaderType;
	ShaderType = e_Shader::none;
	std::stringstream ssvs, ssfs;
	std::ifstream fin(filepath);
	if (!fin.is_open()) { std::cout << "[Error]: Fatal error shader : " << filepath << " not found!"; ASSERT(false); }
	std::string line;
	while (std::getline(fin, line)) {
		if (line == "#Vertex Shader")
		{
			ShaderType = e_Shader::vs; continue;
		}
		if (line == "#Fragment Shader")
		{
			ShaderType = e_Shader::fs; continue;
		}
		if (ShaderType == e_Shader::vs)
		{
			ssvs << line << '\n';
		}
		else if (ShaderType == e_Shader::fs)
		{
			ssfs << line << '\n';
		}
	}
	std::string vs_source = ssvs.str();
	std::string fs_source = ssfs.str();
	const char* vs_src = vs_source.c_str();
	const char* fs_src = fs_source.c_str();
	gc(glShaderSource(vs, 1, &vs_src, nullptr));
	gc(glShaderSource(fs, 1, &fs_src, nullptr));
	gc(glCompileShader(vs));
	gc(glCompileShader(fs));

	ShaderCompilationErrorCheck(vs);
	ShaderCompilationErrorCheck(fs);

	gc(glAttachShader(program, vs));
	gc(glAttachShader(program, fs));
	gc(glLinkProgram(program));

	ShaderLinkErrorCheck(program);

	gc(glValidateProgram(program));

	gc(glDeleteShader(vs));
	gc(glDeleteShader(fs));
	return program;
}

Shader::Shader(const char* filepath) : m_RendererID(0),m_FilePath(filepath)
{
	m_RendererID = CreateProgam(filepath);
	gc(glUseProgram(m_RendererID));
	programID = m_RendererID;
}

Shader::~Shader()
{
	gc(glDeleteProgram(m_RendererID));
}

void Shader::Bind() const
{
	gc(glUseProgram(m_RendererID));
}

void Shader::Unbind() const
{
	gc(glUseProgram(0));
}
void Shader::SetUniform2f(std::string name, float v0, float v1)
{
	Bind();
	gc(glUniform2f(GetUniformLocation(name), v0, v1));
}
void Shader::SetUniform3f(std::string name, float v0, float v1, float v2)
{
	Bind();
	gc(glUniform3f(GetUniformLocation(name), v0, v1, v2));
}
void Shader::SetUniform3f(std::string name, glm::vec3 vec)
{
	Bind();
	gc(glUniform3f(GetUniformLocation(name), vec.x, vec.y, vec.z));
}
void Shader::SetUniform4f(std::string name, float v0, float v1, float v2, float v3)
{
	Bind();
	gc(glUniform4f(GetUniformLocation(name), v0, v1, v2, v3)); 
}
void Shader::SetUniform1f(std::string name, float v0)
{
	Bind();
	gc(glUniform1f(GetUniformLocation(name), v0));
}
void Shader::SetUniformMat4f(std::string name, glm::mat4& matrix)
{
	Bind();
	gc(glUniformMatrix4fv(GetUniformLocation(name), 1, false, &matrix[0][0]));
}
void Shader::SetUniformMat3f(std::string name, glm::mat3& matrix)
{
	Bind();
	gc(glUniformMatrix3fv(GetUniformLocation(name), 1, false, &matrix[0][0]));
}
void Shader::SetUniformMat2f(std::string name, glm::mat2& matrix)
{
	Bind();
	gc(glUniformMatrix2fv(GetUniformLocation(name), 1, false, &matrix[0][0]));
}
int Shader::GetUniformLocation(const std::string& name)
{
	if (m_UniformLocationCache.find(name) != m_UniformLocationCache.end())
		return m_UniformLocationCache[name];
	gc(int location = glGetUniformLocation(programID, name.c_str()));
	if (location == -1)
		std::cout << "[Warning]: uniform " << name << "does not exist!\n";
	m_UniformLocationCache[name] = location;
	return location;
}

