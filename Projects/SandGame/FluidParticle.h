#pragma once
#include "Particle.h"
class FluidParticle : public Particle
{
private:
	int velocity = 0;
public:	
	FluidParticle(Point _pos, MatrixElementInfo** _ownerMatrix, int _ownerMatrixHeight, int _ownerMatrixWidth, std::vector<Particle*>& _ownerParticleList) : Particle(_pos, _ownerMatrix, _ownerMatrixHeight, _ownerMatrixWidth, _ownerParticleList)
	{
		type = ParticleType::Fluid;
		speed = 14;
		color = glm::vec3(0 / 255.0, 0 / 255.0, 255 / 255.0);
		ownerMatrix[position.Y][position.X].owner = this;
	}
	void UpdatePos() override 
	{
		for (int i = 0; i <= speed; i++)
		{
			if (position.Y >= ownerMatrixHeight - 1 || position.X >= ownerMatrixWidth - 1)
				return;
			if (ownerMatrix[position.Y + 1][position.X].type == ParticleType::None)
			{
				Move(Point(position.X, position.Y + 1));
				velocity++;
			}
			else if (position.X != 0 && ownerMatrix[position.Y + 1][position.X - 1].type == ParticleType::None)
			{
				Move(Point(position.X - 1, position.Y + 1));
				velocity++;
			}
			else if (position.X != ownerMatrixWidth - 1 && ownerMatrix[position.Y + 1][position.X + 1].type == ParticleType::None)
			{
				Move(Point(position.X + 1, position.Y + 1));
				velocity++;
			}
			else
			{
				if (position.X != 0 && ownerMatrix[position.Y][position.X - 1].type == ParticleType::None && position.X != ownerMatrixWidth - 1 && ownerMatrix[position.Y][position.X + 1].type == ParticleType::None)
				{
					if (rand() % 2 == 1)
						Move(Point(position.X + 1, position.Y));
					else
						Move(Point(position.X - 1, position.Y));
				}
				if (position.X != ownerMatrixWidth - 1 && ownerMatrix[position.Y][position.X + 1].type == ParticleType::None)
				{
					Move(Point(position.X + 1, position.Y));
					velocity = 0;
				}
				else if (position.X != 0 && ownerMatrix[position.Y][position.X - 1].type == ParticleType::None)
				{
					Move(Point(position.X - 1, position.Y));
					velocity = 0;
				}
				if(velocity > 10)
				{
					velocity = rand()%10;
					while (velocity)
					{
						if (position.X != ownerMatrixWidth - 1 && position.Y != 0)
							if(ownerMatrix[position.Y - 1][position.X + 1].type == ParticleType::None)
							Move(Point(position.X + 1, position.Y - 1));
						else if (position.X != 0 && position.Y != 0)
							if(ownerMatrix[position.Y - 1][position.X - 1].type == ParticleType::None)
							Move(Point(position.X - 1, position.Y - 1));
						else break;
						velocity--;
					}
				}
				return;
			}
		}
	}
};