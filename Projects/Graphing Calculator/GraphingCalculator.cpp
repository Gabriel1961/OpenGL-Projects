#include "GraphingCalculator.h"
#include "./GraphGrid.h"
#include "../src/Input/Input.h"
#include <string>
using namespace glm;
using namespace std;
constexpr auto FORMULA_SIZE = 20;
char* formula = new char[FORMULA_SIZE] {0};

mat3 Calculator::_aspectMat;

void Calculator::Render()
{
	ImGui::Text("Enter a valid mathematical formula:");
	ImGui::InputText("", formula, FORMULA_SIZE);
	ImGui::Text("Formula is invalid");
	GraphGrid::Draw();
}

void Calculator::Start(GLFWwindow* window)
{
	Input::Init(window);
	_aspectMat = {
		AspectR, 0, 0,
		0, 1, 0,
		0, 0 , 1
	};
	GraphGrid::Start(100, 100);
	Input::Mouse::MouseScroll += OnZoom;
	Input::Mouse::MouseMove += OnMouseMove;
}

glm::mat3& Calculator::GetAspectMat()
{
	return _aspectMat;
}
void OnMouseMove(double x, double y)
{
	if (Input::Mouse::LeftPressed)
	{
		vec3 position = { x / AspectR,y,1 };
		position = position / GraphGrid::transform.GetScaleMat();
		GraphGrid::transform.SetTranslation(
			GraphGrid::transform.GetTranslateMat() + mat3{
			  0,0,position.x,
			  0,0,position.y,
			  0,0,0 });
	}
}
float zoom = 1;
void OnZoom(double OffsetX, double OffsetY)
{
	const float zoomSensitivity = 0.1f;
	zoom += zoomSensitivity * OffsetY;
	mat3 zoomMat = {
		zoom ,0,0,
		0,zoom ,0,
		0,0,1, };
	GraphGrid::transform.SetScale(zoomMat);
}