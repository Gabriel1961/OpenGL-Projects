#pragma once
#include <tuple>
#include <Common.h>
#include <../ScreenSize.h>
#include <Logger.h>
#include <random>
#include <vector>
using namespace glm;
namespace HelperFuncs
{
	/// Returns a float f, 0 <= f < 1
	float RandomF() // TODO Modify to a threadsafe rand_r();
	{
		return rand() / (float)(RAND_MAX + 1);
	}
	/// Generates a random vec outside a sphere of radius = 1
	vec3 RandOutUnitSphere()
	{
		vec3 p;
		do
		{
			p = 2.0f * vec3(RandomF(), RandomF(), RandomF()) - vec3(1);
		} while (dot(p, p) >= 1.0f);
		return p;
	}

	/// <summary>
	/// Refracts ray
	/// </summary>
	/// <param name="incident">- must be normalized</param>
	/// <param name="normal">- must be normalized</param>
	/// <param name="ni_divby_nr">- n1/n2</param>
	/// <param name="refracted"></param>
	/// <returns>if refraction was possible</returns>
	inline bool Refract(const vec3& incident, const vec3& normal, const float ni_divby_nr, vec3& refracted)
	{
		float dt = dot(incident, normal);
		float cy2 = 1.0f - ni_divby_nr * ni_divby_nr * (1.0f - dt * dt);
		if (cy2 > 0) {
			refracted = ni_divby_nr * (incident - normal * dt) - normal * sqrt(cy2);
			return true;
		}
		else
			return false;
	}
};
struct Ray
{
	// Constructors
	Ray() {}
	Ray(vec3 origin, vec3 dir, bool normalized)
		:a(origin), b(normalized ? dir : normalize(dir))
	{}

	// Methods
	const vec3& Direction()const { return b; }
	const vec3& Origin()const { return a; }
	vec3 PointTo(float t) const { return a + b * t; }

	// Static Funcs
	static Color SkyColor(const Ray& ray)
	{
		float y = 0.5f * (1 + ray.Direction().y);
		return Color::White() * (1 - y) + Color(0.5, 0.7, 1) * y;
	}

	// Public Fields
	vec3 a = vec3(0);
	vec3 b = vec3(0);
};
class Object;
class Material;
struct RayHitInfo
{
	RayHitInfo()
	{
	}
	RayHitInfo(const vec3& point, const vec3& normal) :point(point), normal(normal)
	{
	}
	float t;
	vec3 normal = vec3(0);
	vec3 point = vec3(0);
	Material* mat = nullptr;
};

class Object
{
public:
	virtual bool CheckRayHit(const Ray& ray, RayHitInfo& out, float tmin = 0, float tmax = INFINITY)const = 0;

	Material* mat;

	~Object()
	{
		delete mat;
	}
};

class Sphere : public Object
{
public:
	Sphere(float radius, const vec3& center, Material* material)
		:radius(radius), center(center)
	{
		mat = material;
	}

	bool CheckRayHit(const Ray& ray, RayHitInfo& out, float tmin = 0, float tmax = INFINITY)const override
	{
		vec3 oc = ray.Origin() - center;
		float a = dot(ray.Direction(), ray.Direction());
		float b = dot(oc, ray.Direction());
		float c = dot(oc, oc) - radius * radius;
		float delta = b * b - a * c;
		if (delta >= 0)
		{
			float t = (-b - sqrt(delta)) / a; // The 2 in 2a cancels itself out
			if (tmin < t && t < tmax)
			{
				out.t = t;
				out.point = ray.PointTo(t);
				out.normal = (out.point - center) / radius;
				out.mat = mat;
				return true;
			}
			t = (-b + sqrt(delta)) / a;
			if (tmin < t && t < tmax)
			{
				out.t = t;
				out.point = ray.PointTo(t);
				out.normal = (out.point - center) / radius;
				out.mat = mat;
				return true;
			}
		}
		return false;
	}

	//Public fields
	float radius;
	vec3 center;
};

class ObjectList
{
public:
	bool CheckHit(const Ray& ray, RayHitInfo& out, float tmin = 0, float tmax = INFINITY)
	{
		RayHitInfo info;
		bool hasHitAnything = false;
		double closestT = tmax;
		for (auto& obj : objs)
		{
			if (obj->CheckRayHit(ray, info, tmin, tmax))
				if (info.t < closestT)
				{
					closestT = info.t;
					out = info;
					hasHitAnything = true;
				}
		}
		return hasHitAnything;
	}
	std::vector<Object*> objs;
};


