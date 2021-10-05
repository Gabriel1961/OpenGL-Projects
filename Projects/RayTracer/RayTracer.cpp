#include "RayTracer.h"
#include <Common.h>
#include <Logger.h>
#include <DebugLine.h>
#include <PixelDisplay.h>
#include "RayTracingImpl.h"
#include <thread>
#include <vector>
using namespace glm;

PixelDisplay* pd;
GLFWwindow* window;
ObjectList objList;
std::vector<std::thread> threadPool;
Camera cam(1, 4);
int maxDepth = 50;
void Update(int startLine, int endLine);

void DispatchThreads(int threads)
{// Each thread makes a set of lines
	int indvWorkLoad = Window_Height / threads;
	int offset = 0;
	for (int i = 0; i < threads - 1; i++)
	{
		threadPool.push_back(std::thread(Update, offset, offset + indvWorkLoad));
		offset += indvWorkLoad;
	}
	threadPool.push_back(std::thread(Update, offset, Window_Height));
}

void RayTracer::Start(GLFWwindow* _window)
{
	window = _window;
	pd = new PixelDisplay();
	objList.objs.push_back(new Sphere(1, { 0,0,-5 }, new  GlassMat()));
	objList.objs.push_back(new Sphere(1, { -1,0,-10 }, new  Lambertian(Color(1, 0, 1))));
	objList.objs.push_back(new Sphere(1, { 1,0,-10 }, new  Lambertian(Color(1, 94.0f / 255, 19.0f / 255))));
	objList.objs.push_back(new Sphere(99, { 0,-100,-6 }, new Lambertian(Color::Green())));

	DispatchThreads(5);
}

Color GetRayColor(const Ray& ray, int depth = 0)
{
	RayHitInfo info;
	if (objList.CheckHit(ray, info, 0.001, INFINITY))
	{
		Ray scattered;
		Color attenuation;
		if (depth <= maxDepth && info.mat->Scatter(ray, info, attenuation.rgb, scattered))
		{
			vec3 target = info.point + info.normal + HelperFuncs::RandOutUnitSphere();
			return GetRayColor(scattered, depth + 1) * attenuation;
		}
		else
			return Color::Black();
	}
	else
	{
		return Ray::SkyColor(ray);
	}
}

void Update(int startLine, int endLine)
{
	using namespace std;
	while (!glfwWindowShouldClose(window))
	{
		srand(1);
		for (int i = startLine; i < endLine; i++)
		{
			for (int j = 0; j < Window_Width; j++)
			{
				//auto color = GetRayColor(cam.GetRay(i, j));
				// MSAA
				auto color = cam.MultiSample(i, j, GetRayColor);
				color.GammaCorrect();
				pd->screen[i][j] = color;
			}
		}
	}
}
void RayTracer::Render()
{
	ImGui::SliderFloat("distToScreen: ", &cam.invDistToScreen, 0, 10);
	cam.UpdateDistToScreen();
	ImGui::SliderFloat3("spherePos:", &((Sphere*)objList.objs[0])->center.x, -20, 10);
	static char maxDepthStr[5] = "4";
	ImGui::InputText("maxDepth", maxDepthStr, 5);
	maxDepth = atoi(maxDepthStr);
	pd->Render();
}

void RayTracer::Terminate()
{
	for (int i = 0; i < threadPool.size(); i++)
	{
		threadPool[i].join();
	}
}
