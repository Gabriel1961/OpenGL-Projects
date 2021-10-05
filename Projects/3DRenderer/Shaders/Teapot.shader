#Vertex Shader
#version 330 core
layout(location = 0)in vec3 position;
layout(location = 1)in vec3 normal;
uniform mat4 Projection = mat4(1);
uniform mat4 View = mat4(1);
uniform vec3 LightPosition;
uniform mat4 Model = mat4(1);
uniform vec3 camPos;
uniform vec2 u_CameraAngle;
out vec3 v_normal;
out vec3 v_camPosDif;
out vec3 v_lightPosDif;
#define pi 3.14159265359
vec4 ComputeCameraRotation()
{
	mat4 camRot;
	{
		mat3 camRotX = mat3
		(
			cos(u_CameraAngle.x), 0, sin(-u_CameraAngle.x),
			0, 1, 0,
			sin(u_CameraAngle.x), 0, cos(u_CameraAngle.x)
		);
		mat3 camRotY = mat3
		(
			1, 0, 0,
			0, cos(u_CameraAngle.y), sin(-u_CameraAngle.y),
			0, sin(u_CameraAngle.y), cos(u_CameraAngle.y)
		);
		camRot = mat4(camRotY * camRotX);
	}
	return camRot * vec4(position - camPos, 1);
}
void main()
{
	vec3 vertPosInWorldC = (vec4(position, 1) * View * Model).xyz;
	v_normal = normal;
	v_lightPosDif = normalize(LightPosition - vertPosInWorldC);
	gl_Position = ComputeCameraRotation() * Projection * View * Model;
}

#Fragment Shader
#version 330 core
out vec4 FragColor;
in vec3 v_normal;
in vec3 v_lightPosDif;
in vec3 v_camPosDif;
uniform vec4 Color = vec4(1);
uniform vec4 LightColor = vec4(1);
void main()
{
	float ambientStrength = 0.1;
	vec4 ambient = vec4(ambientStrength * LightColor.xyz, 1);
	//vec3 refl = () ()
		FragColor = vec4(ambient.xyz * Color.xyz + LightColor.xyz * Color.xyz * max(dot(v_lightPosDif, v_normal), 0), 1);
}