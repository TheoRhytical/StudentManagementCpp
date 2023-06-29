#ifndef TREE_NODE_HPP
#define TREE_NODE_HPP

#include <string>
#include "./Key.hpp"
#include "./BTree.hpp"


class TreeNode
{
private:
	bool IsLeaf;
	Key *Keys;
	int T, // t = degree
		N, // n = order; curr # of keys
		Filename;

	TreeNode **Children;

public:
	TreeNode(int temp, bool bool_leaf, BTree* parentTree);
	// ~TreeNode();

	void InsertNonFull(Key key, int rowSize, BTree* tree);
	void SplitChild(int i, TreeNode *y, BTree* tree);
	void Traverse();

	TreeNode *Search(Key k);
	// void TreeNode::InsertToFile(std::string filename, std::string content);
	void TreeNode::InsertToFile(Key key, int rowSize);

	friend class BTree;
};


#endif