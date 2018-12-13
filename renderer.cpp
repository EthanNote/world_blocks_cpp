#include<GL/glew.h>
#include "renderer.h"



CRenderer::CRenderer()
{
	glGenVertexArrays(1, &vao);
}


CRenderer::~CRenderer()
{
}


CBlockRenderer::CBlockRenderer()
{
	

}


void CBlockRenderer::Draw()
{
	shader->UseProgram();

	auto error = glGetError();
	glBindVertexArray(0);
	glBindBuffer(GL_ARRAY_BUFFER, 0);
	glEnableVertexAttribArray(0);

	this->pool->LockRead();
	glVertexAttribPointer(0, 4, GL_FLOAT, GL_FALSE, sizeof(Block), &this->pool->blocks[0]);
	glDrawArrays(GL_POINTS, 0, this->pool->blocks.size());
	this->pool->UnlockRead();
	
	glDisableVertexAttribArray(0);

	glUseProgram(0);
	error = glGetError();
}

//BlockRenderer Create(BlockShader shader, BlockPool pool)
BlockRenderer CBlockRenderer::Create(BlockShader shader, BlockPool pool)
{
	auto renderer = std::shared_ptr<CBlockRenderer>(new CBlockRenderer);
	renderer->shader = shader;
	renderer->pool = pool;
	return renderer;
}




CScreenRenderer::CScreenRenderer()
{
}

void CScreenRenderer::Draw()
{
}
