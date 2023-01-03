#pragma once

#include <string>
#include <unordered_map>
#include "glm/glm.hpp"
struct ShaderProgramSource
{
	std::string VertexSource;
	std::string FragmentSource;
};

class Shader
{
private:
	std::string m_vertexFilePath;
	std::string m_fragmentFilePath;

	unsigned int m_RenderingID;
	//caching for uniforms speeds up the function to get uniform location by not needing to call glGetUniformLocation every time
	std::unordered_map<std::string, int> m_UniformLocationCache;

public:

	Shader(const std::string& vertexShaderfilepath, const std::string& fragmentShaderfilepath);
	~Shader();


	void Bind()const ;
	void Unbind() const;

	//Set uniforms
	void SetUniform1i(const std::string& name, int value);
	void SetUniform1f(const std::string& name, float value);
	void SetUniform4f(const std::string& name, float v0, float v1, float v2, float v3);
	void SetUniformMat4f(const std::string& name, const glm::mat4& matrix);


private:

	ShaderProgramSource ParseShader(const std::string& vertexShaderfilepath, const std::string& fragmentShaderfilepath);
	unsigned int CompileShader(unsigned int type, const std::string& source);
	unsigned int CreateShader(const std::string& vertexShader, const std::string& fragmentShader);

	int GetUniformLocation(const std::string& name);
};