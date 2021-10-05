#pragma once
#include "../PhysicsEngine.h"
#include "../Colors.h"
#include "Base/PhysicsObject.h"
#include "Base/Shape.h"
#include <string>
namespace Object
{

	class Square : public PhysicsObject, public RenderableShape
	{
	protected:

		float vbData[8]
		{
			-1,-1,
			-1,1,
			1,1,
			1,-1,
		};

		uint idxData[6]
		{
			0,1,2,
			0,2,3
		};


	public:
		Color color;

		void SetupRender()
		{
			AspectMat = glm::mat2(PhysicsEngine::GetAspectRatio(), 0, 0, 1);
			AspectMat[2][2] = 1;
			vb = new VertexBuffer(sizeof(vbData), vbData);
			ib = new IndexBuffer(sizeof(idxData) / sizeof(*idxData), idxData);
			shader = new Shader(SHADER_DIR + std::string("Square.shader"));
			va = new VertexArray();
			VertexBufferLayout layout;
			layout.Push<float>(2);
			va->AddLayout(*vb, layout);
		}

		Square(glm::vec2 scale, glm::vec2 pos, Color color, float mass = 1)
			: color(color)
		{
			using std::pow;
			transform.position = pos;
			SetScale(scale);
			SetupRender();

			collider = new BoxCollider(transform, vbData);
			rigibody = new RigidBody(transform, mass,
				1 / 12.0f * mass * (pow(transform.scale.x, 2) + pow(transform.scale.y, 2)));

		}


		void Render() override
		{
			glm::mat3 PosMat(
				1, 0, transform.position.x,
				0, 1, transform.position.y,
				0, 0, 1);
			glm::mat3 MVP = ScaleMat * transform.RotationMat3() * PosMat * AspectMat;
			shader->SetUniformMat3f("MVP", MVP);
			shader->SetUniform4f("COLOR", color.rgba);
			Renderer::Draw(*va, *ib, *shader, GL_TRIANGLES);
		}

		void FixedUpdate() override
		{
			rigibody->Update();
		}
	};

}