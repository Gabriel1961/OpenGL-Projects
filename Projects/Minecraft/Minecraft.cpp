#include "Minecraft.h"
#include <thread>
#include "../../OpenGLWrapper/Common.h"
#include "../../src/Input/Input.h"
#include "../../OpenGLWrapper/VertexBuffer.h"
#include "../../OpenGLWrapper/IndexBuffer.h"
#include "../../OpenGLWrapper/Shader.h"
#include "../../OpenGLWrapper/VertexArray.h"
#include "../../OpenGLWrapper/Renderer.h"
#include "../../OpenGLWrapper/Texture.h"
#include "../../Dependencies/Action/Action.h"

std::thread* Minecraft::inputThread = nullptr;
glm::vec3 CamPos = glm::vec3(0,0,-10);
CameraRotation CamRotation;

VertexArray* CubeVertexArray = NULL;
VertexBuffer* CubeVertexBuffer = NULL;
IndexBuffer* CubeIndexBufferFill = NULL;
IndexBuffer* CubeIndexBufferFrame = NULL;
Shader* shader = NULL;
GLFWwindow* window;

const double camSpeed = 0.1;

void Minecraft::Start(GLFWwindow* _window,float AspectR,int Window_Width,int Window_Height)
{
	window = _window;
	Input::Init(window, Window_Width, Window_Height);
	CubeVertexArray = new VertexArray();
	CamPos = glm::vec3(0, 0, -10);
	
	glm::mat3 mat_MVP
	{
		AspectR,0,0,
		0,      1,0,
		0,      0,AspectR
	};
	
	float CubData[]{
	-1,-1,-1, // 0
	 1,-1,-1, // 1
	 1,-1, 1, // 2
	-1,-1, 1, // 3 
	-1, 1,-1, // 4 
	 1, 1,-1, // 5 
	 1, 1, 1, // 6
	-1, 1, 1  // 7 
	};
	
	uchar CubeFillIndexData[]{
	0,4,5,
	0,5,1,

	1,5,6,
	1,6,2,

	2,6,7,
	2,7,3,

	3,7,4,
	3,4,0,

	0,3,2,
	0,2,1,

	4,7,6,
	4,6,5
	};
	
	uchar CubeFrameIndexData[]{
	4,5,
	5,6,
	6,7,
	7,4
	};

	ASSERT(sizeof(*CubeFillIndexData) == 1);
	CubeVertexBuffer = new VertexBuffer(sizeof(CubData), CubData);
	CubeIndexBufferFill = new IndexBuffer(sizeof(CubeFillIndexData) / sizeof(*CubeFillIndexData), CubeFillIndexData, GL_UNSIGNED_BYTE);
	CubeIndexBufferFrame = new IndexBuffer(sizeof(CubeFrameIndexData) / sizeof(*CubeFrameIndexData), CubeFrameIndexData, GL_UNSIGNED_BYTE);
	VertexBufferLayout layout;
	layout.Push<float>(3);
	CubeVertexArray->AddLayout(*CubeVertexBuffer, layout);
	shader =new Shader("res/shaders/Cube.shader");
	shader->SetUniform3f("u_CameraPos", CamPos);
	shader->SetUniformMat3f("u_mvp", mat_MVP);

	glfwSetInputMode(window, GLFW_CURSOR, GLFW_CURSOR_DISABLED); // seteaza cursorul lockat pe window

	#pragma region Input
 	inputThread = new std::thread([&] {
		while (!glfwWindowShouldClose(window))
		{
			if (glfwGetKey(window, GLFW_KEY_SPACE))
				CamPos.y += camSpeed;
			if (glfwGetKey(window, GLFW_KEY_LEFT_CONTROL))
				CamPos.y -= camSpeed;
			if (glfwGetKey(window, GLFW_KEY_A))
			{
				CamPos.x -= cos(CamRotation.AngleX) * camSpeed;
				CamPos.z -= sin(CamRotation.AngleX) * camSpeed;
			}
			if (glfwGetKey(window, GLFW_KEY_D))
			{
				CamPos.x += cos(CamRotation.AngleX) * camSpeed;
				CamPos.z += sin(CamRotation.AngleX) * camSpeed;
			}
			if (glfwGetKey(window, GLFW_KEY_W))
			{
				CamPos.y -= sin(CamRotation.AngleY) * camSpeed;
				CamPos.x -= sin(CamRotation.AngleX) * camSpeed;
				CamPos.z += cos(CamRotation.AngleX) * camSpeed;
			}
			if (glfwGetKey(window, GLFW_KEY_S))
			{
				CamPos.y += sin(CamRotation.AngleY) * camSpeed;
				CamPos.x += sin(CamRotation.AngleX) * camSpeed;
				CamPos.z -= cos(CamRotation.AngleX) * camSpeed;
			}
			std::this_thread::sleep_for(std::chrono::milliseconds(10));
		}
		});

	Input::Mouse::MouseMove += [](double speedX, double speedY) {
		if (abs(speedX) > 1 || abs(Input::Mouse::SpeedY) > 1)
			return;
		CamRotation.AngleX -= speedX;
		CamRotation.AngleY -= speedY;
		if (CamRotation.AngleY > pi / 2)
			CamRotation.AngleY = pi / 2;
		if (CamRotation.AngleY < -pi / 2)
			CamRotation.AngleY = -pi / 2;
	};
#pragma endregion
}

void Minecraft::Render()
{
	shader->SetUniform3f("u_CameraPos", CamPos);
	shader->SetUniform2f("u_CameraAngle", CamRotation.AngleX, CamRotation.AngleY);
	for (int i = 0; i < 20; i++)
	{
		for (int j = 0; j < 20; j++)
		{
			shader->SetUniform3f("u_CubePos", glm::vec3(j * 2, 0, i * 2));
			shader->SetUniform4f("u_CubeColor", 0, 1, 0, 1);
			Renderer::Draw(*CubeVertexArray, *CubeIndexBufferFill, *shader, GL_TRIANGLES);
		}
	}
	for (int i = 0; i < 20; i++)
	{
		for (int j = 0; j < 20; j++)
		{
			shader->SetUniform3f("u_CubePos", glm::vec3(j * 2, 0, i * 2));
			shader->SetUniform4f("u_CubeColor", 0, 0, 0, 1);
			Renderer::Draw(*CubeVertexArray, *CubeIndexBufferFrame, *shader, GL_LINES);
		}
	}

}
