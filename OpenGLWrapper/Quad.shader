#Vertex Shader
#version 330 core
layout(location = 0) in vec2 v_position;
uniform mat4 transform;
void main()
{
	gl_Position.xy = (vec4(v_position, 1, 1) * transform).xy;
}

#Fragment Shader
#version 330 core
out vec4 v_color;
uniform vec4 color;
void main()
{
	v_color = color;
}