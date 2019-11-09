// avltree.cpp

#include <iostream>
#include "avltree.h"
using namespace std;

const int MAXHEIGHT = 15;

struct SearchPath
{
     TreeNode* path[MAXHEIGHT];
     int size;
     int pivotIndex;
     SearchPath();   
};

//this creates a search path that follows the entries added with an array and TreeNode pointers
//		and sets the size to 0 and the pivot index to 0
//pre: the search path exists but the size and pivot arent set to anything 
//post: this sets the size and the searchPath to 0 and sets the pointers in the 
//		array to null pointers
//usage: SearchPath()
SearchPath::SearchPath()
{
    size = 0;
    pivotIndex = 0;
    for(int i = 0; i < MAXHEIGHT; i++) { // you finish
	}
}

//helpers

// Helper function that adds new entry to the search tree
//		and creates a search path from root to new entry
// Pre: treeptr points an AVL tree, search is empty SearchPath
//		with size zero and newItem exists. 
// Post: newItem is added to the tree and 
//		search is a search path from root to new entry
//		with appropriate size
//		If newItem is already in the search tree or
//		there was no memory for new TreeNode exception
//		is thrown.
// Usage: addNewEntryAndMakeSearchPath(root, newItem, search);
void addNewEntryAndMakeSearchPath(TreeNode*& treeptr, const Item& newItem, SearchPath& search) throw (Exception)
{
	if (treeptr != nullptr)
	{
		search.path[search.size] = treeptr;
		search.size += 1;
		
		if (newItem == treeptr -> item)
		{
			throw Exception("That abbreviation is already in the dictionary.");
		} else if ( newItem < treeptr -> item) {
			addNewEntryAndMakeSearchPath(treeptr -> leftChild, newItem, search);
		} else {
			addNewEntryAndMakeSearchPath(treeptr -> rightChild, newItem, search);
		}
	} else {
		treeptr = new (nothrow) TreeNode(newItem, nullptr, nullptr);
		search.path[search.size] = treeptr;
		search.size += 1;
	} 
   
   if ( treeptr == nullptr )
	   throw Exception("There was not enough memory to add a new abbreviation.");
}

// Detects whether the AVLTree has a pivotIndex and assigns pivotIndex to 
//		the location of the node containing the pivotIndex
// Pre: SearchPath has been declared, AVLTree exists
// Post: If there is a pivotIndex, pivotIndex index is assigned in search 
// 			to the location of the node containing the pivotIndex and we return false, 
//			else return true
// usage: bool pivotIndex = hasNopivotIndex(search);
bool hasNopivotIndex(SearchPath& search)
{
   int index = search.size - 1;
   while(index >= 0)
   {
      if(search.path[index] -> balance != 0)
	  {
		 search.pivotIndex = index;
	     return false;
	  }
	  index--;		
   }
   return true;
}

// Changes the balances along the search path
// Pre search path is on an existing AVL tree, but the balances are not correct
// Post each node's balance is correct and fits the definition of an AVL tree
// usage: fixBalances(search, start);
void fixBalances(const SearchPath& search, int start)
{
	while (start < search.size - 1)
	{
		if (search.path[start] -> leftChild == search.path[start + 1])
			search.path[start] -> balance--;
		else
			search.path[start] -> balance++;
		start++;
	}
}


// tells whether the new item has been added to the short or tall side
// pre: SearchPath object exists
// post: returns true if added to the short side, false if added to the tall side
// usage: if(isAddedToShortSide(search))
bool isAddedToShortSide(const SearchPath& search)
{
	return ((search.path[search.pivotIndex] -> balance == -1 && search.path[search.pivotIndex + 1] == search.path[search.pivotIndex] -> rightChild)
	      ||(search.path[search.pivotIndex] -> balance ==  1 && search.path[search.pivotIndex + 1] == search.path[search.pivotIndex] -> leftChild));
}

//Tests whether or not the AVL tree needs a right rotation.
//Pre: Search exists/ is initialized
//Post: Returns true if a right rotation is needed in the AVL tree.
//Usage: isSingleRotateRight(mySearchPath);
bool isSingleRotateRight(const SearchPath& search)
{
	return ((search.path[search.pivotIndex + 1] == search.path[search.pivotIndex] -> leftChild) &&
				(search.path[search.pivotIndex + 2] == search.path[search.pivotIndex + 1] -> leftChild));
}

//	This function starts at the pivotIndex and returns true if the search.path is the right right case
//  	and decides whether it needs a single left rotation
//	Pre: the AVL tree is populated with items and pivotIndex index is assigned in the tree
//	Post: the function returns true if the search path goes right twice in a row
//		  and returns false otherwise
//	usage: isSingleRotateLeft(search);
bool isSingleRotateLeft(const SearchPath& search)
{
	return ((search.path[search.pivotIndex]->rightChild == search.path[search.pivotIndex + 1]) && 
				(search.path[search.pivotIndex]->rightChild->rightChild == search.path[search.pivotIndex + 2]));
}

