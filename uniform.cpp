#include <GL/glew.h>
#include "uniform.h"


void UniformMatrix::Set(void * value)
{
	glUseProgram(this->program);
	glUniformMatrix4fv(this->location, 1, this->transpose, (GLfloat*)value);
	glUseProgram(0);
}

void UniformMatrix::Set(glm::mat4 & mat)
{
	this->Set(&mat[0][0]);
}

void UniformVec4::Set(void * value)
{
	glUseProgram(this->program);
	glUniform4fv(this->location, 1, (GLfloat*)value);
	glUseProgram(0);
}

void UniformVec4::Set(glm::vec4 & vec)
{
	this->Set(&vec[0]);
}

void UniformInt::Set(void * value)
{
	glUseProgram(this->program);
	glUniform1i(this->location, (GLint)value);
	glUseProgram(0);
}

void UniformInt::Set(int value)
{
	this->Set((void*)value);
}

void UniformIntArray::Set(void * value)
{
	throw "Unable set array via `Set(void * value)`, use `Set(std::vector<int>& integers)` instead.";
}

void UniformIntArray::Set(std::vector<int>& integers)
{
	glUseProgram(this->program);
	glUniform1iv(this->location, integers.size(), &integers[0]);
	glUseProgram(0);
}

void UniformSampler2D::Set(TextureUnit unit)
{
	return UniformInt::Set((int)(unit - TextureUnit::TEXTURE0));
}
