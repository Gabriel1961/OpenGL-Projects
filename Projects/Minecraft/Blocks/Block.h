#pragma once
#include <vector>
#include <unordered_map>
#include "./BlockFace.h"
#include "../../OpenGLWrapper/Texture.h"
#include "../../OpenGLWrapper/Common.h"
class Block
{
private:
	std::vector<BlockFace> _faces;
public:
	Block(std::unordered_map<Texture,FaceOrientation> texOrientationMap)
	{
		for(auto mappedItem : texOrientationMap)
		{
			
		}
	}
};