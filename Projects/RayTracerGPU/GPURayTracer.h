#pragma once
struct GLFWwindow;
class GPURayTracer
{
public:
	static void Start(GLFWwindow*);
	static void PreRender();
	static void Render();
	static void Terminate();
};
