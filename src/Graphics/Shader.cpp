#include "Shader.h"

#include <fstream>
#include <vector>
#include <iostream>
#include <sstream>
#include <algorithm>

Shader::Shader()
{

}

Shader::Shader(const std::string &vertexFilename, const std::string &fragmentFilename)
{
	Initialise(vertexFilename, fragmentFilename);
}

void Shader::Initialise(const std::string &vertexFilename, const std::string &fragmentFilename)
{
	mVertexID = glCreateShader(GL_VERTEX_SHADER);
	mFragmentID = glCreateShader(GL_FRAGMENT_SHADER);

	std::string vertexShaderCode = loadFile(vertexFilename);
	if (vertexShaderCode.empty())
		return;

	std::string fragmentShaderCode = loadFile(fragmentFilename);
	if (fragmentShaderCode.empty())
		return;

	//std::cout << "Compiling shader : " << vertexFilename << std::endl;
	compileShader(mVertexID, vertexShaderCode);

	//std::cout << "Compiling shader : " << fragmentFilename << std::endl;
	compileShader(mFragmentID, fragmentShaderCode);

	//std::cout << "Linking program..." << std::endl;
	mProgramID = glCreateProgram();
	glAttachShader(mProgramID, mVertexID);
	glAttachShader(mProgramID, mFragmentID);
	glLinkProgram(mProgramID);

	std::cerr << validateProgram(mProgramID) << std::endl;
}

void Shader::Terminate()
{
	glDeleteShader(mProgramID);
	glUseProgram(0);
}

std::string Shader::loadFile(const std::string &filename)
{
	std::ifstream stream(filename, std::ios::in);

	if (stream.is_open())
	{
		std::stringstream buffer;
		buffer << stream.rdbuf();
		std::string contents(buffer.str());
		stream.close();
		return contents;
	}
	else
	{
		std::cerr << "Error: Cannot open shader file!" << std::endl;
		return std::string();
	}
}

std::string Shader::validateShader(GLuint shaderID)
{
	GLint result = GL_FALSE;
	int infoLogLength;

	glGetShaderiv(shaderID, GL_COMPILE_STATUS, &result);
	glGetShaderiv(shaderID, GL_INFO_LOG_LENGTH, &infoLogLength);
	std::vector<char> shaderErrorMessage(infoLogLength);

	if (!result)
		return "";

	glGetShaderInfoLog(shaderID, infoLogLength, NULL, &shaderErrorMessage[0]);
	return std::string(shaderErrorMessage.begin(), shaderErrorMessage.end());
}

std::string Shader::validateProgram(GLuint programID)
{
	GLint result = GL_FALSE;
	int infoLogLength;

	glGetProgramiv(programID, GL_LINK_STATUS, &result);
	glGetProgramiv(programID, GL_INFO_LOG_LENGTH, &infoLogLength);
	std::vector<char> programErrorMessage(std::max(infoLogLength, int(1)));

	if (!result)
		return "";

	glGetProgramInfoLog(programID, infoLogLength, NULL, &programErrorMessage[0]);
	return std::string(programErrorMessage.begin(), programErrorMessage.end());
}

void Shader::compileShader(GLuint shaderID, const std::string &shaderCode)
{
	char const *sourcePointer = shaderCode.c_str();
	glShaderSource(shaderID, 1, &sourcePointer, NULL);
	glCompileShader(shaderID);

	std::cerr << validateShader(shaderID) << std::endl;
}

void Shader::Bind()
{
	glUseProgram(mProgramID);
}

void Shader::UnBind()
{
	glUseProgram(0);
}

void Shader::setInt(const std::string &uniform, int value)
{
	GLuint id = glGetUniformLocation(mProgramID, uniform.c_str());
	glUniform1i(id, value);
}

void Shader::setFloat(const std::string &uniform, float value)
{
	GLuint id = glGetUniformLocation(mProgramID, uniform.c_str());
	glUniform1f(id, value);
}

void Shader::setBool(const std::string &uniform, bool value)
{
	GLuint id = glGetUniformLocation(mProgramID, uniform.c_str());
	glUniform1i(id, value);
}

void Shader::setVec2(const std::string &uniform, const glm::vec2 &value)
{
	GLuint id = glGetUniformLocation(mProgramID, uniform.c_str());
	glUniform2f(id, value.x, value.y);
}

void Shader::setVec3(const std::string &uniform, const glm::vec3 &value)
{
	GLuint id = glGetUniformLocation(mProgramID, uniform.c_str());
	glUniform3f(id, value.x, value.y, value.z);
}

void Shader::setVec4(const std::string &uniform, const glm::vec4 &value)
{
	GLuint id = glGetUniformLocation(mProgramID, uniform.c_str());
	glUniform4f(id, value.x, value.y, value.z, value.w);
}

void Shader::setMat2(const std::string &uniform, const glm::mat2 &value, bool transpose)
{
	GLuint id = glGetUniformLocation(mProgramID, uniform.c_str());
	glUniformMatrix2fv(id, 1, transpose, &value[0][0]);
}

void Shader::setMat3(const std::string &uniform, const glm::mat3 &value, bool transpose)
{
	GLuint id = glGetUniformLocation(mProgramID, uniform.c_str());
	glUniformMatrix3fv(id, 1, transpose, &value[0][0]);
}

void Shader::setMat4(const std::string &uniform, const glm::mat4 &value, bool transpose)
{
	GLuint id = glGetUniformLocation(mProgramID, uniform.c_str());
	glUniformMatrix4fv(id, 1, transpose, &value[0][0]);
}

unsigned int Shader::GetUniformLocation(const std::string &uniform)
{
	return glGetUniformLocation(mProgramID, uniform.c_str());
}