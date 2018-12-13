#pragma once
#include <GLFW/glfw3.h>
#include<vector>
#include "texture.h"

class CRenderTarget;
typedef std::shared_ptr<CRenderTarget> RenderTarget;


enum RTCheckResult {
	COMPLETE = GL_FRAMEBUFFER_COMPLETE,
	INCOMPLETE_ATTACHMENT = GL_FRAMEBUFFER_INCOMPLETE_ATTACHMENT,
	INCOMPLETE_MISSING_ATTACHMENT = GL_FRAMEBUFFER_INCOMPLETE_MISSING_ATTACHMENT,
	INCOMPLETE_DRAW_BUFFER = GL_FRAMEBUFFER_INCOMPLETE_DRAW_BUFFER,
	INCOMPLETE_READ_BUFFER = GL_FRAMEBUFFER_INCOMPLETE_READ_BUFFER
};


class CRenderTarget
{
	GLuint fbo=0;	
public:
	std::vector<Texture> color_buffers;
	Texture depth_buffer;

	void CreateColorBuffers(int width, int height, int count);
	void CreateDepthBuffer(int width, int height);

	RTCheckResult CheckStatus();
	
	void Bind();
	CRenderTarget();
	~CRenderTarget();
	
	static RenderTarget Create();
	static void UnbindAll();
};
