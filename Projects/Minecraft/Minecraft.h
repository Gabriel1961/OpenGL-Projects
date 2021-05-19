#pragma once
#include "../../OpenGLWrapper/Common.h"
#include "../../OpenGLWrapper/VertexArray.h"
#include <thread>

struct CameraRotation
{
	float AngleX = 0;
	float AngleY = 0;
};

class Minecraft
{
private:
 	static std::thread* inputThread;
public:
	Minecraft() = delete;
	static void Start(GLFWwindow* window,float AspectR,int Window_Width,int Window_Height);
	static void Render();
};



