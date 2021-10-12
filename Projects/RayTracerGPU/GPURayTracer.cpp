#include "GPURayTracer.h"
#include "GPURayTracerImpl.h"
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

vector <Sphere> spheres;
vector <Material> materials;

Material diffuse = Material(vec4(1, 0, 1, 1));

void GPURayTracer::Start(GLFWwindow*)
{
	tex = new Texture(TEX_WIDTH, TEX_HEIGHT);
	compute = new ComputeShader("Projects/RayTracerGPU/Compute.comp");
	q = new Quad(true, 0);

	materials.push_back(diffuse);
	spheres.push_back(Sphere(1, vec4{ 0,0,-10, 0 }, 0));

	compute->AddUniformBlockList("SphereUniform", spheres);
	compute->AddUniformBlockList("MaterialUniform", materials);
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
