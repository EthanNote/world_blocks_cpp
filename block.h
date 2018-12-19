#pragma once
#include<memory>
#include<vector>
#include<thread>
#include<mutex>
#include<shared_mutex>

class Block {
public:
	int x;
	int y;
	int z;
	int level;
	int color;
	int face_mask;
	int type_id;

	int children[8];
};

struct BlockColor {
	float r;
	float g;
	float b;
	float a;
};


typedef std::vector<BlockColor> BlockPalette;


class CBlockPool {

	std::shared_mutex lock;

public:
	std::vector<Block> blocks;
	Block & Alloc();
	Block & Alloc(Block & block);


	void LockRead();
	void UnlockRead();

	void LockWrite();
	void UnlockWrite();
	static std::shared_ptr<CBlockPool> Create();
};

typedef std::shared_ptr<CBlockPool> BlockPool;

typedef void (*blockcallback)(Block&);

class BlockTree {

private:
	void _dfs(Block & root, blockcallback callback);

protected:
	CBlockPool* pool;
	Block* root;
	
public:
	BlockTree(CBlockPool& pool, Block&root);
	void DFS(blockcallback callback);
};