#include "texture.h"




CTexture::CTexture()
{
	glGenTextures(1, &this->name);
}


CTexture::~CTexture()
{
	glDeleteTextures(1, &this->name);
	this->name = -1;
}

GLuint CTexture::GetName()
{
	return name;
}

void CTexture::Bind(TextureUnit unit)
{
	glActiveTexture(unit);
	glBindTexture(GL_TEXTURE_2D, this->name);
}

inline void set_t2d_params() {
	glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MIN_FILTER, GL_LINEAR);
	glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MAG_FILTER, GL_LINEAR);
	glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_WRAP_S, GL_CLAMP_TO_EDGE);
	glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_WRAP_T, GL_CLAMP_TO_EDGE);
}


Texture CTexture::Create(int width, int height)
{
	auto T = Texture(new CTexture);

	glBindTexture(GL_TEXTURE_2D, T->name);
	glTexImage2D(GL_TEXTURE_2D, 0, GL_RGB32F, width, height, 0, GL_RGB, GL_FLOAT, NULL);
	
	set_t2d_params();

	return T;
	
}

Texture CTexture::CreateDepth(int width, int height)
{
	auto T = Texture(new CTexture);

	glBindTexture(GL_TEXTURE_2D, T->name);
	glTexImage2D(GL_TEXTURE_2D, 0, GL_DEPTH_COMPONENT32F, width, height, 0, GL_DEPTH_COMPONENT, GL_FLOAT, NULL);

	set_t2d_params();

	return T;
}
