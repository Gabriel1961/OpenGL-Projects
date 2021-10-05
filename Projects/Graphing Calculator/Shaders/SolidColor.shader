#Vertex Shader
#version 330 core
layout(location = 0)in vec2 v_location;
uniform mat3 uAspect = mat3(1);
uniform mat3 MVP = mat3(1);
void main()
{
	gl_Position.xy = (vec3(v_location, 1) * MVP * uAspect).xy;
}

#Fragment Shader
#version 330 core
uniform vec4 uColor;
out vec4 color;
void main()
{
	color = uColor;
}