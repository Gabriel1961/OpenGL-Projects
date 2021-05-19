#include "Renderer.h"

void Renderer::Draw(const VertexArray& va, const IndexBuffer& ib, const Shader& shader,unsigned int DrawMode)
{
	shader.Bind();
	va.Bind();
	ib.Bind();
	gc(glDrawElements(DrawMode,ib.GetCount() , ib.GetType(), nullptr));
}
