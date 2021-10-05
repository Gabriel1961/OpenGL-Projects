#pragma once
#include "../PhysicsEngine.h"
struct RigidBody
{
	// Public Fields
	float mass = 1;
	bool bounceOnEdge = true;

	// Translation
	glm::vec2 velocity = { 0,0 };
	glm::vec2 acceleration = { 0,0 };
	glm::vec2 force = { 0,0 };

	// Angular Motion
	float angularVelocity = 0;
	float angularAcceleration = 0;
	float angularInertia;
	float torque = 0;

	glm::vec2 GetMomentum()
	{
		return velocity * mass;
	}

	Transform2D& transform;

	// Constructor
	RigidBody(Transform2D& transform, float mass, float angularInertia)
		:mass(mass), transform(transform), angularInertia(angularInertia)
	{
	}

	/// Adds a force at a specific point 
	/// <param name="point :">in world coordinates</param>
	void AddForceAtPoint(glm::vec2 point, glm::vec2 _force)
	{
		glm::vec2 posRelToCenter = point - transform.position;
		float netTorque = -posRelToCenter.x * _force.x + posRelToCenter.y * _force.y;
		torque += netTorque;
	}

	/// Adds a force at a specific point 
	/// <param name="point :">in local coordinates</param>
	void AddRelativeForceAtPoint(glm::vec2 point, glm::vec2 _force)
	{
		point = point * transform.RotationMat2();
		float netTorque = Det(point - transform.position, _force);
		force += (_force * transform.RotationMat2()).x;
		torque += netTorque;
	}

	// Helper
	void AddForce(glm::vec2 val)
	{
		force += val;
	}


	void SetVelocity(glm::vec2 val)
	{
		force = val / PhysicsEngine::DeltaTime() * mass;
	}

	///Bounces on edge if bool bounceOnEdge is set to true
	void BounceOnEdge()
	{
		if (bounceOnEdge)
		{
			if (transform.position.x < -1 / PhysicsEngine::GetAspectRatio() + transform.scale.x)
			{
				SetVelocity({ -velocity.x ,velocity.y });
				transform.position.x = -1 / PhysicsEngine::GetAspectRatio() + transform.scale.x;
			}
			else if (transform.position.x > 1 / PhysicsEngine::GetAspectRatio() - transform.scale.x)
			{
				SetVelocity({ -velocity.x,velocity.y });
				transform.position.x = 1 / PhysicsEngine::GetAspectRatio() - transform.scale.x;
			}
			else if (transform.position.y < -1 + transform.scale.y)
			{
				SetVelocity({ velocity.x ,-velocity.y });
				transform.position.y = -1 + transform.scale.y;
			}
			else if (transform.position.y > 1 - transform.scale.y)
			{
				SetVelocity({ velocity.x,-velocity.y });
				transform.position.y = 1 - transform.scale.y;
			}
		}
	}

	// To update position
	void Update()
	{
		//Translation Calculations1
		acceleration = force / mass;
		velocity = acceleration * PhysicsEngine::DeltaTime();
		transform.position += velocity * PhysicsEngine::DeltaTime();

		//Rotational Calculations
		torque -= torque * 0.1;
		angularAcceleration = torque / angularInertia;
		angularVelocity = angularAcceleration * PhysicsEngine::DeltaTime();
		transform.SetAngle(transform.GetAngle() + angularVelocity);

		BounceOnEdge();
	}
};