#include "Minecraft.h"
#include <thread>
#include <cmath>
#include "../../OpenGLWrapper/Common.h"
#include "../../src/Input/Input.h"
#include "../../OpenGLWrapper/VertexBuffer.h"
#include "../../OpenGLWrapper/IndexBuffer.h"
#include "../../OpenGLWrapper/Shader.h"
#include "../../OpenGLWrapper/VertexArray.h"
#include "../../OpenGLWrapper/Renderer.h"
#include "../../OpenGLWrapper/Texture.h"
#include "../../Dependencies/Action/Action.h"
#include "./Blocks/BlockFace.h"
using namespace std;
#define TEXTURE_DIR std::string("Projects/Minecraft/Resources/Textures/")
#define SHADER_DIR std::string("Projects/Minecraft/Resources/Shaders/")
#define CURRENT_DIR std::string("Projects/Minecraft/")

std::thread* Minecraft::inputThread = nullptr;
glm::vec3 CamPos = glm::vec3(0, 0, -10);
CameraRotation CamRotation;
const double camSpeed = 0.1;

GLFWwindow* window;

vector<BlockFace> blockFaces;
glm::vec3 sunVec;
void Minecraft::Start(GLFWwindow* _window, float AspectR, int Window_Width, int Window_Height)
{
	sunVec = glm::vec3(0, -1, 0);
	window = _window;
	Input::Init(window, Window_Width, Window_Height);
	CamPos = glm::vec3(0, 0, -10);

	float fov = pi / 2;
	float zNear = 0.1;
	float zFar = 100;
	glm::mat4 projMat(
		AspectR * 1 / glm::tan(fov / 2), 0, 0, 0,
		0, 1.0f / glm::tan(fov / 2), 0, 0,
		0, 0, zFar / (zFar - zNear), 1,
		0, 0, -zNear * zFar / (zFar - zNear), 0
	);

	glEnable(GL_DEPTH_TEST);
	glDepthFunc(GL_LESS);

	auto CubeTexture = new Texture(TEXTURE_DIR + string("grass.png"));
	auto CubeShader = new Shader(SHADER_DIR + string("Cube.shader"));

	blockFaces.push_back(BlockFace(FaceOrientation::Front, *CubeTexture, *CubeShader, glm::vec3(0, 1, 0), CamPos, CamRotation, projMat, sunVec));
	blockFaces.push_back(BlockFace(FaceOrientation::Down, *CubeTexture, *CubeShader, glm::vec3(0, 1, 0), CamPos, CamRotation, projMat, sunVec));
	for (int i = 0; i < 10; i++)
		for (int j = 0; j < 10; j++)
		{
			blockFaces.push_back(BlockFace(FaceOrientation::Up, *CubeTexture, *CubeShader, glm::vec3(i * 2.01, 0, j * 2.01), CamPos, CamRotation, projMat, sunVec));
		}


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
	for (auto face : blockFaces)
		face.Render();
}
