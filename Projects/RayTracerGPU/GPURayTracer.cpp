#include "GPURayTracer.h"
#include "Common.h"
#include <Renderer.h>
#include <Quad.h>
#include <UniformBlockList.h>
using namespace std;
using namespace glm;
Texture* tex;
ComputeShader* compute;
Quad* q;
Shader* qProg;
const int TEX_WIDTH = Window_Width, TEX_HEIGHT = Window_Height;
struct Pep
{
	vec4 orientation[3];
};

vector <Pep> pep = { {vec4(1,0,1,1),vec4(1),vec4(1)},{vec4(1,0,0,1),vec4(1),vec4(1)} };

void GPURayTracer::Start(GLFWwindow*)
{
	tex = new Texture(TEX_WIDTH, TEX_HEIGHT);
	compute = new ComputeShader("Projects/RayTracerGPU/Compute.comp");
	q = new Quad(true, 0);
	("CameraBlock", pep);
	compute->AddUniformBlockList("CameraBlock", pep);
}

void GPURayTracer::PreRender()
{
	tex->Bind(0);
	compute->Dispatch(TEX_HEIGHT, TEX_WIDTH, 1);
	glMemoryBarrier(GL_SHADER_IMAGE_ACCESS_BARRIER_BIT); // Wait for image to be written to 
}

void GPURayTracer::Render()
{
	q->Render();
}

void GPURayTracer::Terminate()
{
}
