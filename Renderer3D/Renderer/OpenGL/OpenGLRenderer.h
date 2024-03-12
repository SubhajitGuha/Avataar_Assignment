#pragma once
#include "Includes.h"
#include "Camera/Camera.h"
#include "OpenGlShader.h"

//This class is only used to render spheres
class OGLRenderer
{
public:
	OGLRenderer(int width, int height);
	~OGLRenderer();
	void Init(int width, int height);
	void SetViewport(int width, int height);
	void Draw(Camera& camera, glm::mat4 transform, float diffuse, float specular, int isHit, const glm::vec3& color = {1.0,1.0,1.0});
	void Clear();
	void ClearColor(const glm::vec4& color);
public:
	glm::vec3 sun_direction;
private:
	std::shared_ptr<OpenGlShader> lightingShader;
	uint32_t vertexArrayID;
};