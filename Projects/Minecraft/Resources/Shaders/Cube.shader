#Vertex Shader
#version 330 core
layout(location = 0) in vec3 v_positions;
layout(location = 1) in vec2 UVCoords;
#define pi 3.14159265359
uniform vec3 u_CubePos;
uniform vec3 u_CameraPos;
uniform vec2 u_CameraAngle;
uniform mat4 u_ProjectionMat;
uniform vec3 u_light;
out vec2 UV;
out float light;
void main()
{
	UV = UVCoords;
	// Camera rotation
	mat4 camRot;
	{
		mat3 camRotX = mat3
		(
			cos(u_CameraAngle.x), 0, sin(-u_CameraAngle.x),
			0, 1, 0,
			sin(u_CameraAngle.x),0, cos(u_CameraAngle.x)
		);
		mat3 camRotY = mat3
		(
			1, 0, 0,
			0, cos(u_CameraAngle.y), sin(-u_CameraAngle.y),
			0, sin(u_CameraAngle.y), cos(u_CameraAngle.y)
		);
		camRot = mat4(camRotY * camRotX);
	}
	vec4 fposition = camRot *vec4((v_positions- u_CameraPos + u_CubePos),1);

	gl_Position = u_ProjectionMat *fposition;


}

#Fragment Shader
#version 330 core
in float light;
in vec2 UV;
uniform sampler2D texSamp;
out vec4 v_color;
void main()
{
	v_color.a = 1;
	v_color.rgb = vec3(0,1,1) * light;
	//v_color = texture(texSamp,UV);
}