#pragma once
#include <vector>
#include "SandParticle.h"
class ParticleMatrix
{
private:
	MatrixElementInfo** particlePosArray;
	std::vector<Particle*> particles;
	int posArrayWidth,posArrayHeight;
	
	void UpdateParticleSubArray(int startIndex, int length);
	std::pair<float, float> NormalizeCoords(Point p);
public:
	
	ParticleMatrix(int width, int height);
	
	void AddParticle(ParticleType type, Point position);
	void UpdateAllParticles();
	void UpdateParticlesThreadig(int threads=4);
	std::pair<float*,int> GetVertexRenderPos();
	int GetParticleCount(){ return particles.size(); }
};