class Camera
{
public:

	Camera() {}
	Camera(bool MSAA, int samplesPP = 4) :MSAAEnabled(MSAA), samplesPP(samplesPP) {}
	//Methods
	void UpdateDistToScreen() { leftDown = vec3(-1, -AspectR, -1 / invDistToScreen); }
	/// <summary>
	/// Generates a ray for the specified pixel
	/// </summary>
	/// <param name="i">vertical index of pixel</param>
	/// <param name="j">horizontal index of pixel</param>
	/// <returns></returns>
	Ray GetRay(int i, int j)
	{
#if _DEBUG
		if (MSAAEnabled)
		{
			print("[Error]: You can't be using the function GetRay() if bool MSSAEnabled is true. Consider using MultiSample()");
			assert(false);
		}
#endif
		auto cords = vec3{ (float)j / (Window_Width - 1) ,(float)i / (Window_Height - 1) * AspectR,0 };
		return Ray(vec3(0), cords * 2.0f + leftDown, false);
	}

	Color MultiSample(int i, int j, Color(*GetRayColor)(const Ray&, int))
	{
#if _DEBUG
		if (MSAAEnabled == false)
		{
			print("[Error]: You can't be using the function MultiSample() if bool MSSAEnabled is false.Consider using GetRay()");
			throw std::exception();
		}
#endif
		using namespace HelperFuncs;
		vec3 color = vec3(0);
		for (int k = 0; k < samplesPP; k++)
		{
			auto cords = vec3{ (j + RandomF()) / (Window_Width - 1) ,(i + RandomF()) / (Window_Height - 1) * AspectR,0 };
			Color c = GetRayColor(Ray(vec3(0), cords * 2.0f + leftDown, false), 0);
			color += c.rgb;
		}
		color /= samplesPP;
		return color;
	}
	//public Fields
	bool MSAAEnabled = false;
	///Number of samples per pixel;
	int samplesPP = 4;
	vec3 leftDown = vec3(-1, -AspectR, -1 / invDistToScreen);
	float invDistToScreen = 0.6;
};

/// <summary>
/// Base class for materials
/// </summary>
class Material
{
public:
	virtual bool Scatter(const Ray& ray, const RayHitInfo& info, vec3& attenuation, Ray& scattered) const = 0;
};

/// <summary>
/// Diffuse Materia
/// </summary>
class Lambertian : public Material
{
public:
	Lambertian(const Color& albedo) : albedo(albedo) {}
	bool Scatter(const Ray& ray, const RayHitInfo& info, vec3& attenuation, Ray& scattered)const override
	{
		vec3 target = info.point + info.normal + HelperFuncs::RandOutUnitSphere();
		scattered = Ray(info.point, target - info.point, true);
		attenuation = albedo.rgb;
		return true;
	}
	Color albedo;
};

class MetalMat : public Material
{
public:
	MetalMat(const Color& albedo, float fuzz) :albedo(albedo), fuzz(fuzz) {}
	bool Scatter(const Ray& ray, const RayHitInfo& info, vec3& attenuation, Ray& scattered)const override
	{
		if (dot(info.normal, -ray.Direction()) <= 0)
			return false;
		vec3 reflected = reflect(ray.Direction(), info.normal);
		scattered = Ray(info.point, reflected + HelperFuncs::RandOutUnitSphere() * fuzz, false);
		attenuation = albedo.rgb;
		return true;
	}

	Color albedo;
	float fuzz;
};

class GlassMat : public Material
{
public:
	float refracIdxAir = 1.000293;
	float refracIdx = 1.6;

	GlassMat() {}
	GlassMat(double refracIdx) :refracIdx(refracIdx) {}

	bool Scatter(const Ray& ray, const RayHitInfo& info, vec3& attenuation, Ray& scattered)const override
	{
		vec3 outward_normal = info.normal;
		float ni_over_nt;
		attenuation = Color::White().rgb;
		vec3 refracted;
		if (dot(ray.Direction(), info.normal) > 0)
		{
			outward_normal = -info.normal;
			ni_over_nt = refracIdx / refracIdxAir;
		}
		else
		{
			outward_normal = info.normal;
			ni_over_nt = refracIdxAir / refracIdx;
		}
		if (HelperFuncs::Refract(ray.Direction(), outward_normal, ni_over_nt, refracted)) {
			scattered = Ray(info.point, refracted, false);
		}
		else
		{
			vec3 reflected = reflect(ray.Direction(), info.normal);
			scattered = Ray(info.point, reflected, true);
			return false;
		}
		return true;
	}
};
