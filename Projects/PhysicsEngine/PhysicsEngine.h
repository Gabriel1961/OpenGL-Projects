#pragma once
#include "../../OpenGLWrapper/Common.h"
#include "../../OpenGLWrapper/VertexArray.h"
#include "../../OpenGLWrapper/VertexBuffer.h"
#include "../../OpenGLWrapper/IndexBuffer.h"
#include "../../OpenGLWrapper/Shader.h"
#include "../../OpenGLWrapper/Renderer.h"
#include "../../OpenGLWrapper/Texture.h"
#include "../../Dependencies/Action/Action.h"

#include "../../Action/Action.h"
#include "../../ScreenSize.h"
#include <thread>
#include <DebugLine.h>

#define BASE_DIR "Projects/PhysicsEngine/"
#define SHADER_DIR "Projects/PhysicsEngine/Resources/Shaders/"

struct CameraRotation
{
	float AngleX = 0;
	float AngleY = 0;
};

static void PhysicsCheck();

class PhysicsEngine
{
private:
	static std::thread* inputThread;
	static std::thread* fixedUpdateThread;
public:
	static DebugLine* debugLine;
	static const int FixedUpdateDelay = 8;
	static inline float DeltaTime() { return FixedUpdateDelay * 1e-3; }
	inline static float GetAspectRatio() { return AspectR; }
	PhysicsEngine() = delete;
	static void Start(GLFWwindow* window);
	static void Render();
	static void FixedUpdate();
};

#include "Collliders/Transform.h"
#include "Collliders/RigidBody.h"
#include "./Collliders/Collider.h"


