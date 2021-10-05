#pragma once
struct GLFWwindow;
class RayTracer
{
public:
	static void Start(GLFWwindow*);
	static void Render();
	static void Terminate();
};