//#shader vertex
#version 410 core

layout (location = 0) in vec3 position;
layout (location = 1) in vec3 normal;

out vec3 ws_normal;
out vec3 ws_position;

uniform mat4 u_Model;
uniform mat4 u_ProjectionView;

void main()
{
	ws_normal = mat3(u_Model) * normal;
	vec4 ws_pos = u_Model * vec4(position,1.0);
	ws_position = ws_pos.xyz;

	gl_Position = u_ProjectionView * ws_pos; //to clip-space
}

//#shader fragment
#version 410 core
layout (location = 0) out vec4 color;

in vec3 ws_normal;
in vec3 ws_position;

uniform vec3 u_modelColor;
uniform vec3 u_sunDirection;
uniform vec3 u_viewDirection;

uniform float u_diffuse; //Kd
uniform float u_specular; //Ks
uniform int u_isHit; //to highlight the sphere

vec3[2] u_pointLights;

void SetPointLightPositions()
{
	u_pointLights[0] = vec3(-15.0,-10.0,-5.0);
	u_pointLights[1] = vec3(-15.0,10.0,-5.0);
}

//this function calculates the Phong Lighting model
vec3 PhongLighting(vec3 LightVec, vec3 LightColor)
{
		vec3 L = normalize(-LightVec); //light direction
		vec3 N = normalize(ws_normal); //normal direction
		vec3 V = normalize(-u_viewDirection);

		//phong material model
		//diffuse
		float NdotL = clamp(dot(L,N),0.0,1.0); //geometry term
		vec3 diffuse = u_modelColor * NdotL * u_diffuse;

		//specular
		vec3 H = normalize(L + V); //half vector
		float NdotH = clamp(dot(N,H),0.0,1.0);
		vec3 specular = pow(NdotH,64) * u_specular * LightColor; //specular lobe with LightColor

		return diffuse + specular;
}

void main()
{
	if(u_isHit == 1)
	{
		color = vec4(ws_normal*0.5 + 0.5, 1.0);
		return;
	}

	SetPointLightPositions();
	vec3 totalColor = vec3(0.0);

	//directional light
	totalColor += PhongLighting(u_sunDirection, vec3(1.0,0.8,0.7));

	//point lights
	for(int i=0;i<2;i++)
	{
		vec3 lightVector = ws_position - u_pointLights[i];		

		float attenuation = 1.0 / ( 0.01 * length(lightVector) * length(lightVector) );
		totalColor += PhongLighting(lightVector,vec3(1.0,0.8,0.2))*attenuation;
	}

	totalColor += vec3(0.2); //adding a constant ambiant term
	color = vec4(totalColor,1.0);
}