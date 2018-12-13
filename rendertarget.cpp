#include <GL/glew.h>
#include "rendertarget.h"


void CRenderTarget::CreateColorBuffers(int width, int height, int count)
{
	glBindFramebuffer(GL_FRAMEBUFFER, this->fbo);
	std::vector<GLuint> attachments;
	for (int i = 0; i < count; i++) {
		int attachment = GL_COLOR_ATTACHMENT0 + i;
		auto T = CTexture::Create(width, height);
		glFramebufferTexture2D(GL_FRAMEBUFFER, attachment, GL_TEXTURE_2D, T->GetName(), 0);
		this->color_buffers.push_back(T);
		attachments.push_back(attachment);
	}
	glDrawBuffers(count, &attachments[0]);

	glBindFramebuffer(GL_FRAMEBUFFER, 0);
}

void CRenderTarget::CreateDepthBuffer(int width, int height)
{
	glBindFramebuffer(GL_FRAMEBUFFER, this->fbo);
	auto T = CTexture::CreateDepth(width, height);
	glFramebufferTexture2D(GL_FRAMEBUFFER, GL_DEPTH_ATTACHMENT, GL_TEXTURE_2D, T->GetName(), 0);
	glBindFramebuffer(GL_FRAMEBUFFER, 0);

	this->depth_buffer = T;
}

RTCheckResult CRenderTarget::CheckStatus()
{
	glBindFramebuffer(GL_FRAMEBUFFER, this->fbo);

	GLenum Status = glCheckFramebufferStatus(GL_FRAMEBUFFER);

	glBindFramebuffer(GL_FRAMEBUFFER, 0);


	return RTCheckResult(Status);
}

void CRenderTarget::Bind()
{
	glBindFramebuffer(GL_FRAMEBUFFER, this->fbo);
}

CRenderTarget::CRenderTarget()
{
	glCreateFramebuffers(1, &this->fbo);
}


CRenderTarget::~CRenderTarget()
{
	glDeleteFramebuffers(1, &this->fbo);
}

RenderTarget CRenderTarget::Create()
{
	return RenderTarget(new CRenderTarget);
}

void CRenderTarget::Unbind()
{
	glBindFramebuffer(GL_FRAMEBUFFER, 0);
}
