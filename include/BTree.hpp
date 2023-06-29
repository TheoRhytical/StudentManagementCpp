#ifndef BTREE_HPP
#define BTREE_HPP

#include "../include/TreeNode.hpp"
#include <iostream>

class BTree {
private:
	TreeNode *Root;
	int T; // degree

public:
	BTree(int temp) {
		Root = NULL;
		T = temp;
	}

	// To optimize (prevent recursion)
	void Traverse() {
		if (Root != NULL)
			Root->Traverse();
	}

	// To optimize (prevent recursion)
	TreeNode *Search(int k) {
		return (Root == NULL) ? NULL : Root->Search(k);
	}

	void Insert(int k);
};

#endif