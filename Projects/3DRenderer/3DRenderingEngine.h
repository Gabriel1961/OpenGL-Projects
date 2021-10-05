#pragma once
#include "../OpenGLWrapper/VertexArray.h"
#include "../OpenGLWrapper/VertexBuffer.h"
#include "../OpenGLWrapper/VertexBufferLayout.h"
#include "../OpenGLWrapper/Shader.h"
#include "../OpenGLWrapper/Renderer.h"
#include "../OpenGLWrapper/Transform.h"
#define BASE_PATH "Projects/3DRenderer/"
#define SHADER_PATH BASE_PATH "Shaders/"
#define MODEL_PATH BASE_PATH "Models/"
class RenderingEngine
{
public:
	static void Start(GLFWwindow* window);
	static void Render();
	static void Update();
	static void Destroy();
};