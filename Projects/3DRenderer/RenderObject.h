#pragma once
#include <string>
#include "./3DRenderingEngine.h"
class RenderObject
{
public:

	RenderObject() {}
	RenderObject(const std::string& path, Shader* shader) :shader(shader) { ReadObj(path); }

	VertexBuffer* vb = 0;
	IndexBuffer* ib = 0;
	Shader* shader = 0;
	VertexArray* va = 0;
	std::vector < Texture*> textures;
	TransformGroup* transformGroup = new TransformGroup();
	~RenderObject();
	void ReadObj(const std::string& path);
	void Render();
};