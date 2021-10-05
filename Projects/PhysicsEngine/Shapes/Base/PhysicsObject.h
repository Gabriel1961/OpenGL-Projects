#pragma once
#include "../../PhysicsEngine.h"
#include "../../Collliders/Transform.h"
namespace Object
{

	class PhysicsObject
	{
	protected:
		glm::mat3 ScaleMat = glm::mat3(
			1, 0, 0,
			0, 1, 0,
			0, 0, 1);
		glm::mat3 AspectMat;

	public:

		Transform2D transform;
		Collider* collider;
		RigidBody* rigibody;

		void SetScaleX(float val)
		{
			transform.scale.x = val;
			ScaleMat[0][0] = transform.scale.x;
		}

		void SetScale(const glm::vec2& scale)
		{
			transform.scale = scale;
			ScaleMat[0][0] = transform.scale.x;
			ScaleMat[1][1] = transform.scale.y;
		}

		void SetScaleY(float val)
		{
			transform.scale.y = val;
			ScaleMat[1][1] = transform.scale.y;
		}

		virtual void FixedUpdate() = 0;
		virtual void Render() = 0;

	};
}