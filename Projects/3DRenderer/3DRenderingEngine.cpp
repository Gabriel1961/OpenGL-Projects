#include "3DRenderingEngine.h"
#include "../OpenGLWrapper/Common.h"
#include "./RenderObject.h"
#include "../ScreenSize.h"
#include "../../OpenGLWrapper/vendor/imgui/ImguiLib.h"
#include <fstream>
#include <thread>
#include "../src/Input/Input.h"
using namespace std;
using namespace glm;
vec3 camPos;
mat4 projMat;

RenderObject pot;
RenderObject sun;
vec3 CubePos = { 10,10,-4 };
vec4 PotColor = { 1,0,1,1 };
vec4 LightColor = { 1,1,1,1 };
thread* inputThread;

struct CameraRotation
{
	float AngleX = 0;
	float AngleY = 0;
};

CameraRotation CamRotation;
const double camSpeed = 0.1;
GLFWwindow* window;
void SetupInput()
{
	//glfwSetInputMode(window, GLFW_CURSOR, GLFW_CURSOR_DISABLED); // seteaza cursorul lockat pe window
	Input::Init(window);
	inputThread = new std::thread([&] {
		while (!glfwWindowShouldClose(window))
		{
			if (glfwGetKey(window, GLFW_KEY_SPACE))
				camPos.y += camSpeed;
			if (glfwGetKey(window, GLFW_KEY_LEFT_CONTROL))
				camPos.y -= camSpeed;
			if (glfwGetKey(window, GLFW_KEY_A))
			{
				camPos.x -= cos(CamRotation.AngleX) * camSpeed;
				camPos.z -= sin(CamRotation.AngleX) * camSpeed;
			}
			if (glfwGetKey(window, GLFW_KEY_D))
			{
				camPos.x += cos(CamRotation.AngleX) * camSpeed;
				camPos.z += sin(CamRotation.AngleX) * camSpeed;
			}
			if (glfwGetKey(window, GLFW_KEY_S))
			{
				camPos.y -= sin(CamRotation.AngleY) * camSpeed;
				camPos.x -= sin(CamRotation.AngleX) * camSpeed;
				camPos.z += cos(CamRotation.AngleX) * camSpeed;
			}
			if (glfwGetKey(window, GLFW_KEY_W))
			{
				camPos.y += sin(CamRotation.AngleY) * camSpeed;
				camPos.x += sin(CamRotation.AngleX) * camSpeed;
				camPos.z -= cos(CamRotation.AngleX) * camSpeed;
			}
			std::this_thread::sleep_for(std::chrono::milliseconds(10));
		}
		});

	Input::Mouse::MouseMove += [](double speedX, double speedY) {
		if (abs(speedX) > 1 || abs(Input::Mouse::SpeedY) > 1)
			return;
		CamRotation.AngleX += speedX;
		CamRotation.AngleY += speedY;
		if (CamRotation.AngleY > pi / 2)
			CamRotation.AngleY = pi / 2;
		if (CamRotation.AngleY < -pi / 2)
			CamRotation.AngleY = -pi / 2;
	};
}

void RenderingEngine::Start(GLFWwindow* win)
{
	window = win;
	camPos = glm::vec3(0, 2.5, 3.6);
	SetupInput();

	float fov = pi / 4;
	float zNear = 0.1;
	float zFar = 1000;

	projMat = glm::perspective(fov, 1.0f, zNear, zFar);
	glEnable(GL_DEPTH_TEST);
	glDepthFunc(GL_LESS);

	pot.ReadObj(MODEL_PATH "Teapot.obj");
	pot.shader = new Shader(SHADER_PATH "Teapot.shader");

	sun.ReadObj(MODEL_PATH "Cube.obj");
	sun.shader = new Shader(SHADER_PATH "Sun.shader");
	pot.transformGroup->Transforms().push_back(ScaleTransform(0.1, 0.1, 0.1));
	sun.transformGroup->Transforms().push_back(ScaleTransform(0.1, 0.1, 0.1));
}

void RenderingEngine::Render()
{
	ImGui::SliderFloat3("camPos", &camPos.x, -10, 100);
	pot.shader->SetUniformMat4f("Projection", projMat);
	pot.shader->SetUniformMat4f("Model", pot.transformGroup->GetMatrix());
	pot.shader->SetUniform3f("camPos", camPos);
	pot.shader->SetUniform4f("Color", PotColor);
	pot.shader->SetUniform4f("LightColor", LightColor);
	pot.shader->SetUniform3f("LightPosition", CubePos);
	pot.shader->SetUniform2f("u_CameraAngle", CamRotation.AngleX, CamRotation.AngleY);
	ImGui::SliderFloat3("LightPosition", &CubePos.x, -10, 10);
	ImGui::SliderFloat4("LightColor", &LightColor.x, 0, 1);
	ImGui::SliderFloat4("PotColor", &PotColor.x, 0, 1);
	pot.Render();

	sun.shader->SetUniformMat4f("Projection", projMat);
	sun.shader->SetUniform4f("Color", LightColor);
	sun.shader->SetUniformMat4f("Model", sun.transformGroup->GetMatrix());
	sun.shader->SetUniform3f("camPos", camPos - CubePos);
	sun.Render();
}

void RenderingEngine::Update()
{
}

void RenderingEngine::Destroy()
{
	inputThread->join();
}
