#pragma once
#include "Common.h"
#include "../PhysicsEngine.h"
struct Transform2D
{
	glm::vec2 position;
	glm::vec2 scale = { 1,1 };
private:
	float angle = 0;
	glm::mat3 rotation = glm::mat3(1);
public:
	inline const glm::mat3& RotationMat3() { return rotation; }
	inline glm::mat2 RotationMat2() { return glm::mat2(rotation); }
	inline void SetAngle(float value) {
		if (angle == value)
			return;
		angle = value;
		rotation = {
			-sin(angle),cos(angle),0,
			cos(angle),sin(angle),0,
			0,0,1,
		};
	}
	inline const float& GetAngle() { return angle; }
};