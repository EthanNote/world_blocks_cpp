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

void CTerrineRenderOperation::Draw()
{
	if (!terrine->GetSize() || !terrine->IsValid()) {
		return;
	}


	glBindVertexArray(0);
	/*glBindBuffer(GL_ARRAY_BUFFER, 0);
	glEnableVertexAttribArray(0);
	glVertexAttribPointer(0, 3, GL_FLOAT, GL_FALSE, sizeof(TERRINE_TRIANGLE_VERTEX), &terrine->mesh[0]);
	glDrawArrays(GL_TRIANGLES, 0, terrine->mesh.size()*3);*/

	glBindBuffer(GL_ARRAY_BUFFER, terrine->vbo);
	glEnableVertexAttribArray(0);
	glVertexAttribPointer(0, 3, GL_FLOAT, GL_FALSE, sizeof(TERRINE_TRIANGLE_VERTEX), 0);
	glDrawArrays(GL_TRIANGLES, 0, terrine->mesh.size() * 3);

	glDisableVertexAttribArray(0);
}

RenderOperation CTerrineRenderOperation::Create(Terrine terrine)
{
	auto ptr = new CTerrineRenderOperation;
	ptr->terrine = terrine;
	return RenderOperation(ptr);
}
