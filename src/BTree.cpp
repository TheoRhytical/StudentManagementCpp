// TODO: B-Tree file
// 		1 file per node
// 		storage in one folder

#include "../include/TreeNode.hpp"
#include "../include/BTree.hpp"
#include <iostream>

void BTree::Insert(int key) {
	if (Root == NULL) {	// if empty b-tree
		Root = new TreeNode(T, true);
		Root->Keys[0] = key;
		Root->N = 1;
	} else {
		if (Root->N == 2 * T - 1) {	// check if full
			TreeNode *parentNode = new TreeNode(T, false);	// New Parent Node

			parentNode->Children[0] = Root;

			parentNode->SplitChild(0, Root);

			int i = 0;
			if (parentNode->Keys[0] < key)
				i++;
			parentNode->Children[i]->InsertNonFull(key);

			Root = parentNode;
		} else
		Root->InsertNonFull(key);
	}
}

