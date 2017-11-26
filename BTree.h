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

    // Function to Print Node Using a Depth First Search
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
    if (root->numKeys == M-1)
    {
        BTreeNode<M> *newNode = new BTreeNode<M>();
        newNode->isLeaf = false;
        newNode->numKeys = 0;
        newNode->children[0] = root;

        // Splits old root and moves key into new root
        splitChild(newNode, newNode->children[0], 0);

        // Decides which of the two children will get new key
        int i = 0;
        if (newNode->keys[0] < key)
        {
            i++;
        }

        // Inserts key into appropriate child
        insertNonFull(newNode->children[i], key);

        // Updates root
        root = newNode;
    }
    else
    {
        insertNonFull(root, key);
    }
}


// Utility function to insert a new key into a now Non-Full Node
template <int M>
void BTree<M>::insertNonFull(BTreeNode<M>* x, int key)
{
    // Creates an index i to be the index of the rightmost element
    int i = x->numKeys-1;

    if (x->isLeaf == true)
    {
        // Finds location where new key will be added and add all greater keys ahead of it
        while (i >= 0 && x->keys[i] > key)
        {
            x->keys[i+1] = x->keys[i];
            i--;
        }

        // Inserts the new key
        x->keys[i+1] = key;
        x->numKeys = x->numKeys+1;
    }
    else
    {
        while (i >= 0 && x->keys[i] > key)
        {
            i--;
        }

        // Checks if child is full
        if (x->children[i+1]->numKeys == M-1)
        {
            splitChild(x, x->children[i+1], i+1);

            if (x->keys[i+1] < key)
            {
                i++;
            }
        }
        insertNonFull(x->children[i+1], key);
    }
}


// Utility function to split child into two nodes which allows BTree to grow
template <int M>
void BTree<M>::splitChild(BTreeNode<M>* parent, BTreeNode<M> *child, int i)
{

    BTreeNode<M>* newNode = new BTreeNode<M>();
    newNode->isLeaf = child->isLeaf;
    newNode->numKeys = (M-1)/2;

    // Copy second half of keys to new node
    for (int j = 0; j<((M-1)/2); j ++){
        newNode->keys[j] = child->keys[j + M/2];
        child->keys[j+M/2] = 0;
    }

    // Copy second half of children to new node
    if (!child->isLeaf) {
        for (int j = 0; j<((M+1)/2); j ++){
            newNode->children[j] = child->children[j + M/2];
            child->children[j+M/2] = 0;
        }
        child->numKeys = M - (M-1)/2 -1;
    }

    // Create room for new child in parent->child
    for (int j = parent->numKeys + 1; j > i+1; j--){
        parent->children[j] = parent->children[j-1];
    }
    // Inserts the new child
    parent->children[i+1] = newNode;

    // Makes room for the new key for parent
    for (int j = parent->numKeys; j > i; j--){
        parent->keys[j] = parent->keys[j-1];
    }

    // Inserts the new key in parent
    parent->keys[i] = child->keys[(M/2)-1];
    child->keys[(M/2)-1] = 0;
    child->numKeys = (M/2)-1;
    parent->numKeys++;
}
// END BTree Implementation

#endif //PROJECT_4_BTREE_H
