#pragma once
#include "../../OpenGLWrapper/Common.h"
class Testing3D
{
public:
	static void Start(GLFWwindow* window, int Height, int Width, double AspectR);
	static void Render();
};