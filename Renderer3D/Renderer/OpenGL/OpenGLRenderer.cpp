#include "OpenGLRenderer.h"
#include "glad/glad.h"
#include "Sphere.h" //contains the vertex attributes for a sphere

OGLRenderer::OGLRenderer(int width, int height)
{
	sun_direction = { -3.0f,-5.0f,5.0f }; //define the sun direction
	lightingShader = std::make_shared<OpenGlShader>("Shaders/LightingShader.glsl");
	Init(width,height);
}

OGLRenderer::~OGLRenderer()
{
}

void OGLRenderer::Init(int width, int height)
{
	glViewport(0, 0, width, height);

	glEnable(GL_BLEND);
	glBlendFunc(GL_SRC_ALPHA, GL_ONE_MINUS_SRC_ALPHA);
	glEnable(GL_DEPTH_TEST);
	glDepthFunc(GL_LESS);
	glEnable(GL_CULL_FACE);
	glCullFace(GL_BACK);

	//pass buffers to gpu
	glGenVertexArrays(1, &vertexArrayID);
	glBindVertexArray(vertexArrayID);

	uint32_t vertexBuff_PosID;
	glGenBuffers(1, &vertexBuff_PosID);
	glBindBuffer(GL_ARRAY_BUFFER, vertexBuff_PosID);
	glBufferData(GL_ARRAY_BUFFER, sizeof(SphereVertices), SphereVertices, GL_STATIC_DRAW);

	glEnableVertexAttribArray(0);
	glVertexAttribPointer(0, 3, GL_FLOAT, GL_FALSE, 0, 0);

	uint32_t vertexBuff_NormID;
	glGenBuffers(1, &vertexBuff_NormID);
	glBindBuffer(GL_ARRAY_BUFFER, vertexBuff_NormID);
	glBufferData(GL_ARRAY_BUFFER, sizeof(SphereNormal), SphereNormal, GL_STATIC_DRAW);
	
	glEnableVertexAttribArray(1);
	glVertexAttribPointer(1, 3, GL_FLOAT, GL_FALSE, 0, 0);
}

void OGLRenderer::SetViewport(int width, int height)
{
	glViewport(0, 0, width, height);
}

void OGLRenderer::Draw(Camera& camera, glm::mat4 transform, float diffuse, float specular, int isHit, const glm::vec3& color)
{
	lightingShader->Bind();
	lightingShader->SetMat4("u_Model", transform);
	lightingShader->SetMat4("u_ProjectionView", camera.GetProjectionView());
	lightingShader->SetFloat3("u_sunDirection", sun_direction);
	lightingShader->SetFloat3("u_viewDirection", camera.GetViewDirection());
	lightingShader->SetFloat3("u_modelColor", color);
	lightingShader->SetFloat("u_diffuse", diffuse);
	lightingShader->SetFloat("u_specular", specular);
	lightingShader->SetInt("u_isHit", isHit);

	glBindVertexArray(vertexArrayID);
	uint32_t verticesCount = (sizeof(SphereVertices) / sizeof(float)) / 3;
	glDrawArrays(GL_TRIANGLES, 0, verticesCount);
}

void OGLRenderer::Clear()
{
	glClear(GL_COLOR_BUFFER_BIT | GL_DEPTH_BUFFER_BIT);
}

void OGLRenderer::ClearColor(const glm::vec4& color)
{
	glClearColor(color.r, color.g, color.b, color.a);
}
