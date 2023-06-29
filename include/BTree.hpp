#ifndef BTREE_HPP
#define BTREE_HPP

#include "../include/TreeNode.hpp"
#include <iostream>
#include <string>

class BTree {
private:
	TreeNode *Root;
	int T, // degree
		RowSize;

public:
	int Filenum;

	BTree(int temp, int rowSize) : Root(NULL), T(temp), RowSize(rowSize) {}

	// To optimize (prevent recursion)
	void Traverse() {
		if (Root != NULL)
			Root->Traverse();
	}

	// To optimize (prevent recursion)
	TreeNode *Search(Key k) {
		return (Root == NULL) ? NULL : Root->Search(k);
	}

	void Insert(std::string name, std::string content);
};

#endif