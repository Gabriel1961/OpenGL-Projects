#Vertex Shader
#version 330 core
layout(location = 0) in vec3 v_positions;
#define pi 3.14159265359
uniform vec3 u_CubePos;
uniform vec3 u_CameraPos;
uniform vec2 u_CameraAngle;
uniform mat3 u_mvp;
uniform vec4 u_CubeColor;
out vec4 CubeColor;
void main()
{
	CubeColor = u_CubeColor;
	float zNear = 0;
	float zFar = 1000;
	float fov = pi / 2;
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
	vec4 fposition = camRot *vec4(u_mvp*(v_positions- u_CameraPos + u_CubePos),1);
	
	mat4 viewToProjectionMat = mat4
	(
		1/tan(fov/2), 0, 0, 0,
		0, 1 / tan(fov / 2), 0, 0,
		0, 0, zFar/(zFar-zNear), 1,
		0, 0, -(zFar*zNear)/(zFar-zNear), 0
	);

	gl_Position = viewToProjectionMat*fposition;
}

#Fragment Shader
#version 330 core
in vec4 CubeColor;
out vec4 v_color;
void main()
{
	v_color = CubeColor;
}