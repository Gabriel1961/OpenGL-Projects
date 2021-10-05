#pragma once
#include "../../../OpenGLWrapper/Shader.h"
#include "../../../OpenGLWrapper/VertexArray.h"
#include "../../../OpenGLWrapper/VertexBuffer.h"
#include "../../../OpenGLWrapper/IndexBuffer.h"
#include "../../../OpenGLWrapper/IndexBuffer.h"
#include "../../../OpenGLWrapper/VertexBufferLayout.h"
#include "../../../OpenGLWrapper/Texture.h"
#include "../../../OpenGLWrapper/Renderer.h"
#include "../Minecraft.h"
enum class FaceOrientation
{
	Up,
	Down,
	Left,
	Right,
	Front,
	Back
};

class BlockFace
{
private:
	VertexArray*  _vertexArray		= nullptr;
	VertexBuffer* _vertexBuffer		= nullptr;
	IndexBuffer*  _indexBuffer      = nullptr;
	Texture*	  _texture	        = nullptr;
	Shader*		  _shader			= nullptr;

	CameraRotation& cameraRotation;
	glm::vec3& camPos;
	glm::mat4& projMat;
	glm::vec3& light;
public:

	glm::vec3 blockPosition;
	FaceOrientation orientation;
	
	BlockFace(FaceOrientation orientation, Texture& texture, Shader& shader, glm::vec3 blockPosition, glm::vec3& camPos, CameraRotation& camRotation, glm::mat4& projMat, glm::vec3& light)
		:orientation(orientation),_texture(&texture), _shader(&shader), blockPosition(blockPosition),camPos(camPos),cameraRotation(camRotation), projMat(projMat),light(light)
	{
		VertexBufferLayout layout;
		_vertexArray = new VertexArray();
		switch (orientation)
		{
		case FaceOrientation::Up:
			{
				float data[]
				{
					-1, 1,-1,  0,0,// 4
					 1, 1,-1,  1,0,// 5
					 1, 1, 1,  1,1,// 6
					-1, 1, 1,  0,1,// 7 
				};
				uint ibData[]
				{
					0,1,2,
					2,3,0
				};
				_vertexBuffer = new VertexBuffer(sizeof(data), data);
				_indexBuffer = new IndexBuffer(sizeof(ibData) / sizeof(*ibData), ibData, GL_UNSIGNED_INT);
				layout.Push<float>(3);
				layout.Push<float>(2);
				_vertexArray->AddLayout(*_vertexBuffer, layout);
			}	
			break;
		case FaceOrientation::Down:
		{
			float data[]
			{
				-1,-1,-1,	0,0, // 0
				 1,-1,-1,	1,0, // 1
				 1,-1, 1,	1,1, // 2
				-1,-1, 1,	0,1, // 3 
			};
			uint ibData[]
			{
				0,1,2,
				2,3,0
			};
			_vertexBuffer = new VertexBuffer(sizeof(data), data);
			_indexBuffer = new IndexBuffer(sizeof(ibData) / sizeof(*ibData), ibData, GL_UNSIGNED_INT);
			layout.Push<float>(3);
			layout.Push<float>(2);
			_vertexArray->AddLayout(*_vertexBuffer, layout);
		}
			break;
		case FaceOrientation::Left:
		{

		}
			break;
		case FaceOrientation::Right:
		{

		}
			break;
		case FaceOrientation::Front:
		{
			float data[]
			{
				-1,-1,-1,	0,0, // 0
				 1,-1,-1,	1,0, // 1
				-1, 1,-1,	0,1, // 4 
				 1, 1,-1,	1,1, // 5 

			};
			uint ibData[]
			{
				0,1,3,
				0,3,2
			};
			_vertexBuffer = new VertexBuffer(sizeof(data), data);
			_indexBuffer = new IndexBuffer(sizeof(data) / sizeof(*data), ibData, GL_UNSIGNED_INT);
			layout.Push<float>(3);
			layout.Push<float>(2);
			_vertexArray->AddLayout(*_vertexBuffer, layout);

		}
			break;
		case FaceOrientation::Back:
		{

		}
			break;
		default:
			break;
		}
		_shader->SetUniform3f("u_CameraPos", camPos);
		_shader->SetUniformMat4f("u_ProjectionMat", projMat);
		_shader->SetUniform3f("u_CubePos", blockPosition);
		_shader->SetUniform3f("u_light", light);
	}

	void Render()
	{
		_shader->SetUniform3f("u_CubePos", blockPosition);
		_shader->SetUniform3f("u_CameraPos", camPos);
		_shader->SetUniform2f("u_CameraAngle", cameraRotation.AngleX, cameraRotation.AngleY);
		_shader->SetUniform3f("u_light", light);
		Renderer::Draw(*_vertexArray, *_indexBuffer, *_shader,*_texture, GL_TRIANGLES);
	} 

	void Render(glm::vec3 position)
	{
		blockPosition = position;
		this->Render();
	}
};

