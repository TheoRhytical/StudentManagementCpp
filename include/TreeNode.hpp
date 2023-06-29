#ifndef TREE_NODE_HPP
#define TREE_NODE_HPP


class TreeNode
{
private:
	bool IsLeaf;
	int *Keys,
		T, // t = degree
		N; // n = order; curr # of keys
	TreeNode **Children;

public:
	TreeNode(int temp, bool bool_leaf);
	// ~TreeNode();

	void InsertNonFull(int k);
	void SplitChild(int i, TreeNode *y);
	void Traverse();

	TreeNode *Search(int k);

	friend class BTree;
};


#endif