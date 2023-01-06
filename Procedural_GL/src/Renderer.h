#pragma once

#include <GL/glew.h>

#include "VertexArray.h"
#include "IndexBuffer.h"
#include "Shader.h"


#define ASSERT(x) if (!(x)) __debugbreak();
#define GLCall(x) GLClearError();\
    x;\
    ASSERT(GLErrorLogCall(#x, __FILE__,__LINE__))


void GLClearError();
bool GLErrorLogCall(const char* function, const char* file, int line);

class Renderer
{
public:
    void Clear() const;
    void Draw(const VertexArray& va, const IndexBuffer& ib, const Shader& shader) const;
    void DrawInstances(const VertexArray& va, const IndexBuffer& ib, const Shader& shader,const int nmbrOfIndices, const int instanceCount) const;
};