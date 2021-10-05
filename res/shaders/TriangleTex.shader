#Vertex Shader
#version 330 core
layout(location = 0)in vec2 v_location;
layout(location = 1)in vec2 vertexUV;
out vec2 UV;
void main()
{
	UV = vertexUV;
	gl_Position.xy = v_location;
}

#Fragment Shader
#version 330 core

in vec2 UV;
uniform sampler2D texSampler;
out vec4 color;
void main()
{
	color.rgb = texture(texSampler,UV).rgb;
}