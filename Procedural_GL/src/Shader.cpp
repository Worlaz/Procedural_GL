#include "Shader.h"

#include <iostream>
#include <fstream>
#include <string>
#include <sstream>

#include "Renderer.h"

Shader::Shader(const std::string& vertexShaderfilepath, const std::string& fragmentShaderfilepath)
	:m_vertexFilePath{vertexShaderfilepath}, m_fragmentFilePath{fragmentShaderfilepath}, m_RenderingID{0}
{
    ShaderProgramSource source = ParseShader(vertexShaderfilepath, fragmentShaderfilepath);




    m_RenderingID = CreateShader(source.VertexSource, source.FragmentSource);
     
}

Shader::~Shader()
{
    GLCall(glDeleteProgram(m_RenderingID));
}



ShaderProgramSource Shader::ParseShader(const std::string& vertexShaderfilepath, const std::string& fragmentShaderfilepath)
{
    std::ifstream vertexStream(vertexShaderfilepath);
    std::string line;
    std::stringstream ss[2];

    while (getline(vertexStream, line))
    {
        ss[0] << line << "\n";

    }
    std::ifstream fragmentStream(fragmentShaderfilepath);

    line;
    while (getline(fragmentStream, line))
    {
        ss[1] << line << "\n";

    }

    return { ss[0].str(),ss[1].str() };

}

unsigned int Shader::CompileShader(unsigned int type, const std::string& source)
{
    unsigned int id = glCreateShader(type);
    const char* src = source.c_str();
    GLCall(glShaderSource(id, 1, &src, nullptr));
    GLCall(glCompileShader(id));

    int result;
    GLCall(glGetShaderiv(id, GL_COMPILE_STATUS, &result));
    if (result == GL_FALSE)
    {
        int length;
        GLCall(glGetShaderiv(id, GL_INFO_LOG_LENGTH, &length));
        char* message = (char*)alloca(length * sizeof(char));
        GLCall(glGetShaderInfoLog(id, length, &length, message));
        std::cout << "Failed to compile" << (type == GL_VERTEX_SHADER ? "vertex" : "fragment") << "shader!" << std::endl;
        std::cout << message << std::endl;
        GLCall(glDeleteShader(id));
        return 0;


    }


    return id;

}

unsigned int Shader::CreateShader(const std::string& vertexShader, const std::string& fragmentShader)
{
    unsigned int program = glCreateProgram();
    unsigned int vs = CompileShader(GL_VERTEX_SHADER, vertexShader);
    unsigned int fs = CompileShader(GL_FRAGMENT_SHADER, fragmentShader);

    GLCall(glAttachShader(program, vs));
    GLCall(glAttachShader(program, fs));
    GLCall(glLinkProgram(program));
    GLCall(glValidateProgram(program));

    GLCall(glDeleteShader(vs));
    GLCall(glDeleteShader(fs));

    return program;

}

void Shader::Bind() const
{
    GLCall(glUseProgram(m_RenderingID));
}

void Shader::Unbind() const
{
    GLCall(glUseProgram(0));
}

void Shader::SetUniform1i(const std::string& name, int value)
{
    GLCall(glUniform1i(GetUniformLocation(name),value));
}

void Shader::SetUniform1f(const std::string& name, float value)
{
    GLCall(glUniform1f(GetUniformLocation(name), value));
}

void Shader::SetUniform3fv(const std::string& name, glm::vec3& value)
{
    GLCall(glUniform3fv(GetUniformLocation(name),1, glm::value_ptr(value)));
}

void Shader::SetUniform4f(const std::string& name, float v0, float v1, float v2, float v3)
{
    GLCall(glUniform4f(GetUniformLocation(name),v0, v1,v2,v3 ));
}

void Shader::SetUniformMat4f(const std::string& name, const glm::mat4& matrix)
{
    GLCall(glUniformMatrix4fv(GetUniformLocation(name), 1, GL_FALSE, &matrix[0][0]));
}

int Shader::GetUniformLocation(const std::string& name)
{
    //Check if the uniform is cached
    if (m_UniformLocationCache.find(name) != m_UniformLocationCache.end())
    {
        return m_UniformLocationCache[name];
    }
    
  GLCall( int location = glGetUniformLocation(m_RenderingID, name.c_str()));
   if (location == -1)
   {
       std::cout << "Warning: uniform " << name << " dosn't exist!" << std::endl;
   }
   else
   {
       m_UniformLocationCache[name] = location;
   }
   return location;
}
