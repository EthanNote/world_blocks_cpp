#pragma once
#include "block.h"
#include "terrine.h"

class CRenderOperation
{
public:
	CRenderOperation();
	~CRenderOperation();
	virtual void Draw() = 0;
};

typedef std::shared_ptr<CRenderOperation> RenderOperation;

class CSimpleRenderOperation: public CRenderOperation {
public:
	void Draw() override;
	static RenderOperation Create();
};
typedef std::shared_ptr<CSimpleRenderOperation> SimpleRenderOperation;

class CBlockRenderOperation : public CRenderOperation {
public:
	BlockPool pool;
	void Draw() override;
	static RenderOperation Create(BlockPool pool);
};
typedef std::shared_ptr<CBlockRenderOperation> BlockRenderOperation;


class CTerrineRenderOperation : public CRenderOperation {
public:
	Terrine terrine;
	void Draw() override;
	static RenderOperation Create(Terrine terrine);
};