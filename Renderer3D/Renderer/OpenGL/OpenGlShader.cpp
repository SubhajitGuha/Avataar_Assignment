#include "Includes.h"
#include "OpenGlShader.h"
#include "glad/glad.h"
#include "glm/gtc/type_ptr.hpp"

OpenGlShader::OpenGlShader(const std::string& path)
{
	program = glCreateProgram();

	m_shaders = ParseFile(path);
	if (m_shaders.ComputeShader != "") 
	{
		unsigned int cs = CompileShader(m_shaders.ComputeShader, GL_COMPUTE_SHADER);
		glAttachShader(program, cs);
	}
	else 
	{
		unsigned int vs = CompileShader(m_shaders.VertexShader, GL_VERTEX_SHADER);
		unsigned int fs = CompileShader(m_shaders.Fragmentshader, GL_FRAGMENT_SHADER);

		glAttachShader(program, vs);
		glAttachShader(program, fs);
	}

	if (m_shaders.GeometryShader != "")// all optional shaders must be done in this manner
	{
		unsigned int gs = CompileShader(m_shaders.GeometryShader, GL_GEOMETRY_SHADER);
		glAttachShader(program, gs);
	}
	
	if (m_shaders.TessellationControlShader != "")
	{
		unsigned int tcs = CompileShader(m_shaders.TessellationControlShader, GL_TESS_CONTROL_SHADER);
		glAttachShader(program, tcs);
	}

	if (m_shaders.TessellationEvaluationShader != "")
	{
		unsigned int tes = CompileShader(m_shaders.TessellationEvaluationShader, GL_TESS_EVALUATION_SHADER);
		glAttachShader(program, tes);
	}

	glLinkProgram(program);
	glValidateProgram(program);

	glUseProgram(program);
}

OpenGlShader::~OpenGlShader()
{
	glDeleteProgram(program);
}

unsigned int OpenGlShader::CompileShader(std::string& Shader,unsigned int type)
{
	unsigned int s = glCreateShader(type);
	const char* chr = Shader.c_str();
	int length = Shader.size();
	glShaderSource(s, 1, &chr, nullptr);
	glCompileShader(s);

	int id;
	glGetShaderiv(s, GL_COMPILE_STATUS, &id);
	if (id == GL_FALSE)//if the shader code is not successfully compiled
	{
		int length;
		glGetShaderiv(s, GL_INFO_LOG_LENGTH, &length);
		char* message = new char[length];
		glGetShaderInfoLog(s, length, &length, message);
		std::cerr<<message;
	}

	return s;
}

Shaders OpenGlShader::ParseFile(const std::string& path)
{
	enum type {
		VERTEX_SHADER, FRAGMENT_SHADER, GEOMETRY_SHADER, TESS_CONTROL_SHADER, TESS_EVALUATION_SHADER, COMPUTE_SHADER
	};

	std::ifstream stream(path);
	if (&stream == nullptr)
		std::cerr<<"Shader File Not Found!!";
	std::string ShaderCode;
	std::string Shader[6];//as for now we have 6 shaders
	int index;
	while (std::getline(stream, ShaderCode))
	{
		if (ShaderCode.find("#shader vertex") != std::string::npos)
		{
			index = type::VERTEX_SHADER;
			continue;
		}
		if (ShaderCode.find("#shader fragment") != std::string::npos)
		{
			index = type::FRAGMENT_SHADER;
			continue;
		}
		if (ShaderCode.find("#shader geometry") != std::string::npos)
		{
			index = type::GEOMETRY_SHADER;
			continue;
		}

		if (ShaderCode.find("#shader tessellation control") != std::string::npos)
		{
			index = type::TESS_CONTROL_SHADER;
			continue;
		}

		if (ShaderCode.find("#shader tessellation evaluation") != std::string::npos)
		{
			index = type::TESS_EVALUATION_SHADER;
			continue;
		}
		if (ShaderCode.find("#shader compute") != std::string::npos)
		{
			index = type::COMPUTE_SHADER;
			continue;
		}

		Shader[index].append(ShaderCode + "\n");
	}
	return { Shader[0],Shader[1],Shader[2],Shader[3],Shader[4], Shader[5] };
}

