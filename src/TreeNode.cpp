#include "../include/TreeNode.hpp"
#include "../include/Key.hpp"
#include <iostream>
#include <fstream>
#include <sstream>
#include <vector>

TreeNode::TreeNode(int t, bool isLeaf, BTree* parentTree)
{
	T = t;
	IsLeaf = isLeaf;

	Keys[2 * T - 1];
	Children = new TreeNode *[2 * T];

	Filename = ++parentTree->Filenum;

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
		std::cout << " " << Keys[i].KeyNum;
	}

	if (IsLeaf == false)
		Children[i]->Traverse();
}


// Search by key
TreeNode *TreeNode::Search(Key key) {
	int i = 0;

	// Linear search over keys of curr node
	while (i < N && key.KeyNum > Keys[i].KeyNum)
		i++;

	// Return if found
	if (Keys[i].KeyNum == key.KeyNum)
		return this;

	// Return NULL if at leaf and no result
	if (IsLeaf == true)
		return NULL;

	// Continue search in child node
	return Children[i]->Search(key);
}


void TreeNode::InsertNonFull(Key key, int rowSize, BTree* tree) {
	int i = N - 1;

	if (IsLeaf == true) {
		// Find position for key in node
		while (i >= 0 && Keys[i].KeyNum > key.KeyNum) {
			Keys[i + 1] = Keys[i];
			i--;
		}

		// Insert key and update N of node
		Keys[i + 1] = key;
		N++;
		int row = i + 1;
		key.Filename = std::to_string(Filename) + std::to_string(row);
		InsertToFile(key, rowSize);
	} else {
		while (i >= 0 && Keys[i].KeyNum > key.KeyNum)
			i--;

		// If full, split
		if (Children[i + 1]->N == 2 * T - 1) {
		SplitChild(i + 1, Children[i + 1], tree);

		// Find right child node for key
		if (Keys[i + 1].KeyNum < key.KeyNum)
			i++;
		}
		Children[i + 1]->InsertNonFull(key, rowSize, tree);
	}
}

void TreeNode::SplitChild(int i, TreeNode *oldNode, BTree* tree) {
	TreeNode *newNode = new TreeNode(oldNode->T, oldNode->IsLeaf, tree);
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

// function src: https://stackoverflow.com/a/46931770/9902326
std::vector<std::string> splitString (const std::string &s, char delim) {
    std::vector<std::string> result;
    std::stringstream ss (s);
    std::string item;

    while (getline (ss, item, delim)) {
        result.push_back (item);
    }

    return result;
}

void updateRow(const std::string& filename, int rowNumber, const std::string& newData) {
    std::ifstream inputFile(filename);
    std::vector<std::string> lines;
    std::string line;

    // Read the file line by line and store the lines in a vector
    while (std::getline(inputFile, line)) {
        lines.push_back(line);
    }

    inputFile.close();

    // Check if the row number is within the valid range
    if (rowNumber >= 0 && rowNumber < lines.size()) {
        lines[rowNumber] = newData;
    } else {
        std::cerr << "Invalid row number." << std::endl;
        return;
    }

    // Write the updated lines to a new file
    std::ofstream outputFile(filename);
    for (const auto& updatedLine : lines) {
        outputFile << updatedLine << std::endl;
    }

    outputFile.close();
}




void TreeNode::InsertToFile(Key key, int rowSize) {
	std::vector<std::string> keyArr = splitString(key.Filename, '_');
	std::string filename = keyArr[0];
	int row = std::stoi(keyArr[1]);

	updateRow(filename, row, key.Content);
}