#include "GraphGrid.h"
#include "../../OpenGLWrapper/IndexBuffer.h"
#include "../../OpenGLWrapper/VertexBuffer.h"
#include "../../OpenGLWrapper/VertexArray.h"
#include "../../OpenGLWrapper/Renderer.h"
#include "GraphingCalculator.h"
#include "Text/Text.h"
#define BASE_DIR "Projects/Graphing Calculator/"
#define SHADER_DIR "Projects/Graphing Calculator/Shaders/"
VertexBuffer* vb;
IndexBuffer* ib;
VertexArray* va;
Shader* shader;
float GraphGrid::spacing;
float GraphGrid::sizeY;
float GraphGrid::sizeX;
Transform GraphGrid::transform;
std::vector<float> GenLineBufferData(int sizeX, int sizeY, float spacing);

void GraphGrid::Start(int sizeX, int sizeY, float spacing)
{
	GraphGrid::spacing = spacing;

	int sizeLinePosData = 2/*Line has 2 points*/ * 2/*X and Z Coords*/ * sizeX;

	auto posData = GenLineBufferData(sizeX, sizeY, spacing);
	float* linePosData = &posData[0];
	vb = new VertexBuffer(posData.size() * sizeof(float), linePosData);
	va = new VertexArray();
	VertexBufferLayout vbl;
	vbl.Push<float>(2);
	va->AddLayout(*vb, vbl);

	int ibCount = posData.size() / 2;
	uint* ibData = new uint[ibCount];
	for (int i = 0; i < ibCount; i++)
		ibData[i] = i;
	ib = new IndexBuffer(ibCount, ibData, GL_UNSIGNED_INT);

	shader = new Shader(SHADER_DIR "SolidColor.shader");


}

std::vector<Text*> GetNumbersOnGraph(const glm::ivec2& size)
{
	std::vector<Text*> vec;
	const int halfSizeX = size.x / 2;
	for (int i = -halfSizeX; i <= halfSizeX; i++)
	{
		vec.push_back(new Text(std::to_string(i)));
		vec[vec.size() - 1]->transform.SetTranslation(
			{
				1,0,GraphGrid::GetSpacing() * i,
				0,1,-0.05,
				0,0,1,
			});
	}
	return vec;
}

void GraphGrid::Draw()
{
	shader->SetUniform4f("uColor", glm::vec4(1, 1, 1, 1));
	shader->SetUniformMat3f("uAspect", Calculator::GetAspectMat());
	shader->SetUniformMat3f("MVP", transform.GetMatrix());
	Renderer::Draw(*va, *ib, *shader, GL_LINES);
}

std::vector<float> GenLineBufferData(const int sizeX, const int sizeY, const float spacing)
{
	int halfSizeX = sizeX / 2;
	int halfSizeY = sizeY / 2;
	std::vector <float> vec;
	for (int i = -halfSizeX; i <= halfSizeX; i++)
	{
		vec.push_back(i * spacing);
		vec.push_back(-halfSizeY * spacing);
		vec.push_back(i * spacing);
		vec.push_back(halfSizeY * spacing);
	}

	for (int i = -halfSizeY; i <= halfSizeY; i++)
	{
		vec.push_back(-halfSizeX * spacing);
		vec.push_back(i * spacing);
		vec.push_back(halfSizeX * spacing);
		vec.push_back(i * spacing);
	}
	return vec;
}
