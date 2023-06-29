// TODO: B-Tree file
// 		1 file per node
// 		storage in one folder

#include "../include/TreeNode.hpp"
#include "../include/BTree.hpp"
#include "../include/Key.hpp"
#include <iostream>
#include <string>

void BTree::Insert(std::string name, std::string content) {
	Key key = Key(name, content);
	int currFile = 0;
	if (Root == NULL) {	// if empty b-tree
		Root = new TreeNode(T, true, this);
		key.Filename = "0_0";
		Root->Keys[0] = key;
		Root->N = 1;
		Root->InsertToFile(key, RowSize);
	} else {
		if (Root->N == 2 * T - 1) {	// check if full
			TreeNode *parentNode = new TreeNode(T, false, this);	// New Parent Node

			parentNode->Children[0] = Root;

			parentNode->SplitChild(0, Root, this);

			int i = 0;
			if (parentNode->Keys[0].KeyNum < key.KeyNum)
				i++;
			parentNode->Children[i]->InsertNonFull(key, RowSize, this);

			Root = parentNode;
		} else
		Root->InsertNonFull(key, RowSize, this);
	}
}

