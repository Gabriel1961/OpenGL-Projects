#Vertex Shader
#version 330 core
layout(location = 0)in vec2 v_location;
layout(location = 1)in vec4 v_color;
out vec4 fcolor;
#define pi 3.14159265359
uniform mat2 u_AspectR;
uniform float u_angle;
void main()
{
	mat2 spin = mat2(
		cos(u_angle), cos(u_angle+pi/2),
		sin(u_angle), sin(u_angle+pi/2)
	);
	gl_Position.xy = u_AspectR * spin*v_location;
	fcolor = v_color;
}

#Fragment Shader
#version 330 core
in vec4 fcolor;
out vec4 color;
void main()
{
	color = fcolor;
}