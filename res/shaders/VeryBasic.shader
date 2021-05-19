#Vertex Shader
#version 330 core
layout(location = 0)in vec2 v_location;
layout(location = 1)in vec3 v_color;
out vec3 fcolor;
void main()
{
	fcolor = v_color;
	gl_Position.xy = v_location;
}

#Fragment Shader
#version 330 core
in vec3 fcolor;
out vec4 color;
void main()
{
	color.rgb = fcolor;
}