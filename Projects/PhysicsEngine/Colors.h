#pragma once
#include "../../GLEW/include/GL/glm/vec4.hpp"
#include "../../GLEW/include/GL/glm/vec3.hpp"
class Color
{
public:
	glm::vec4 rgba;

	Color()
	{
		rgba = glm::vec4(1, 1, 1, 1);
	}

	Color(glm::vec4 rgba)
	{
		this->rgba = rgba;
	}

	Color(glm::vec3 rgb)
	{
		rgba = glm::vec4(rgb, 1);
	}

	static Color Red()
	{
		return Color(glm::vec3(1, 0, 0));
	}

	static Color Green()
	{
		return Color(glm::vec3(0, 1, 0));
	}

	static Color Blue()
	{
		return Color(glm::vec3(0, 0, 1));
	}
};

