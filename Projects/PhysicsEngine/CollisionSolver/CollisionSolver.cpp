#include "CollisionSolver.h"
#include "../Collliders/Collider.h"
#include <iostream>
#define PLASTIC 0
void CollisionSolver::Solve(CircleCollider& collider1, CircleCollider& collider2)
{
	RigidBody& rb1 = collider1.rigidBody;
	RigidBody& rb2 = collider2.rigidBody;

	if (PLASTIC)
	{
		rb1.SetVelocity((rb1.velocity * rb1.mass + rb2.velocity * rb2.mass) / (rb1.mass + rb2.mass));
		rb2.SetVelocity((rb1.velocity * rb1.mass + rb2.velocity * rb2.mass) / (rb1.mass + rb2.mass));
	}
	else
	{
		rb1.SetVelocity(2.0f * (rb1.velocity * rb1.mass + rb2.velocity * rb2.mass) / (rb1.mass + rb2.mass) - rb1.velocity);
		rb2.SetVelocity(2.0f * (rb1.velocity * rb1.mass + rb2.velocity * rb2.mass) / (rb1.mass + rb2.mass) - rb2.velocity);
	}
}