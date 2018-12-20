#include "blocktree.h"



CBlockTree::CBlockTree()
{
}


CBlockTree::~CBlockTree()
{
}

void CBlockTree::_dfs(Block* root, blockcallback callback)
{
	callback(root);
	for (int i = 0; i < 8; i++) {
		if (root->children[i] >= 0) {
			_dfs(&this->pool->blocks[root->children[i]], callback);
		}
	}
}

void CBlockTree::DFS(blockcallback callback)
{
	this->_dfs(this->root, callback);
}

void CBlockTree::Insert(Block & block)
{
	if (block.level >= this->root->level) {
		return;
	}

	Block* parent = this->root;
	if (!parent->IsContain(&block)) {
		return;
	}

	//non leaf node
	while (parent->level > block.level + 1)
	{
		//if (parent->IsContain(&block)) {
		int i = parent->GetChildIndex(&block);
		if (parent->children[i] < 0) {
			Block non_leaf_node;
			int mask = -1 << (parent->level);

			non_leaf_node.x = block.x & mask;
			non_leaf_node.y = block.y & mask;
			non_leaf_node.z = block.z & mask;
			non_leaf_node.level = parent->level - 1;
			non_leaf_node.type_id = 0;

			//push back policy
			int insert_index = pool->blocks.size();
			pool->blocks.push_back(non_leaf_node);
			parent->children[i] = insert_index;
		}
		parent = &pool->blocks[parent->children[i]];
		//}
	}

	//leaf node
	int i = parent->GetChildIndex(&block);

	if (parent->children[i] < 0) { // insert
		int insert_index = pool->blocks.size();
		pool->blocks.push_back(block);
		parent->children[i] = insert_index;
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