// goes to the pivotIndex and checks to see if the avl tree goes left then right 
// pre: SearchPath exists
// post: returns true if search path goes left then right and false otherwise
// usage: isDoubleRotateLeftRight(SearchPath& mySearchPath);
bool isDoubleRotateLeftRight(const SearchPath& search)
{
	if (search.path[search.pivotIndex] -> leftChild == search.path[search.pivotIndex + 1])
	{
		if(search.path[search.pivotIndex + 1] -> rightChild == search.path[search.pivotIndex + 2])
			return true;
	}
	return false;
}

//need to print balance next to abbrev

//prints the tree to look like a computer science tree
//pre: treep points to the root of a tree. level >= 0
//post: outputs the tree as in the example below
//
//  					  bar
//                  foo
//                        geeU
//  root ->  queue
//                        stack
//                  list
//                        array
//nodes at the same level are indented the same.
//Viewer must rotate head 90 degrees in order to look like a cs tree
//usage: writePretty(root,0);
void writePrettyAVL (TreeNode* treep, int level, ofstream& outputFile)
{	
	if (treep != nullptr)
	{
		writePrettyAVL(treep -> rightChild, level += 1, outputFile);
		if (treep -> rightChild != nullptr)
		{
			for (int j = 0;j <= level;j++)
				outputFile << "        ";
			outputFile << "/" << endl;
		}
		if (level == 1)
			outputFile << "root ->" ;
		else	
		{			
			for (int i = 0; i < level; i++)
				outputFile << "        " ;
		}
		Key text = treep -> item;
		outputFile<< "  " << text << ": " << treep -> balance << endl;
		 if (treep -> leftChild != nullptr)
        {
            for (int i = 0; i <= level; i++)
                outputFile << "        ";
            outputFile << "\\" << endl;
			writePrettyAVL(treep -> leftChild, level, outputFile);
		}
	}		
}

//this rotates the tree to the left when isSingleRotateLeft returns true
//pre: the AVLTree has items in it, isSingleRotateLeft returns true
//post: an item has been added to the AVLtree with a search path and 
//		the tree is rotated to the left, keeping it balanced
//usage: doSingleRotateLeft(search, root); 
void doSingleRotateLeft(SearchPath& search, TreeNode*& root)
{
	search.path[search.pivotIndex] -> rightChild = search.path[search.pivotIndex + 1] -> leftChild;
	search.path[search.pivotIndex + 1 ] -> leftChild = search.path[search.pivotIndex];
	fixBalances(search, search.pivotIndex);
	search.path[search.pivotIndex + 1 ] -> balance--;
	search.path[search.pivotIndex ] -> balance -= 2;
	
	if (search.pivotIndex != 0)
		if (search.path[search.pivotIndex - 1] -> leftChild == search.path[search.pivotIndex])
		{
			search.path[search.pivotIndex - 1] -> leftChild = search.path[search.pivotIndex + 1];
		}
		else
		{
			search.path[search.pivotIndex - 1] -> rightChild = search.path[search.pivotIndex + 1];
		}
	else
		root = search.path[search.pivotIndex + 1];
}
//this rotates the tree to the right when isSingleRotateRight returns true
//pre: the AVLTree has items in it, isSingleRotateRight returns true
//post: an item has been added to the AVLtree with a search path and 
//		the tree is rotated to the right, keeping it balanced
//usage: doSingleRotateRight(search, root); 
void doSingleRotateRight(SearchPath& search, TreeNode*& root)
{
	search.path[search.pivotIndex] -> leftChild = search.path[search.pivotIndex + 1] -> rightChild;
	search.path[search.pivotIndex + 1 ] -> rightChild = search.path[search.pivotIndex];
	fixBalances(search, search.pivotIndex);
	search.path[search.pivotIndex + 1 ] -> balance++;
	if (search.pivotIndex != 0)
		if (search.path[search.pivotIndex - 1] -> leftChild == search.path[search.pivotIndex])
			search.path[search.pivotIndex - 1] -> leftChild = search.path[search.pivotIndex + 1];
		else
			search.path[search.pivotIndex - 1] -> rightChild = search.path[search.pivotIndex + 1];
	else
		root = search.path[search.pivotIndex + 1];
}

