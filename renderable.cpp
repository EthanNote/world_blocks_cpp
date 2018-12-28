#include "renderable.h"

void Renderable::ApplyVertexAttribute()
{
	for (auto i = attributes.begin(); i != attributes.end(); i++) {
		glVertexAttribPointer(i->index, i->size, i->type, i->normalized, i->stride, i->ptr);
	}
}

void Renderable::EnableAttributes()
{
	for (auto i = attributes.begin(); i != attributes.end(); i++) {
		glEnableVertexAttribArray(i->index);
	}
}

void Renderable::DisableAttributes()
{
	for (auto i = attributes.begin(); i != attributes.end(); i++) {
		glDisableVertexAttribArray(i->index);
	}
}

//VERTEX_ATTRIBUTE vertex_attribute::end()
//{
//	static VERTEX_ATTRIBUTE* attr = NULL;
//	if(attr==NULL){
//		attr = new VERTEX_ATTRIBUTE;
//		memset(attr, 0, sizeof(VERTEX_ATTRIBUTE));
//	}
//	return *attr;
//}
