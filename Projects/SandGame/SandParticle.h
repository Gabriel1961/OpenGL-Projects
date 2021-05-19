#pragma once
#include "Particle.h"


class SandParticle : public Particle
{
private:
	void SwapParticle(Point newPos)
	{
		auto otherOwner = ownerMatrix[newPos.Y][newPos.X].owner;
		if (otherOwner == NULL)
			return;
		otherOwner->position = position;
		ownerMatrix[position.Y][position.X].owner = otherOwner;
		ownerMatrix[position.Y][position.X].type = otherOwner->type;
		
		ownerMatrix[newPos.Y][newPos.X].owner = this;
		ownerMatrix[newPos.Y][newPos.X].type = this->type;
		position = newPos;
	}
public:
	SandParticle(Point _pos, MatrixElementInfo** _ownerMatrix, int _ownerMatrixHeight, int _ownerMatrixWidth, std::vector<Particle*>& _ownerParticleList) : Particle(_pos,_ownerMatrix,_ownerMatrixHeight,_ownerMatrixWidth,_ownerParticleList)
	{
		type = ParticleType::Sand;
		ownerMatrix[position.Y][position.X].owner = this;
		speed = 4;
		color = glm::vec3(255 / 255.0, 244 / 255.0, 100/ 255.0);
	}

	void UpdatePos() override
	{
		for (int i = 0; i <= speed; i++)
		{
			if (position.Y >= ownerMatrixHeight - 1 || position.X >= ownerMatrixWidth -1)
				return;
			if (ownerMatrix[position.Y+1][position.X].type == ParticleType::None)
			{
				Move(Point(position.X, position.Y + 1));
			}
			else if (ownerMatrix[position.Y + 1][position.X].type == ParticleType::Fluid)
			{
				SwapParticle(Point(position.X, position.Y + 1));
			}
			else if (position.X != 0 && ownerMatrix[position.Y + 1][position.X - 1].type == ParticleType::None)
			{
				Move(Point(position.X - 1, position.Y + 1));
			}
			else if (position.X != 0 && ownerMatrix[position.Y + 1][position.X - 1].type == ParticleType::Fluid)
			{
				SwapParticle(Point(position.X - 1 , position.Y + 1));
			}
			else if (position.X != ownerMatrixWidth - 1 && ownerMatrix[position.Y+ 1][position.X + 1].type == ParticleType::None)
			{
				Move(Point(position.X + 1, position.Y + 1));
			}
			else if (position.X != ownerMatrixWidth - 1 && ownerMatrix[position.Y + 1][position.X + 1].type == ParticleType::Fluid)
			{
				SwapParticle(Point(position.X + 1, position.Y + 1));
			}
			else
				return;
		}
	}
};
