#include <iostream>
#include <vector>
#include <string>
#include <sstream>

class BinaryTree
{
public:

	//ATRIBUTOS
	struct node
	{
		int info;
		node *right;
		node *left;
	};

	node *root;

	//METODES
	BinaryTree();
	~BinaryTree();
	void PreOrder();
	void InOrder();
	void PostOrder();
	int GetNumbersNode();
	int GetNumbersNodeI();
	void PreOrderI();
	int Height();

};

BinaryTree::BinaryTree()
{
	root = NULL;
}

BinaryTree::~BinaryTree()
{
}

void BinaryTree::PreOrder()
{
}

void BinaryTree::InOrder()
{
}

void BinaryTree::PostOrder()
{
}

int BinaryTree::GetNumbersNode()
{
	return 0;
}

int BinaryTree::GetNumbersNodeI()
{
	return 0;
}

void BinaryTree::PreOrderI()
{
}

int BinaryTree::Height()
{
	return 0;
}


int main()
{
	return 0;
}