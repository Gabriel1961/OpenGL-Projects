#Vertex Shader
#version 330 core
layout(location = 0)in vec2 v_location;
void main()
{
	gl_Position.xy = v_location;
}

#Fragment Shader
#version 330 core
out vec4 color;
void main()
{
	color.rgb = vec3(0,1,0);
}