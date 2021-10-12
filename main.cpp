#include "ScreenSize.h"
#include "Common.h"
#include <fstream>
#include <string>
#include <sstream>
#include <Math.h>
#include <conio.h>
#include <iomanip>
#define _USE_MATH_DEFINES

#include "RayTracerGPU/GPURayTracer.h"
#include "VertexBuffer.h"
#include "IndexBuffer.h"
#include "Shader.h"
#include "VertexArray.h"
#include "Renderer.h"
#include "Texture.h"
#include "Text/Text.h"

int main(void)
{
	GLFWwindow* window;
	/* Initialize the library */
	if (!glfwInit())
		return -1;
	/* Create a windowed mode window and its OpenGL context */
	window = glfwCreateWindow(Window_Width, Window_Height, "OpenGLPractice", NULL, NULL);
	if (!window)
	{
		glfwTerminate();
		return -1;
	}
	/* Make the window's context current */
	glfwMakeContextCurrent(window);
	{
		if (glewInit() != GLEW_OK) {
			std::cout << "Failed to initilize glew\n" << endl;
			ASSERT(false);
		}
		Imgui_Start(window);
		/// ///////////////////////////////////////////
		double lastTime = glfwGetTime();
		int nbFrames = 0;
		/// ///////////////////////////////////////////
		gc(glBlendFunc(GL_SRC_ALPHA, GL_ONE_MINUS_SRC_ALPHA));
		gc(glEnable(GL_BLEND));
		GPURayTracer::Start(window);
		while (!glfwWindowShouldClose(window))
		{
			GPURayTracer::PreRender();
#pragma region FrameCounter
			double currentTime = glfwGetTime();
			nbFrames++;
			if (currentTime - lastTime >= 1.0) { // If last prinf() was more than 1 sec ago
				// printf and reset timer
				glfwSetWindowTitle(window, std::to_string(nbFrames).c_str());
				nbFrames = 0;
				lastTime += 1.0;
			}
#pragma endregion
			Renderer::Clear();
			Imgui_NewFrame();
			/* Render here */
			GPURayTracer::Render();

			Imgui_Render();
			/* Swap front and back buffers */
			glfwSwapBuffers(window);

			/* Poll for and process events */
			glfwPollEvents();
		}
		GPURayTracer::Terminate();
	}
	Imgui_Close();

	glfwTerminate();
	return 0;
}
