#pragma once
#include "glm/glm.hpp"

struct Shaders {
	std::string VertexShader;
	std::string Fragmentshader;
	std::string GeometryShader;
	std::string TessellationControlShader;
	std::string TessellationEvaluationShader;
	std::string ComputeShader;
	//compute shader, tess shader....
};

class OpenGlShader
{
public:
	OpenGlShader(const std::string& path);
	~OpenGlShader();

	void Bind();
	void UnBind();

	void SetMat4(const std::string& str, glm::mat4& UniformMat4, size_t count=1);
	void SetInt(const std::string& str, const int& UniformInt);
	void SetIntArray(const std::string& str, const size_t size, const void* pointer);
	void SetFloat(const std::string& str,const float& UniformFloat);
	virtual void SetFloatArray(const std::string& str, float& UniformFloat, size_t count);
	void SetFloat4(const std::string& str, const glm::vec4& UniformFloat4);
	void SetFloat3(const std::string& str, const glm::vec3& UniformFloat4);
	void SetFloat3Array(const std::string& str,const float* pointer , size_t count);
	void SetFloat4Array(const std::string& str, const float* arr, size_t count);

private://opengl specific
	unsigned int CompileShader(std::string&, unsigned int);
	Shaders ParseFile(const std::string& path);
	void UploadUniformMat4(const std::string& str, glm::mat4& UniformMat4, size_t count = 1);
	void UploadUniformInt(const std::string& str,const int& UniformInt);
	void UploadIntArray(const std::string& str, const size_t size, const void* pointer);
	void UpladUniformFloat(const std::string& str,const float& UniformFloat);
	void UpladUniformFloatArray(const std::string& str, size_t count, float& UniformFloat);
	void UpladUniformFloat4(const std::string& str, const glm::vec4& UniformFloat4);
	void UpladUniformFloat3(const std::string& str, const glm::vec3& UniformFloat3);
	void UpladUniformFloat3Array(const std::string& str, const float* pointer, size_t count);
	void UpladUniformFloat4Array(const std::string& str, const float* pointer, size_t count);

	unsigned int program;
	Shaders m_shaders;
};