//this rotates the tree to the right when isDoubleRotateLeftRight returns true
//pre: the AVLTree has items in it, isDoubleRotateLeftRight returns true
//post: an item has been added to the AVLtree with a search path and 
//		the tree is rotated to the right, keeping it balanced
//usage: doDoubleRotateRightLeft(search, root);
void doDoubleRotateLeftRight(SearchPath& search, TreeNode*& root)
{	
	search.path[search.pivotIndex + 1] -> rightChild = search.path[search.pivotIndex + 2] -> leftChild;
	search.path[search.pivotIndex + 2 ] -> leftChild = search.path[search.pivotIndex + 1];
	search.path[search.pivotIndex] -> leftChild = search.path[search.pivotIndex + 2] -> rightChild;
	search.path[search.pivotIndex + 2] -> rightChild = search.path[search.pivotIndex];
	fixBalances(search, search.pivotIndex);
	search.path[search.pivotIndex + 1 ] -> balance--;
	if (search.pivotIndex != 0)
		if (search.path[search.pivotIndex - 1] -> rightChild == search.path[search.pivotIndex])
		{
			search.path[search.pivotIndex - 1] -> rightChild = search.path[search.pivotIndex + 2];
		}
		else
		{
			search.path[search.pivotIndex - 1] -> leftChild = search.path[search.pivotIndex + 2];
		}
	else
		root = search.path[search.pivotIndex + 2];
}

//this rotates the tree to the left when isDoubleRotateLeftRight returns false
//pre: the AVLTree has items in it, isDoubleRotateLeftRight returns false
//post: an item has been added to the AVLtree with a search path and 
//		the tree is rotated to the left, keeping it balanced
//usage: doDoubleRotateRightLeft(search, root); 
void doDoubleRotateRightLeft(SearchPath& search, TreeNode*& root)
{	
	if((search.path[search.pivotIndex] -> rightChild == search.path[search.pivotIndex] + 1) && 
	(search.path[search.pivotIndex + 1] -> leftChild == search.path[search.pivotIndex + 2])){
		search.path[search.pivotIndex + 2] -> balance--;
		search.path[search.pivotIndex] -> balance--;
	}
	search.path[search.pivotIndex + 1] -> leftChild = search.path[search.pivotIndex + 2] -> rightChild;
	search.path[search.pivotIndex + 2 ] -> rightChild = search.path[search.pivotIndex + 1];
	search.path[search.pivotIndex] -> rightChild = search.path[search.pivotIndex + 2] -> leftChild;
	search.path[search.pivotIndex + 2] -> leftChild = search.path[search.pivotIndex];
	fixBalances(search, search.pivotIndex);
	search.path[search.pivotIndex + 1 ] -> balance--;
	search.path[search.pivotIndex ] -> balance -= 2;
	//if((search.path[search.pivotIndex] -> rightChild == search.path[search.pivotIndex] + 1) && 
	//(search.path[search.pivotIndex + 1] -> leftChild == search.path[search.pivotIndex + 2])){
	//	search.path[search.pivotIndex + 2] -> balance--;
	//	search.path[search.pivotIndex] -> balance--;
	//}
	if (search.pivotIndex != 0)
		if (search.path[search.pivotIndex - 1] -> rightChild == search.path[search.pivotIndex])
		{
			search.path[search.pivotIndex - 1] -> rightChild = search.path[search.pivotIndex + 2];
		}
		else
		{
			search.path[search.pivotIndex - 1] -> leftChild = search.path[search.pivotIndex + 2];
		}
	else
		root = search.path[search.pivotIndex + 2];
}
AVLTree::AVLTree()
{
}

AVLTree::~AVLTree()
{
}


//depending on the case, addNewEntry adds a newItem keeping the tree balanced
//pre: the tree exists but no entries are added to the AVLTree
//post: an item is added into the tree and the tree has stays balanced
//usage: addNewEntry(newItem);
void AVLTree::addNewEntry(const Item& newItem) throw (Exception)
{
	SearchPath search;
    addNewEntryAndMakeSearchPath(root, newItem, search);
    if(hasNopivotIndex(search))
      fixBalances(search, 0);
    else if (isAddedToShortSide(search))
      fixBalances(search, search.pivotIndex);
    else if (isSingleRotateLeft(search))
		doSingleRotateLeft(search, root);
    else if (isSingleRotateRight(search))
		doSingleRotateRight(search, root);
    else if (isDoubleRotateLeftRight(search))
		doDoubleRotateLeftRight(search, root);
    else
		doDoubleRotateRightLeft(search, root);

}

//prints the tree to look like a computer science tree
//post: outputs the tree as in the example below
//
//                        bar
//                  foo
//                        geeU
//  root ->  queue
//                        stack
//                  list
//                        array
//nodes at the same level are indented the same.
//Viewer must rotate head 90 degrees in order to look like a cs tree
//usage: myAVLTree.prettyDisplay();
void AVLTree::prettyDisplay(ofstream& outputFile)
{
	writePrettyAVL(root, 0, outputFile);
}

