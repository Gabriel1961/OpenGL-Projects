#include "Collider.h"
using namespace glm;

CollisionInfo CollisionTests::TestColl(CircleCollider* c1, CircleCollider* c2)
{
	CollisionInfo info;
	Transform2D& t1 = c1->transform; Transform2D& t2 = c2->transform;
	if (glm::length(t1.position - t2.position) < c1->radius + c2->radius)
	{
		info.IsCollision = true;
		float k = c1->radius / c2->radius;
		info.CollisionPoint = 1.0f / (k + 1) * t1.position + k / (k + 1) * t2.position;
	}
	else
		info.IsCollision = false;
	return info;
}




CollisionInfo CollisionTests::TestColl(CircleCollider* c1, BoxCollider* c2)
{
	CollisionInfo info;
	mat2 scale{ c2->transform.scale.x / 2,0,0,c2->transform.scale.y / 2 };
	mat2 rot = c2->transform.RotationMat2();
	vec2 circlePos = (c1->transform.position - c2->transform.position) * rot;
	if (Det(circlePos + vec2{ c1->radius,0 } + c2->boxSides[3] * scale, c2->boxSides[0]) <= 0)
		if (Det(circlePos + vec2{ 0,-c1->radius } + c2->boxSides[0] * scale, c2->boxSides[1]) <= 0)
			if (Det(circlePos + vec2{ -c1->radius,0 } + c2->boxSides[1] * scale, c2->boxSides[2]) <= 0)
				if (Det(circlePos + vec2{ 0,c1->radius } + c2->boxSides[2] * scale, c2->boxSides[3]) <= 0)
				{
					printf("Hit");
				}

	return info;
}

CollisionInfo CollisionTests::TestColl(BoxCollider* c1, BoxCollider* c2)
{
	return CollisionInfo();
}

CollisionInfo CircleCollider::TestCollision(Transform2D& otherTransform, Collider* otherCollider)
{
	CollisionInfo info;
	if (auto coll = dynamic_cast<CircleCollider*>(otherCollider))
	{
		info = CollisionTests::TestColl(this, coll);
		if (info.IsCollision)
			CollisionSolver::Solve(*this, *coll);
	}
	else if (auto coll = dynamic_cast<BoxCollider*>(otherCollider))
	{
		info = CollisionTests::TestColl(this, coll);
	}
	return info;
}

// initial value of vb

CollisionInfo BoxCollider::TestCollision(Transform2D& otherTransform, Collider* otherCollider)
{
	CollisionInfo info;
	if (auto coll = dynamic_cast<CircleCollider*>(otherCollider))
	{
		info = CollisionTests::TestColl(coll, this);
	}
	else if (auto coll = dynamic_cast<BoxCollider*>(otherCollider))
	{
		info = CollisionTests::TestColl(coll, this);
	}
	return info;
}
