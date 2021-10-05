#Vertex Shader
#version 330 core
layout(location = 0) in vec2 v_position;
uniform mat3 MVP = mat3(1);
#define pi 3.14159265359
void main()
{
	gl_Position.xy = (vec3(v_position, 1) * MVP).xy;
}

#Fragment Shader
#version 330 core
out vec4 v_color;
uniform vec4 COLOR;
void main()
{
	v_color = COLOR;
}