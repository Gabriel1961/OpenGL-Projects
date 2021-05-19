#pragma once
#include <vector>
#include <assert.h>
#include <GL/glm/vec3.hpp>
struct Point
{
	Point() { X = 0, Y = 0; }
	Point(int x, int y) : X(x), Y(y) {}
	int X, Y;
};

enum class ParticleType
{
	None = 0, Sand = 1, Fluid = 2
};

class Particle;

class MatrixElementInfo
{
public:
	ParticleType type = ParticleType::None;
	Particle* owner = nullptr;

	MatrixElementInfo() {}
	MatrixElementInfo(ParticleType type, Particle* owner)
		: type(type), owner(owner) {}
};

class Particle
{
protected:
	Particle(Point _pos, MatrixElementInfo** _ownerMatrix, int _ownerMatrixHeight, int _ownerMatrixWidth, std::vector<Particle*>& _ownerParticleList) : position(_pos), ownerMatrix(_ownerMatrix), ownerMatrixHeight(_ownerMatrixHeight), ownerMatrixWidth(_ownerMatrixWidth), ownerParticleList(_ownerParticleList)
	{
	}
	void Move(Point p)
	{
		ownerMatrix[position.Y][position.X].type = ParticleType::None;
		ownerMatrix[position.Y][position.X].owner = nullptr;
		ownerMatrix[p.Y][p.X].type = type;
		ownerMatrix[p.Y][p.X].owner = this;
		position = p;
	}

	int speed;
public:
	ParticleType type;
	glm::vec3 color;
	int ownerMatrixWidth;
	int ownerMatrixHeight;
	Point position;

	MatrixElementInfo** ownerMatrix = NULL;
	std::vector<Particle*>& ownerParticleList;
	virtual void UpdatePos() {}
};

