#include "PhysicsEngine.h"
#include <thread>
#include <cmath>
#include "./Shapes/Circle.h"
#include "../../OpenGLWrapper/Common.h"
#include "./Shapes/Square.h"
#include "./Shapes/Base/PhysicsObject.h"
#include "../../ScreenSize.h"
#include "../../src/Input/Input.h"
#include "vendor/imgui/ImguiLib.h"
using namespace std;
using namespace Object;
using namespace glm;
std::thread* PhysicsEngine::inputThread = nullptr;
std::thread* PhysicsEngine::fixedUpdateThread = nullptr;

DebugLine* PhysicsEngine::debugLine;

CameraRotation CamRotation;
GLFWwindow* window;
vector<PhysicsObject*> physicsObjects;
vector<RenderableShape*> renderShapes;

vec4 lineColor(1);
vec2 lineVec = { 1,1 };
void PhysicsEngine::Start(GLFWwindow* _window)
{
	using namespace glm;
#pragma region Physics
	using namespace chrono_literals;
	fixedUpdateThread = new thread([&]() {
		while (!glfwWindowShouldClose(window))
		{
			std::this_thread::sleep_for(chrono::duration<long long, std::milli>(PhysicsEngine::FixedUpdateDelay));
			PhysicsEngine::FixedUpdate();
		}
		});
#pragma endregion

#pragma region Init
	window = _window;
	Input::Init(window);
	//glEnable(GL_DEPTH_TEST);
	//glDepthFunc(GL_LESS);

	debugLine = new DebugLine({ 0,0 }, { 0, 0 });
	//glfwSetInputMode(window, GLFW_CURSOR, GLFW_CURSOR_DISABLED); // seteaza cursorul lockat pe window
#pragma endregion

#pragma region Input
	inputThread = new std::thread([&] {
		const float accel = 80;
		const float angAccel = 0.1;
		while (!glfwWindowShouldClose(window))
		{
			if (glfwGetKey(window, GLFW_KEY_A))
			{
				physicsObjects[0]->rigibody->AddForce(vec2(-accel, 0));
			}
			if (glfwGetKey(window, GLFW_KEY_D))
			{
				physicsObjects[0]->rigibody->AddForce(vec2(accel, 0));
			}
			if (glfwGetKey(window, GLFW_KEY_W))
			{
				physicsObjects[0]->rigibody->AddForce(vec2(0, accel));
			}
			if (glfwGetKey(window, GLFW_KEY_S))
			{
				physicsObjects[0]->rigibody->AddForce(vec2(0, -accel));
			}
			if (glfwGetKey(window, GLFW_KEY_LEFT))
			{
			}
			if (glfwGetKey(window, GLFW_KEY_RIGHT))
			{
			}
			if (glfwGetKey(window, GLFW_KEY_UP))
			{
				physicsObjects[0]->rigibody->AddRelativeForceAtPoint({ 1,1 }, { 0,0.001 });
			}
			std::this_thread::sleep_for(std::chrono::milliseconds(10));
		}
		});

#pragma endregion

	physicsObjects.push_back(new Square({ 0.1,0.1 }, { 0,0 }, Color::Red(), 10));
	physicsObjects.push_back(new Circle(vec2(0.1), vec2(0.5), Color({ 1,0,1,1 }), 50, 10));
}

void PhysicsEngine::Render()
{
	DebugLine::RenderLines();
	for (auto obj : physicsObjects)
		obj->Render();
	for (auto shape : renderShapes)
		shape->Render();
}

void PhysicsEngine::FixedUpdate()
{
	PhysicsCheck();
}

static void PhysicsCheck()
{
	for (int i = 0; i < physicsObjects.size(); i++)
		for (int j = i + 1; j < physicsObjects.size(); j++)
			if (physicsObjects[i]->collider && physicsObjects[j]->collider)
				physicsObjects[i]->collider->TestCollision(physicsObjects[j]->transform, physicsObjects[j]->collider);
	for (auto obj : physicsObjects)
		obj->FixedUpdate();
}