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

class PerlinNoiseProject
{
public:
	static void Render();
	static void Start();
};