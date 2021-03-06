#include <GL/glew.h>
#include <GLFW/glfw3.h>
#include "renderoperation.h"



CRenderOperation::CRenderOperation()
{
}


CRenderOperation::~CRenderOperation()
{
}

void CSimpleRenderOperation::Draw()
{
	glBegin(GL_POINTS);
	glVertex2i(0, 0);
	glEnd();
}

RenderOperation CSimpleRenderOperation::Create()
{
	return RenderOperation(new CSimpleRenderOperation);
}




void CBlockRenderOperation::Draw()
{
	if (this->pool->blocks.size() < 1) {
		return;
	}
	glBindVertexArray(0);
	glBindBuffer(GL_ARRAY_BUFFER, 0);
	glEnableVertexAttribArray(0);
	glEnableVertexAttribArray(1);
	this->pool->LockRead();
	glVertexAttribPointer(0, 4, GL_FLOAT, GL_FALSE, sizeof(Block), &this->pool->blocks[0]);
	glVertexAttribPointer(1, 3, GL_FLOAT, GL_FALSE, sizeof(Block), &this->pool->blocks[0].color);
	glDrawArrays(GL_POINTS, 0, this->pool->blocks.size());
	this->pool->UnlockRead();

	glDisableVertexAttribArray(0);
	glDisableVertexAttribArray(1);
}

RenderOperation CBlockRenderOperation::Create(BlockPool pool)
{
	auto ptr = new CBlockRenderOperation;
	ptr->pool = pool;
	return RenderOperation(ptr);
}
