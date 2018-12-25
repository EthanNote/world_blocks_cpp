#include "blocktree.h"



CBlockTree::CBlockTree()
{
}


CBlockTree::~CBlockTree()
{
}

void CBlockTree::_dfs(int buffer_index, blockcallback callback)
{
	if (buffer_index < 0) {
		return;
	}
	callback(&this->pool->blocks[buffer_index]);
	for (int i = 0; i < 8; i++) {
		_dfs(this->pool->blocks[buffer_index].children[i], callback);
	}
}

void CBlockTree::DFS(blockcallback callback)
{
	this->_dfs(0, callback);
}

void CBlockTree::Insert(Block & block)
{
	if (block.level >= this->pool->blocks[0].level) {
		return;
	}

	//Block* parent = this->root;
	int parent_index = 0;
	if (!this->pool->blocks[0].IsContain(&block)) {
		return;
	}

	//non leaf node
	while (this->pool->blocks[parent_index].level > block.level + 1)
	{
		Block & parent = this->pool->blocks[parent_index];
		//if (parent->IsContain(&block)) {
		int child_slot_index = parent.GetChildIndex(&block);
		int child_index = parent.children[child_slot_index];
		if (child_index < 0) {
			int insert_index = pool->blocks.size();
			parent.children[child_slot_index] = insert_index;
			child_index = insert_index;

			Block non_leaf_node;
			int mask = -1 << (parent.level);

			non_leaf_node.x = block.x & mask;
			non_leaf_node.y = block.y & mask;
			non_leaf_node.z = block.z & mask;
			non_leaf_node.level = parent.level - 1;
			non_leaf_node.type_id = 0;

			non_leaf_node.parent = parent_index;

			non_leaf_node.children[0] = -1;
			non_leaf_node.children[1] = -1;
			non_leaf_node.children[2] = -1;
			non_leaf_node.children[3] = -1;
			non_leaf_node.children[4] = -1;
			non_leaf_node.children[5] = -1;
			non_leaf_node.children[6] = -1;
			non_leaf_node.children[7] = -1;

			non_leaf_node.color = -1;

			//push back policy
			pool->LockWrite();
			pool->blocks.push_back(non_leaf_node);
			pool->UnlockWrite();

		}
		//parent = &pool->blocks[parent->children[child_slot_index]];
		parent_index = child_index;
		//}
	}

	//leaf node
	Block & parent = this->pool->blocks[parent_index];
	int i = parent.GetChildIndex(&block);

	if (parent.children[i] < 0) { // insert
		int insert_index = pool->blocks.size();
		parent.children[i] = insert_index;
		block.parent = parent_index;
		pool->blocks.push_back(block);
	}
	else { // replace

	}


}

void CBlockTree::Remove(Block & block, bool skip_verify)
{
	//replace target block with tail block and pop_back
	int buffer_size = this->pool->blocks.size();
	int buffer_index = (&block - &this->pool->blocks[0]) / sizeof(Block);
	int parent_buffer_index = block.parent;
	if (skip_verify || buffer_index >= 0 && buffer_index < buffer_size) { //verify
		if (skip_verify || parent_buffer_index >= 0 && parent_buffer_index < buffer_size) {
			Block & parent_block = this->pool->blocks[parent_buffer_index];
			int child_index = parent_block.GetChildIndex(&block);
			parent_block.children[child_index] = -1;
		}
		this->pool->blocks[buffer_index] = this->pool->blocks[buffer_size - 1];
		this->pool->blocks.pop_back();
	}
}

BlockTree CBlockTree::Create(BlockPool pool, int root_level)
{
	auto tree = new CBlockTree;
	tree->pool = pool;
	Block root{ 0,0,0,root_level,-1,0,0,{-1,-1,-1,-1,-1,-1,-1,-1}, -1 };

	pool->blocks.clear();
	pool->blocks.push_back(root);
	//tree->root = &pool->blocks[0];

	return BlockTree(tree);
}
