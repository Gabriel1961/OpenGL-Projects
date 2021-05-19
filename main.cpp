#include "OpenGLWrapper/Common.h"
#include <fstream>
#include <future>
#include <string>
#include <sstream>
#include <Math.h>
#include <conio.h>
#include <iomanip>
#define _USE_MATH_DEFINES


#include "OpenGLWrapper/VertexBuffer.h"
#include "OpenGLWrapper/IndexBuffer.h"
#include "OpenGLWrapper/Shader.h"
#include "OpenGLWrapper/VertexArray.h"
#include "OpenGLWrapper/Renderer.h"
#include "OpenGLWrapper/Texture.h"
#include "Dependencies/Action/Action.h"

#include "Projects/Minecraft/Minecraft.h"
#define Window_Width 800
#define Window_Height 600
const float AspectR = (float)Window_Height / Window_Width;


int main(void)
{
    #pragma region Init
	GLFWwindow* window;
	/* Initialize the library */
	if (!glfwInit())
		return -1;
	/* Create a windowed mode window and its OpenGL context */
	window = glfwCreateWindow(Window_Width, Window_Height, "Hello World", NULL, NULL);
	if (!window)
	{
		glfwTerminate();
		return -1;
	}
	/* Make the window's context current */
	glfwMakeContextCurrent(window);
	
	if (glewInit() != GLEW_OK) {
		std::cout << "Failed to initilize glew\n" << std::endl;
		ASSERT(false);
	}
	
	/// ///////////////////////////////////////////
	int frameKounter = 0; 
	double previousTime = glfwGetTime();
	/// ///////////////////////////////////////////

	#pragma endregion
	
	Minecraft::Start(window,AspectR,Window_Width,Window_Height);
	
	while (!glfwWindowShouldClose(window))
	{
		#pragma region Common Stuff

		frameKounter++;
		double currentTime = glfwGetTime();
		if (currentTime - previousTime >= 1.0)
		{
			auto str = std::to_string(frameKounter);
			glfwSetWindowTitle(window, str.c_str());
			frameKounter = 0;
			previousTime = currentTime;
		}
		#pragma endregion
		
		/* Render here */
		Renderer::Clear();
		
		Minecraft::Render();

		/* Swap front and back buffers */
		glfwSwapBuffers(window);

		/* Poll for and process events */
		glfwPollEvents();
	}

	glfwTerminate();
	return 0;
}
