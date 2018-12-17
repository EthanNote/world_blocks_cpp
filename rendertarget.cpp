#include <GL/glew.h>
#include "rendertarget.h"


GLenum DrawBuffers[] = {
	GL_COLOR_ATTACHMENT0,
	GL_COLOR_ATTACHMENT1,
	GL_COLOR_ATTACHMENT2,
	GL_COLOR_ATTACHMENT3,
	GL_COLOR_ATTACHMENT4,
	GL_COLOR_ATTACHMENT5,
	GL_COLOR_ATTACHMENT6,
	GL_COLOR_ATTACHMENT7,
	GL_COLOR_ATTACHMENT8,
	GL_COLOR_ATTACHMENT9,
	GL_COLOR_ATTACHMENT10
};


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
	glDrawBuffers(count, DrawBuffers);

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

RTCheckResult CRenderTarget::CheckStatus(std::function<void(RTCheckResult)> callback)
{
	auto result = this->CheckStatus();
	callback(result);
	return result;
}

//void CRenderTarget::Bind()
//{
//	glBindFramebuffer(GL_FRAMEBUFFER, this->fbo);
//	glClear(GL_COLOR_BUFFER_BIT | GL_DEPTH_BUFFER_BIT);
//}

void CRenderTarget::Pass(std::function<void()> drawing)
{
	glBindFramebuffer(GL_FRAMEBUFFER, this->fbo);
	glClear(GL_COLOR_BUFFER_BIT | GL_DEPTH_BUFFER_BIT);

	drawing();

	glBindFramebuffer(GL_FRAMEBUFFER, 0);
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

//void CRenderTarget::UnbindAll()
//{
//	glBindFramebuffer(GL_FRAMEBUFFER, 0);
//}

CRenderTarget* create_default() {
	auto pRT = new CRenderTarget();
	pRT->fbo = 0;
	return pRT;
}

RenderTarget CRenderTarget::Screen()
{
	static RenderTarget rt = RenderTarget(create_default());
	return rt;
}
