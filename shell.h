#define _CRT_SECURE_NO_WARNINGS
#pragma once
#include "blocktree.h"
namespace shell {
	void init();
	
	void destroy();
	int start_idle();

	namespace crafting {
		void init(BlockTree tree);
	}
}