#Vertex Shader
#version 330 core
layout(location = 0)in vec3 position;
uniform mat4 Projection = mat4(1);
uniform mat4 View = mat4(1);
uniform mat4 Model = mat4(1);
uniform vec3 camPos;
#define pi 3.14159265359
void main()
{
	gl_Position = (vec4(position - camPos, 1) * Projection * View * Model);
}

#Fragment Shader
#version 330 core
uniform vec4 Color = vec4(1);
out vec4 color;
void main()
{
	color = Color;
}