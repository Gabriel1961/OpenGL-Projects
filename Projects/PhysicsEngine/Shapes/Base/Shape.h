#pragma once
#include <functional>
#include "../../PhysicsEngine.h"
/// Base class for any shape that needs rendering
class RenderableShape
{
public:
	VertexBuffer* vb = 0;
	IndexBuffer* ib = 0;
	Shader* shader = 0;
	VertexArray* va = 0;
	GLenum drawMode;
	std::function<void()> setUniforms;
	~RenderableShape()
	{
		delete vb;
		delete ib;
		delete shader;
		delete va;
	}
	virtual void Render()
	{
		setUniforms();
		Renderer::Draw(*va, *ib, *shader, drawMode);
	}
};