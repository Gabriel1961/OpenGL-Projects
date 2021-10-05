#pragma once
#include <vector>
#include "Transform.h"
#include "GL/glm/vec2.hpp"
#include "Text/Text.h"
static std::vector<Text*> GetNumbersOnGraph(const glm::ivec2& size);
class GraphGrid
{
private:
	static float spacing;
	static float sizeX;
	static float sizeY;
public:
	static Transform transform;
	static void Start(int sizeX, int sizeY, float spacing = 0.1);
	static void Draw();
	static  const float& GetSpacing() { return spacing; };
};