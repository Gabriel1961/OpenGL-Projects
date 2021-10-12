#pragma once
#include <Common.h>
#include <string>
#include <vector>
class Shader;
/// <summary>
/// Base class for UniformBlockList and UniformBlockItem
/// </summary>
class UniformBlock
{
public:
	virtual void BindUniformBlock(uint shaderProgramId) const = 0;
};

static uint GetNewBindingIdx()
{
	static uint nextIdx = 0;
	nextIdx++;
	return nextIdx - 1;
}

/// <summary>
/// This class wraps the ubo and it updates it every time it needs to render
/// </summary>
/// <typeparam name="T">Type of the data that is inside the vector</typeparam>
template<typename T>
class UniformBlockList : public UniformBlock
{
	uint ID;
	uint bindingIdx;
	std::string name;

public:
	//Constructors
	UniformBlockList(const std::string& uboName, std::vector<T>& objects) :data(objects), name(uboName)
	{
		bindingIdx = GetNewBindingIdx();
		gc(glGenBuffers(1, &ID));
		gc(glBindBuffer(GL_UNIFORM_BUFFER, ID));
		gc(glBufferData(GL_UNIFORM_BUFFER, sizeof(T) * objects.size(), &objects[0], GL_DYNAMIC_DRAW));
		gc(glBindBufferBase(GL_UNIFORM_BUFFER, bindingIdx, ID));
	}

	//Methods
	void BindUniformBlock(uint shaderProgramId) const override
	{
		gc(int uniformIdx = glGetUniformBlockIndex(shaderProgramId, name.c_str())); // TODO add hashtable to shader to store  
		gc(glUniformBlockBinding(shaderProgramId, uniformIdx, bindingIdx));
	}

	//Getters
	uint GetId() { return ID; }
	uint GetBindingIdx() { return bindingIdx; }

	//Fields
	std::vector<T>& data;
};
