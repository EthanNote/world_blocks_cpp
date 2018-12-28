#pragma once
#include <GL\glew.h>
#include <vector>
#include "camera.h"

struct VERTEX_ATTRIBUTE {
	GLuint index;
	GLint size;
	GLenum type;
	GLboolean normalized;
	GLsizei stride;
	const void *ptr;
};

//namespace vertex_attribute {
//	VERTEX_ATTRIBUTE end();
//}
class Renderable
{
protected:
	std::vector<VERTEX_ATTRIBUTE> attributes;
	GLuint VAO=0;
	GLuint VBO=0;
public:
	virtual void Draw()=0;
	void ApplyVertexAttribute();
	void EnableAttributes();
	void DisableAttributes();
};

