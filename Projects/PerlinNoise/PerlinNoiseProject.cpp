#include "./PerlinNoiseProject.h"
#include <Quad.h>
#include <PixelDisplay.h>
PixelDisplay* px;
float seeds[Window_Width];
float noiseOut[Window_Width];
float scale = 1;
void PerlinNoise1D(int count, float* seed, int octaves, float* output)
{

}

void PerlinNoiseProject::Start()
{
	px = new PixelDisplay();
	for (int i = 0; i < Window_Width; i++)
		px->screen[100][i] = { 0,0,0,255 };
	px->transform.Transforms().push_back(ScaleTransform(scale, scale, 1));
}

void PerlinNoiseProject::Render()
{
	ImGui::SliderFloat("Scale", &scale, 1, 5);
	((ScaleTransform*)&px->transform.Transforms()[0])->SetScale({ scale,scale,1 });
	px->Render();
	DebugLine::RenderLines();
}

