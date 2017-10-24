#ifndef SHADER_H
#define SHADER_H

#include "Glad/glad.h"

#define GLM_FORCE_RADIANS

#include <glm/glm.hpp>
#include <glm/gtc/matrix_transform.hpp>

#include <string>
#include <map>

#include "Glad/glad.h"

class Shader
{
public:
	Shader();
	Shader(const std::string &vertexFilename, const std::string &fragmentFilename);

	void Initialise(const std::string &vertexFilename, const std::string &fragmentFilename);
	void Terminate();

	void Bind();
	void UnBind();

	void setInt(const std::string &uniform, int value);
	void setFloat(const std::string &uniform, float value);
	void setBool(const std::string &uniform, bool value);
	void setVec2(const std::string &uniform, const glm::vec2 &value);
	void setVec3(const std::string &uniform, const glm::vec3 &value);
	void setVec4(const std::string &uniform, const glm::vec4 &value);
	void setMat2(const std::string &uniform, const glm::mat2 &value, bool transpose = false);
	void setMat3(const std::string &uniform, const glm::mat3 &value, bool transpose = false);
	void setMat4(const std::string &uniform, const glm::mat4 &value, bool transpose = false);

	unsigned int GetUniformLocation(const std::string &uniform);

private:
	std::string loadFile(const std::string &filename);
	void compileShader(GLuint shader, const std::string &shaderCode);
	std::string validateShader(GLuint shader);
	std::string validateProgram(GLuint program);

private:
	std::map<std::string, GLuint> mUniformList;
	std::map<std::string, GLuint> mConstantUniformList;

	GLuint                          mVertexID;
	GLuint                          mFragmentID;
	GLuint                          mProgramID;
};

#endif