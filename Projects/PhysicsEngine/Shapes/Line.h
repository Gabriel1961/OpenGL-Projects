#pragma once
#include "Base/Shape.h"
#include "./Base/PhysicsObject.h"
#include "../Colors.h"
#include "../Collliders/Collider.h"
namespace Object
{
	class Line : public PhysicsObject, public RenderableShape
	{

	public:
		// Constructors
		Color color;

		/// Creat a segment using a start end an end vector
		Line(glm::vec2 start, glm::vec2 end)
		{

		}

		/// Create a line using a formula like ax + c
		Line(int a, int c, glm::vec2 position)
		{

		}

		void Render() override
		{

		}

		void FixedUpdate() override
		{

		}
	};
}