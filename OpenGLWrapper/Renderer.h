#pragma once
#include "Shader.h"
#include "Indexbuffer.h"
#include "VertexArray.h"

class Renderer
{
public:
	static void Draw(const VertexArray& va,const IndexBuffer& ib,const Shader& shader, unsigned int DrawMode = GL_TRIANGLES);
	static inline void Clear(float r=0,float g=0,float b=0,float a=1) {
		glClearColor(r, g, b, a);
		glClear(GL_COLOR_BUFFER_BIT);
	}
};