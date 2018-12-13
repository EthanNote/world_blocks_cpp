#pragma once
#include "shader.h"
#include <memory>
#include "block.h"

class CRenderer;
class CBlockRenderer;
class CScreenRenderer;


typedef std::shared_ptr<CRenderer> Renderer;
typedef std::shared_ptr<CBlockRenderer> BlockRenderer;
typedef std::shared_ptr<CScreenRenderer> ScreenRenderer;



class CRenderer
{
protected:
	GLuint vao;
public:
	virtual void Draw() = 0;

	CRenderer();
	~CRenderer();
};



class CBlockRenderer :CRenderer {
public:
	BlockShader shader;
	BlockPool pool;
	CBlockRenderer();
	
	virtual void Draw() override;
	
	static BlockRenderer Create(BlockShader shader, BlockPool pool);
};


class CScreenRenderer : CRenderer {
public:
	CScreenRenderer();
	virtual void Draw() override;
};

