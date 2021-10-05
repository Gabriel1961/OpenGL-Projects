#pragma once
#include <complex>
#include <cmath>
#include "./GL/glm/mat3x3.hpp"
#include "../../OpenGLWrapper/vendor/imgui/ImguiLib.h"
using namespace std::complex_literals;
static void OnZoom(double offsetX, double offsetY);
static void OnMouseMove(double x, double y);
class Calculator
{
	static glm::mat3 _aspectMat;
public:
	static void Render();
	static glm::mat3& GetAspectMat();
	static void Start(GLFWwindow* window);
};