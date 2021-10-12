#include "RayTracer.h"
#include <Common.h>
//#include <Logger.h>
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
int spp = 4;
Camera cam(1, spp);
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
	objList.objs.push_back(new Sphere(1, { -0.3,1,-6 }, new GlassMat()));
	objList.objs.push_back(new Sphere(1, { -1,0,-10 }, new  Lambertian(Color(1, 0, 1))));
	objList.objs.push_back(new Sphere(1, { 1,0,-10 }, new  Lambertian(Color(1, 94.0f / 255, 19.0f / 255))));
	objList.objs.push_back(new Sphere(99, { 0,-100,-6 }, new Lambertian(Color::Green())));
	objList.objs.push_back(new Sphere(0.5, { 0,2.5,-12 }, new Lambertian(Color::Random())));
	objList.objs.push_back(new Sphere(0.2, { 0.5,1.6,-11 }, new MetalMat(Color::White(), 0)));
	objList.objs.push_back(new Sphere(0.6, { -2.5,2.5,-13 }, new Lambertian(Color(1, 1, 0))));
	objList.objs.push_back(new Sphere(0.8, { -2.3,2,-15 }, new Lambertian(Color::Red())));
	objList.objs.push_back(new Sphere(0.3, { 2,1.8,-12 }, new MetalMat(Color::White(), 0)));
	objList.objs.push_back(new Sphere(0.1, { 1.5,1.5,-10 }, new Lambertian(Color::Blue())));

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
static float theta = 1e-3;
static float idxRefrac = 1;
static bool isMirror = false;
static Color mirrorAlbedo = Color::White();
bool spin = 0;
void RayTracer::Render()
{

	ImGui::SliderFloat("distToScreen: ", &cam.invDistToScreen, 0, 10);
	ImGui::SliderInt("Samples per pixel: ", &spp, 1, 400);
	cam.samplesPP = spp;
	cam.UpdateDistToScreen();
	ImGui::SliderFloat3("spherePos:", &((Sphere*)objList.objs[0])->center.x, -20, 10);
	static char maxDepthStr[5] = "4";
	ImGui::InputText("maxDepth:", maxDepthStr, 5);
	maxDepth = atoi(maxDepthStr);
	ImGui::Checkbox("Is Mirror", &isMirror);
	GlassMat* mat = dynamic_cast<GlassMat*>(objList.objs[0]->mat);
	if (isMirror == true)
	{
		if (mat != null)
		{
			auto oldptr = objList.objs[0]->mat;
			objList.objs[0]->mat = new MetalMat(mirrorAlbedo, 0);
			delete oldptr;
		}
		ImGui::SliderFloat("blurare", &((MetalMat*)objList.objs[0]->mat)->fuzz, 0, 1);
	}
	else
	{
		if (mat == null)
		{
			auto oldptr = objList.objs[0]->mat;
			objList.objs[0]->mat = new GlassMat();
			delete oldptr;
		}
		ImGui::SliderFloat("Indice de refractie", &idxRefrac, 1, 4);
		((GlassMat*)objList.objs[0]->mat)->refracIdx = idxRefrac;
	}
	pd->Render();
	ImGui::Checkbox("Spin", &spin);
	ImGui::SliderFloat("SpinSpeed", &theta, 1e-4, 1e-2);

	///Spin
	if (spin)
	{
		mat4 model = mat4(1);
		model = translate(model, ((Sphere*)objList.objs[0])->center);
		model = rotate(model, theta, vec3(0, 1, 0));
		model = translate(model, -((Sphere*)objList.objs[0])->center);
		for (int i = 0; i < objList.objs.size(); i++)
		{
			vec4 coords = vec4(((Sphere*)objList.objs[i])->center, 1);
			((Sphere*)objList.objs[i])->center = vec3(model * coords);
		}
	}
}

void RayTracer::Terminate()
{
	for (int i = 0; i < threadPool.size(); i++)
	{
		threadPool[i].join();
	}
}
