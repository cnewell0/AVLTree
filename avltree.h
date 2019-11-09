// avltree.h
#include "binarysearchtree.h"
#include "fstream"
class AVLTree: public BinarySearchTree
{
	public:
	
	AVLTree();
	
	~AVLTree();
	
	void addNewEntry(const Item& newItem) throw (Exception);
	
	void prettyDisplay(ofstream& outputFile);
	
};