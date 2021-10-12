#pragma once
#include <Common.h>
using namespace std;
using namespace glm;
enum class MaterialTypes
{
	diffuse = 0,     // Diffuse
	reflective = 1,  // Lambertian, Metal
	translucent = 2, // Transparent
};

struct Material
{
	vec4 albedo;
	float refracIdx, refracIdxAir, blur;
	int type;

	Material() {}
	/// Translucent Constructor
	Material(float refracIdx, float refracIdxAir, vec4 albedo = vec4(1))
		:albedo(albedo), refracIdx(refracIdx), refracIdxAir(refracIdxAir), type((int)MaterialTypes::translucent) {}
	/// Diffuse Constructor
	Material(vec4 albedo) : albedo(albedo), type((int)MaterialTypes::diffuse) {}
	/// Reflective construtor
	Material(float blur, vec4 albedo) :blur(blur), albedo(albedo), type((int)MaterialTypes::reflective) {}
};

struct Sphere
{
	vec4 center;
	float radius;
	int material;

	Sphere(float radius, vec4 center, int material) :center(center), radius(radius), material(material) {  }
};
