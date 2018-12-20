#include<memory>
#include<vector>
#include "block.h"


Block & CBlockPool::Alloc()
{
	this->LockWrite();
	this->blocks.push_back({ 0,0,0,-1,0,0,0, {-1,-1,-1,-1,-1,-1,-1,-1} });
	auto it = this->blocks.end();
	it--;

	this->UnlockWrite();
	return (*it._Ptr);

}

Block & CBlockPool::Alloc(Block & block)
{
	this->LockWrite();
	this->blocks.push_back(block);
	auto it = this->blocks.end();
	it--;

	this->UnlockWrite();
	return (*it._Ptr);
}

void CBlockPool::LockRead()
{
	this->lock.lock_shared();
}

void CBlockPool::UnlockRead()
{
	this->lock.unlock_shared();
}

void CBlockPool::LockWrite()
{
	this->lock.lock();
}

void CBlockPool::UnlockWrite()
{
	this->lock.unlock();
}

//void CBlockPool::Unlock()
//{
//	this->lock.unlock();
//}


std::shared_ptr<CBlockPool> CBlockPool::Create()
{
	return std::shared_ptr<CBlockPool>(new CBlockPool);
}

//void BlockTree::_dfs(Block & root, blockcallback callback)
//{
//	callback(root);
//	for (int i = 0; i < 8; i++) {
//		if (root.children[i] >= 0) {
//			_dfs(this->pool->blocks[root.children[i]], callback);
//		}
//	}
//}
//
//BlockTree::BlockTree(CBlockPool & pool, Block & root)
//{
//	this->pool = &pool;
//	this->root = &root;
//}
//
//
//
//
//
//void BlockTree::DFS(blockcallback)
//{
//}

bool Block::IsContain(Block* another)
{
	//int inc = 1 << (this->level-1);
	int mask = ~(-1 << (this->level));
	return (
		another->x & mask == this->x &&
		another->y & mask == this->y &&
		another->z & mask == this->z
		);
	
}

int Block::GetChildIndex(Block * another)
{
	int i = 0;
	if (another->x > this->x) {
		i |= 1;
	}
	if (another->y > this->y) {
		i |= 2;
	}
	if (another->z > this->z) {
		i |= 4;
	}
	return i;
}
