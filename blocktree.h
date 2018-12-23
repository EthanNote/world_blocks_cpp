#pragma once
#include "block.h"
#include<memory>
class CBlockTree;
typedef std::shared_ptr<CBlockTree> BlockTree;


class CBlockTree
{
public:
	CBlockTree();
	~CBlockTree();

private:
	void _dfs(int buffer_index, blockcallback callback);

protected:
	BlockPool pool;
	//Block* root;

public:
	//BlockTree(CBlockPool& pool, Block&root);
	void DFS(blockcallback callback);
	void Insert(Block& block);
	void Remove(Block & block, bool skip_verify = false);
	static BlockTree Create(BlockPool pool, int root_level);
};

