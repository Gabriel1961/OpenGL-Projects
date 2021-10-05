#pragma once
#include "../PhysicsEngine.h"
#include "DebugLine.h"

#include "../CollisionSolver/CollisionSolver.h"
struct CollisionInfo
{
	bool IsCollision = false;
	glm::vec2 CollisionPoint;
};

class Collider
{
public:
	Transform2D& transform;

	EventSystem::Action<CollisionInfo> OnCollisionEnter;
	EventSystem::Action<CollisionInfo> OnCollisionStay;
	EventSystem::Action<CollisionInfo> OnCollisionExit;

	Collider(Transform2D& transform)
		: transform(transform)
	{
	}

	virtual CollisionInfo TestCollision(Transform2D& otherTransform, Collider* otherCollider) = 0;
};
class BoxCollider;
namespace CollisionTests
{
	CollisionInfo TestColl(CircleCollider* c1, CircleCollider* c2);
	CollisionInfo TestColl(CircleCollider* c1, BoxCollider* c2);
	CollisionInfo TestColl(BoxCollider* c1, BoxCollider* c2);
};

class CircleCollider : public Collider
{
public:
	const float& radius;
	RigidBody& rigidBody;
	bool hasEnteredCollision = false;

	CircleCollider(Transform2D& transform, RigidBody& rigidBody, const float& radius)
		:Collider(transform), radius(radius), rigidBody(rigidBody)
	{

	}

	CollisionInfo TestCollision(Transform2D& otherTransform, Collider* otherCollider) override;;
};



class BoxCollider : public Collider
{
public:
	/// array size of 4
	glm::vec2 boxSides[4]; // the vectors of the sides of the box
	BoxCollider(Transform2D& transform, float* vbData)
		:Collider(transform)
	{
		auto vertData = reinterpret_cast<glm::vec2*>(vbData);
		boxSides[0] = vertData[0] - vertData[1];
		boxSides[1] = vertData[1] - vertData[2];
		boxSides[2] = vertData[2] - vertData[3];
		boxSides[3] = vertData[3] - vertData[0];
	}
	CollisionInfo TestCollision(Transform2D& otherTransform, Collider* otherCollider) override;
};