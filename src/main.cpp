#include <iostream>
#include "../include/BTree.hpp"
#include "../include/TreeNode.hpp"

int main()
{
	BTree t(3);
	t.Insert(8);
	t.Insert(9);
	t.Insert(10);
	t.Insert(11);
	t.Insert(15);
	t.Insert(16);
	t.Insert(17);
	t.Insert(18);
	t.Insert(20);
	t.Insert(23);

	std::cout << "The B-tree is: ";
	t.Traverse();

	int k = 10;
	(t.Search(k) != NULL) ? std::cout << std::endl
					<< k << " is found"
				: std::cout << std::endl
					<< k << " is not Found";

	k = 2;
	(t.Search(k) != NULL) ? std::cout << std::endl
					<< k << " is found"
				: std::cout << std::endl
					<< k << " is not Found\n";

	// std::cout << "Welcome to students management system" << std::endl;
	return 0;
}