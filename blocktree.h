#pragma once
#include "block.h"

class CBlockTree
{
public:
	CBlockTree();
	~CBlockTree();

private:
	void _dfs(Block* root, blockcallback callback);

protected:
	CBlockPool* pool;
	Block* root;

public:
	//BlockTree(CBlockPool& pool, Block&root);
	void DFS(blockcallback callback);
	void Insert(Block& block);
	void Remove(Block & block, bool skip_verify = false);
};

