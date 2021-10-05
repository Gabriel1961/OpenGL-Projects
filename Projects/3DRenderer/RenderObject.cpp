#include "./RenderObject.h"
#include "3DRenderingEngine.h"
#include "../OpenGLWrapper/Common.h"
#include "./RenderObject.h"
#include "../ScreenSize.h"
#include <fstream>
#include <algorithm>
using namespace std;
using namespace glm;
RenderObject::~RenderObject()
{
	delete vb;
	delete ib;
	delete shader;
	delete va;
	for (int i = 0; i < textures.size(); i++)
		delete textures[i];
}

/// <summary>
/// Calculates normals for each vertex (uses interpolation)
/// </summary>
/// <param name="ib"></param>
/// <param name="vb"></param>
/// <returns>VertexBufferData having a vec3(position) and a vec3(normal) on the same vertex</returns>
vector<float> CalculateNoramls(const vector<uint>& ib, const vector<float>& vb)
{
	vector<float> result;
	result.reserve(vb.size() * 2);
	vector<float> normals(vb.size(), 0);
	vec3 triangle[3];
	for (int it = 0; it < ib.size(); it += 3)
	{
		uint v1 = ib[it];
		uint v2 = ib[it + 1];
		uint v3 = ib[it + 2];
		// Getting the vertexes 
		triangle[0] = vec3(vb[v1 * 3], vb[v1 * 3 + 1], vb[v1 * 3 + 2]);
		triangle[1] = vec3(vb[v2 * 3], vb[v2 * 3 + 1], vb[v2 * 3 + 2]);
		triangle[2] = vec3(vb[v3 * 3], vb[v3 * 3 + 1], vb[v3 * 3 + 2]);
		vec3 norm = normalize(cross(triangle[1] - triangle[0], triangle[2] - triangle[0]));
		// TODO Normalize

		// Setting the normals
		if (normals[v1 * 3] != 0 && normals[v1 * 3 + 1] != 0 && normals[v1 * 3 + 2] != 0)
		{
			normals[v1 * 3] = (norm.x + normals[v1 * 3]) / 2;
			normals[v1 * 3 + 1] = (norm.y + normals[v1 * 3 + 1]) / 2;
			normals[v1 * 3 + 2] = (norm.z + normals[v1 * 3 + 2]) / 2;
		}
		else
		{
			normals[v1 * 3] = norm.x;
			normals[v1 * 3 + 1] = norm.y;
			normals[v1 * 3 + 2] = norm.z;
		}

		if (normals[v2 * 3] != 0 && normals[v2 * 3 + 1] != 0 && normals[v2 * 3 + 2] != 0)
		{
			normals[v2 * 3] = (norm.x + normals[v2 * 3]) / 2;
			normals[v2 * 3 + 1] = (norm.y + normals[v2 * 3 + 1]) / 2;
			normals[v2 * 3 + 2] = (norm.z + normals[v2 * 3 + 2]) / 2;
		}
		else
		{
			normals[v2 * 3] = norm.x;
			normals[v2 * 3 + 1] = norm.y;
			normals[v2 * 3 + 2] = norm.z;
		}

		if (normals[v3 * 3] != 0 && normals[v3 * 3 + 1] != 0 && normals[v3 * 3 + 2] != 0)
		{
			normals[v3 * 3] = (norm.x + normals[v3 * 3]) / 2;
			normals[v3 * 3 + 1] = (norm.y + normals[v3 * 3 + 1]) / 2;
			normals[v3 * 3 + 2] = (norm.z + normals[v3 * 3 + 2]) / 2;
		}
		else
		{
			normals[v3 * 3] = norm.x;
			normals[v3 * 3 + 1] = norm.y;
			normals[v3 * 3 + 2] = norm.z;
		}
	}
	int k = 0;
	for (int i = 0; i < vb.size(); i += 3)
	{
		result.push_back(vb[i]);
		result.push_back(vb[i + 1]);
		result.push_back(vb[i + 2]);
		result.push_back(normals[i]);
		result.push_back(normals[i + 1]);
		result.push_back(normals[i + 2]);
	}
	return result;
}

void RenderObject::ReadObj(const string& path)
{
	fstream fin(path);
	ASSERT(fin.is_open() == true);
	char type;
	fin >> type;
	vector<float> vbData;
	while (type != 'f')
	{
		float v1, v2, v3;
		fin >> v1 >> v2 >> v3;
		vbData.push_back(v1);
		vbData.push_back(v2);
		vbData.push_back(v3);
		fin >> type;
	}
	vector<uint> ibData;
	do
	{
		uint i1, i2, i3;
		fin >> i1 >> i2 >> i3;
		ibData.push_back(i1 - 1);
		ibData.push_back(i2 - 1);
		ibData.push_back(i3 - 1);
	} while (fin >> type);
	va = new VertexArray();

	auto vbNorm = CalculateNoramls(ibData, vbData);
	vb = new VertexBuffer(vbNorm.size() * sizeof(float), &vbNorm[0]);
	VertexBufferLayout vbl;
	vbl.Push<float>(3);
	vbl.Push<float>(3);
	va->AddLayout(*vb, vbl);
	ib = new IndexBuffer(ibData.size(), &ibData[0]);

	// delete[] vbDataNorm;
	fin.close();
}

void RenderObject::Render()
{
	Renderer::Draw(*va, *ib, *shader);
}
