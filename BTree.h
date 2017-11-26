// HYBRID OF CLRS AND G4G 11:40PM 11/24

/*
 * BTree.h
 *
 * COSC 160 Fall 2017
 * Project #4
 *
 * Due on: NOV 27, 2017
 * Author: Carl Kyrillos
 *
 *
 * In accordance with the class policies and Georgetown's
 * Honor Code, I certify that, with the exception of the
 * class resources and those items noted below, I have neither
 * given nor received any assistance on this project.
 *
 * References not otherwise commented within the program source code.
 * Note that you should not mention any help from the TAs, the professor,
 * or any code taken from the class textbooks.
 */

#ifndef PROJECT_4_BTREE_H
#define PROJECT_4_BTREE_H


#include <iostream>
#include <sstream>
using namespace std;


// BEGIN BTreeNode Declaration
template <int M>
class BTreeNode
{
public:
    // Public Members
    int numKeys;
    int keys[M-1];
    BTreeNode* children[M];
    bool isLeaf;

    // Constructor and Destructor
    void printNode() const;
};
// END BTreeNode Declaration


// BEGIN BTreeNode Implementation
// Depth First Search Print
template <int M>
void BTreeNode<M>::printNode() const
{
    cout << "[";

    for (int i = 0; i < this->numKeys; i++)
    {
        cout << this->keys[i];
        if (i != (this->numKeys-1))
        {
            cout << ",";
        }
    }
    if (!this->isLeaf)
    {
        for (int i = 0; i <= this->numKeys; i++)
        {
            children[i]->printNode();
        }
    }
    cout << "]";
}
// END BTreeNode Implementation


// BEGIN BTree Declaration
template<int M>
class BTree
{
public:
    // Public Members
    BTreeNode<M>* root;

    // Constructors and Destructor
    BTree();
    ~BTree();

    // Public Operations
    void insert(int key);
    void remove(int);
    void printBTree() const;

private:
    // Private Operations
    void splitChild(BTreeNode<M>*, BTreeNode<M>*, int);
    void insertNonFull(BTreeNode<M>*, int);

};
// END BTree Declaration


// BEGIN BTree Implementation
// Default Constructor for BTree
template <int M>
BTree<M>::BTree()
{
    BTreeNode<M> * root = new BTreeNode<M>();
    root->isLeaf = true;
    this->root = root;
}


// Destructor for BTree
template <int M>
BTree<M>::~BTree()
{
    delete this->root;
}

// Prints BTree by use of printNode()
template <int M>
void BTree<M>::printBTree() const
{
    root->printNode();
    cout << endl;
}

// Insertion Operator for BTree
template <int M>
void BTree<M>::insert(int key)
{
    // If root is full, then tree grows in height
    if (root->numKeys == M-1)
    {
        // Allocate memory for new root
        BTreeNode<M> *newNode = new BTreeNode<M>();
        newNode->isLeaf = false;
        newNode->numKeys = 0;
        newNode->children[0] = root;

        // Split the old root and move 1 key to the new root
        splitChild(newNode, newNode->children[0], 0);

        // Decide which of the two children is going to have new key
        int i = 0;
        if (newNode->keys[0] < key)
        {
            i++;
        }
        insertNonFull(newNode->children[i], key);

        // Change root
        root = newNode;
    }
    else  // If root is not full, call insertNonFull for root
    {
        insertNonFull(root, key);
    }
}


// A utility function to insert a new key in this node
// The assumption is, the node must be non-full when this
// function is called
template <int M>
void BTree<M>::insertNonFull(BTreeNode<M>* x, int key)
{
    // Initialize index as index of rightmost element
    int i = x->numKeys-1;

    // If this is a leaf node
    if (x->isLeaf == true)
    {
        // The following loop does two things
        // a) Finds the location of new key to be inserted
        // b) Moves all greater keys to one place ahead
        while (i >= 0 && x->keys[i] > key)
        {
            x->keys[i+1] = x->keys[i];
            i--;
        }

        // Insert the new key at found location
        x->keys[i+1] = key;
        x->numKeys = x->numKeys+1;
    }
    else // If this node is not leaf
    {
        // Find the child which is going to have the new key
        while (i >= 0 && x->keys[i] > key)
        {
            i--;
        }

        // See if the found child is full
        if (x->children[i+1]->numKeys == M-1)
        {
            // If the child is full, then split it
            splitChild(x, x->children[i+1], i+1);

            // After split, the middle key of C[i] goes up and
            // C[i] is splitted into two.  See which of the two
            // is going to have the new key
            if (x->keys[i+1] < key)
            {
                i++;
            }
        }
        insertNonFull(x->children[i+1], key);
    }
}

// A utility function
template <int M>
void BTree<M>::splitChild(BTreeNode<M>* parent, BTreeNode<M> *child, int i)
{

    BTreeNode<M>* newNode = new BTreeNode<M>();
    newNode->isLeaf = child->isLeaf;
    newNode->numKeys = (M-1)/2;

    // copy right half of keys to newNode:
    for (int j = 0; j<((M-1)/2); j ++){
        newNode->keys[j] = child->keys[j + M/2];
        child->keys[j+M/2] = 0;
    }

    // copy right half of children to newNode:
    if (!child->isLeaf) {
        for (int j = 0; j<((M+1)/2); j ++){
            newNode->children[j] = child->children[j + M/2];
            child->children[j+M/2] = 0;
        }
        child->numKeys = M - (M-1)/2 -1;
    }

    // make room for new child in parent.children:
    for (int j = parent->numKeys + 1; j > i+1; j--){
        parent->children[j] = parent->children[j-1];
    }
    // insert new child:
    parent->children[i+1] = newNode;

    // make room for new key in parent:
    for (int j = parent->numKeys; j > i; j--){
        parent->keys[j] = parent->keys[j-1];
    }
    // insert new key in parent:
    parent->keys[i] = child->keys[(M/2)-1];
    child->keys[(M/2)-1] = 0;
    child->numKeys = (M/2)-1;
    parent->numKeys++;
}

// END BTree Implementation
#endif //PROJECT_4_BTREE_H
