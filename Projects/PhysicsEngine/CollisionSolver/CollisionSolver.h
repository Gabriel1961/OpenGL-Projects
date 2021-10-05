#pragma once
class CircleCollider;
class CollisionSolver
{
public:
	static void Solve(CircleCollider& collider1, CircleCollider& collider2);
};