void OpenGlShader::Bind()
{
	glUseProgram(program);
}

void OpenGlShader::UnBind()
{
	glUseProgram(0);
}

void OpenGlShader::SetMat4(const std::string& str, glm::mat4& UniformMat4, size_t count)
{
	UploadUniformMat4(str, UniformMat4,count);
}

void OpenGlShader::SetInt(const std::string& str, const int& UniformInt)
{
	UploadUniformInt(str, UniformInt);
}

void OpenGlShader::SetFloat(const std::string& str,const float& UniformFloat)
{
	UpladUniformFloat(str, UniformFloat);
}

void OpenGlShader::SetFloatArray(const std::string& str, float& UniformFloatArr,size_t count)
{
	UpladUniformFloatArray(str, count, UniformFloatArr);
}

void OpenGlShader::SetFloat4(const std::string& str, const glm::vec4& UniformFloat4)
{
	UpladUniformFloat4(str, UniformFloat4);
}

void OpenGlShader::SetFloat3(const std::string& str, const glm::vec3& UniformFloat3)
{
	UpladUniformFloat3(str, UniformFloat3);
}

void OpenGlShader::SetFloat3Array(const std::string& str,const float* pointer, size_t count)
{
	UpladUniformFloat3Array(str, pointer, count);
}

void OpenGlShader::SetFloat4Array(const std::string& str, const float* arr, size_t count)
{
	UpladUniformFloat4Array(str, arr, count);
}

void OpenGlShader::SetIntArray(const std::string& str, const size_t size, const void* pointer)
{
	UploadIntArray(str, size, pointer);
}

//opengl specific upload uniform
void OpenGlShader::UploadUniformMat4(const std::string& str, glm::mat4& UniformMat4, size_t count)
{
	unsigned int location = glGetUniformLocation(program, str.c_str());
	glUniformMatrix4fv(location, count, false, glm::value_ptr(UniformMat4));

}

void OpenGlShader::UploadUniformInt(const std::string& str, const int& UniformInt)
{
	unsigned int location = glGetUniformLocation(program, str.c_str());
	glUniform1i(location, UniformInt);
}

void OpenGlShader::UploadIntArray(const std::string& str, const size_t size, const void* pointer)
{
	auto location = glGetUniformLocation(program, str.c_str());
	glUniform1iv(location, size,(const GLint*) pointer);
}

void OpenGlShader::UpladUniformFloat(const std::string& str,const float& UniformFloat)
{
	unsigned int location = glGetUniformLocation(program, str.c_str());
	glUniform1f(location, UniformFloat);
}

void OpenGlShader::UpladUniformFloatArray(const std::string& str,size_t count, float& UniformFloatArr)
{
	unsigned int location = glGetUniformLocation(program, str.c_str());
	glUniform1fv(location,count, &UniformFloatArr);
}

void OpenGlShader::UpladUniformFloat4(const std::string& str, const glm::vec4& UniformFloat4)
{
	unsigned int location = glGetUniformLocation(program, &str[0]);
	glUniform4f(location, UniformFloat4.r, UniformFloat4.g, UniformFloat4.b, UniformFloat4.a);
}
void OpenGlShader::UpladUniformFloat3(const std::string& str, const glm::vec3& UniformFloat3)
{
	uint32_t location = glGetUniformLocation(program, str.c_str());
	glUniform3f(location, UniformFloat3.x, UniformFloat3.y, UniformFloat3.z);
}
void OpenGlShader::UpladUniformFloat3Array(const std::string& str,const float *pointer , size_t count)
{
	uint32_t location = glGetUniformLocation(program, str.c_str());
	glUniform3fv(location, count,(const GLfloat*) pointer);
}
void OpenGlShader::UpladUniformFloat4Array(const std::string& str, const float* pointer, size_t count)
{
	uint32_t location = glGetUniformLocation(program, str.c_str());
	glUniform4fv(location, count, (const GLfloat*)pointer);
}