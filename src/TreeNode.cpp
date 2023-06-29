#include "../include/TreeNode.hpp"
#include <iostream>

TreeNode::TreeNode(int t, bool isLeaf)
{
	T = t;
	IsLeaf = isLeaf;

	Keys = new int[2 * T - 1];
	Children = new TreeNode *[2 * T];

	N = 0;
}


// TreeNode::~TreeNode()
// {
// }


void TreeNode::Traverse() {
	int i;
	// Traverse child node, key, child node, key, ..., child node
	for (i = 0; i < N; i++) {
		if (IsLeaf == false)
			Children[i]->Traverse();
		std::cout << " " << Keys[i];
	}

	if (IsLeaf == false)
		Children[i]->Traverse();
}


TreeNode *TreeNode::Search(int key) {
	int i = 0;

	// Linear search over keys of curr node
	while (i < N && key > Keys[i])
		i++;

	// Return if found
	if (Keys[i] == key)
		return this;

	// Return NULL if at leaf and no result
	if (IsLeaf == true)
		return NULL;

	// Continue search in child node
	return Children[i]->Search(key);
}


void TreeNode::InsertNonFull(int key) {
	int i = N - 1;

	if (IsLeaf == true) {
		// Find position for key in node
		while (i >= 0 && Keys[i] > key) {
			Keys[i + 1] = Keys[i];
			i--;
		}

		// Insert key and update N of node
		Keys[i + 1] = key;
		N++;
	} else {
		while (i >= 0 && Keys[i] > key)
			i--;

		// If full, split
		if (Children[i + 1]->N == 2 * T - 1) {
		SplitChild(i + 1, Children[i + 1]);

		// Find right child node for key
		if (Keys[i + 1] < key)
			i++;
		}
		Children[i + 1]->InsertNonFull(key);
	}
}

void TreeNode::SplitChild(int i, TreeNode *oldNode) {
	TreeNode *newNode = new TreeNode(oldNode->T, oldNode->IsLeaf);
	newNode->N = T - 1;

	// Copy over rightmost keys of old node -> new node
	for (int j = 0; j < T - 1; j++)
		newNode->Keys[j] = oldNode->Keys[j + T];

	if (oldNode->IsLeaf == false) {
		// Copy over rightmost children of old node -> new node
		for (int j = 0; j < T; j++)
			newNode->Children[j] = oldNode->Children[j + T];
	}

	// Shifts children to right; prepping for new child insert
	oldNode->N = T - 1;
	for (int j = N; j >= i + 1; j--)
		Children[j + 1] = Children[j];

	Children[i + 1] = newNode;

	// Find position for key in node
	for (int j = N - 1; j >= i; j--)
		Keys[j + 1] = Keys[j];

	Keys[i] = oldNode->Keys[T - 1];
	N++;
}
