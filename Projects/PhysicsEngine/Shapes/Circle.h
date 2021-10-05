#pragma once
#include "Base/Shape.h"
#include "./Base/PhysicsObject.h"
#include "../Colors.h"
#include "../Collliders/Collider.h"
namespace Object
{
	/// Contains a circle  
	class Circle : public PhysicsObject, public RenderableShape
	{

	private:
		std::vector<float> GetVbData(uint res)
		{
			float angle = 2 * pi / res;
			std::vector<float> vec;
			vec.push_back(0);
			vec.push_back(0);
			for (int i = 0; i < res; i++)
			{
				vec.push_back(std::cos(angle * i));
				vec.push_back(std::sin(angle * i));
			}
			return vec;
		}
	public:
		//Fields
		Color color;

		// Events 
		EventSystem::Action<CollisionInfo>* OnCollisionEnter;



		Circle(glm::vec2 scale, glm::vec2 pos, Color color, int divisions, float mass = 1)
			: color(color)
		{
#pragma region Init
			transform.position = pos;
			SetScaleX(scale.x);
			SetScaleY(scale.y);
			AspectMat = glm::mat2(PhysicsEngine::GetAspectRatio(), 0, 0, 1);
			AspectMat[2][2] = 1;
#pragma endregion

#pragma region Rendering

			std::vector <uint> ibData;
			for (int i = 1; i <= divisions; i++)
			{
				ibData.push_back(0);
				ibData.push_back(i);
				ibData.push_back(i + 1);
			}
			ibData.push_back(0);
			ibData.push_back(divisions);
			ibData.push_back(1);
			ib = new IndexBuffer(ibData.size(), &ibData[0], GL_UNSIGNED_INT);
			std::vector<float> vbData = GetVbData(divisions);
			vb = new VertexBuffer(vbData.size() * sizeof(int), &vbData[0]);
			shader = new Shader(SHADER_DIR + std::string("Circle.shader"));
			va = new VertexArray();
			VertexBufferLayout layout;
			layout.Push<float>(2);
			va->AddLayout(*vb, layout);

#pragma endregion

			//Rigibody
			rigibody = new RigidBody(transform, mass, 1 / 2 * mass * std::pow(GetRadius(), 2));

			// Collider
			collider = new CircleCollider(transform, *rigibody, GetRadius());
			this->OnCollisionEnter = &(collider->OnCollisionEnter);

		}

#pragma endregion

		// Getter for radius
		const float& GetRadius()
		{
			return transform.scale.x;
		}

		// RenderCall 
		void Render() override
		{
			glm::mat3 PosMat(
				1, 0, transform.position.x,
				0, 1, transform.position.y,
				0, 0, 1);
			auto MVP = ScaleMat * PosMat * AspectMat;
			shader->SetUniformMat3f("MVP", MVP);
			shader->SetUniform4f("uColor", color.rgba);
			Renderer::Draw(*va, *ib, *shader, GL_TRIANGLES);
		}

		// Physics
		void FixedUpdate()
		{
			rigibody->Update();
		}

	};
}