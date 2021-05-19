#include "ParticleMatrix.h"
#include "FluidParticle.h"
#include <utility> 
#include <iostream>
#include <thread>
/*
* Matrice
 -----< x = width
 ||||
 ||||
 ||||
 ^
 y = height
*/

ParticleMatrix::ParticleMatrix(int width, int height)
{
	posArrayHeight = height;
	posArrayWidth = width;
	particlePosArray = new MatrixElementInfo*[height];
	for (int i = 0; i < height; i++)
	{
		particlePosArray[i] = new MatrixElementInfo[width];
	}
}

void ParticleMatrix::AddParticle(ParticleType type, Point position)
{
	if (position.X < 0 || position.Y < 0 || position.X >= posArrayWidth || position.Y >= posArrayHeight)
		return;
	if (this->particlePosArray[position.Y][position.X].type != ParticleType::None)
		return;
	switch (type)
	{
	case ParticleType::None:
		return;
	case ParticleType::Sand:
		particles.push_back(new SandParticle(position,this->particlePosArray,posArrayHeight,posArrayWidth,this->particles));
		break;
	case ParticleType::Fluid:
		particles.push_back(new FluidParticle(position, this->particlePosArray, posArrayHeight, posArrayWidth, this->particles));
		break;
	default:
		break;
	}
}

void ParticleMatrix::UpdateAllParticles()
{
	for (auto particle : particles)
	{
		particle->UpdatePos();
	}
}

void ParticleMatrix::UpdateParticlesThreadig(int threads)
{
	using namespace std;
	vector<int> workloads(threads);
	int workload = particles.size()/threads;
	{
		for (int i = 0; i < threads - 1; i++)
		{
			workloads[i] = workload;
		}
		workloads[threads - 1] = workload + particles.size() % threads;
	}
	vector<thread> threadArray;
	int sIndex = 0;
	for (int i = 0; i < threads; i++)
	{
		threadArray.push_back(thread(&ParticleMatrix::UpdateParticleSubArray,this,sIndex,workloads[i]));
		sIndex += workload;
	}
	for (int i = 0; i < threads; i++)
	{
		threadArray[i].join();
	}
}

void ParticleMatrix::UpdateParticleSubArray(int startIndex, int length)
{
	for (int i = 0; i < length; i++)
	{
		particles[startIndex + i]->UpdatePos();
	}
}


/// <summary>
/// Calculeaza pozitile normalizate ale fiecarei particule 
/// </summary>
/// <returns>bufferul,marimea in bytes a bufferului</returns>
std::pair<float*,int> ParticleMatrix::GetVertexRenderPos()
{
	int i = 0;
	// 2 pt poz, 3 pt culoare
	float* data = new float[particles.size()*(2 + 3)];
	for (int j =0;j< particles.size();j++)
	{
		std::pair<float, float> pos = NormalizeCoords(particles[j]->position);
		
		data[i++] = pos.first;
		data[i++] = pos.second;
		glm::vec3& color = particles[j]->color;
		data[i++] = color.r;
		data[i++] = color.g;
		data[i++] = color.b;
	}
	return std::pair<float*,int>(data, particles.size() * (2+3) * 4);
}

std::pair<float, float> ParticleMatrix::NormalizeCoords(Point p)
{
	return std::pair<float,float>(2.0f*(float)p.X/(float)posArrayWidth - 1.0f, -2.0f*(float)p.Y / (float)posArrayHeight +1.0f );
}