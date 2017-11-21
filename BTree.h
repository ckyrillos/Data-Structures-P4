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
    BTreeNode();
    ~BTreeNode();
    void printNode() const;
};
// END BTreeNode Declaration


// BEGIN BTreeNode Implementation
// Default Constructor for BTreeNode
template <int M>
BTreeNode<M>::BTreeNode()
{
    numKeys = 0;
}


// Destructor for BTreeNode
template <int M>
BTreeNode<M>::~BTreeNode()
{
    numKeys = 0;
}


// Depth First Search Print
//TODO: is this right?!?
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
    if (this->isLeaf == false)
    {
        for (int i = 0; i <= this->numKeys; i++)
        {
            this->children[i]->printNode();
        }
    }
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
    void remove(BTreeNode<M>* Node, int key);
    void printBTree() const;

private:
    // Private Operations
    void splitChild(BTreeNode<M>* parent, BTreeNode<M>* child, int index);
    void insertNonFull(BTreeNode<M>* nonFullNode, int key);

};
// END BTree Declaration


// BEGIN BTree Implementation
// Default Constructor for BTree
template <int M>
BTree<M>::BTree()
{
    BTreeNode<M>* root = new BTreeNode<M>();
    root->isLeaf = true;
    this->root = root;
}


// Destructor for BTree
template <int M>
BTree<M>::~BTree()
{
    delete this->root;
}


// Insertion Operator for BTree
template <int M>
void BTree<M>::insert(int key)
{
    BTreeNode<M>* root = this->root;
    if (root->numKeys == (M-1))
    {
        BTreeNode<M>* newRoot = new BTreeNode<M>;
        this->root = newRoot;
        newRoot->isLeaf = false;
        newRoot->numKeys = 0;
        newRoot->children[1] = root;
        this->splitChild(newRoot, root, 1);
        this->insertNonFull(newRoot, key);
    }
    else
    {
        this->insertNonFull(root, key);
    }
}


// Prints BTree by use of printNode()
template <int M>
void BTree<M>::printBTree() const
{
    this->root->printNode(); //TODO: should there be 'this->'
    cout << endl;
}


// Helper function that splits child which allows the tree to grow
template <int M>
void BTree<M>::splitChild(BTreeNode<M> *parent, BTreeNode<M> *child, int index)
{
    BTreeNode<M>* newNode = new BTreeNode<M>();
    newNode->isLeaf = child->isLeaf;
    newNode->numKeys = ((M/2)-1);

    for (int j = 1; j < ((M/2)-1); j++)
    {
        newNode->keys[j] = child->keys[j+(M/2)];
    }

    if (child->isLeaf == false)
    {
        for (int j = 1; j < (M/2); j++)
        {
            newNode->children[j] = child->children[j+(M/2)];
        }
    }
    child->numKeys = ((M/2)-1);

    for (int j = (parent->numKeys+1); j >= index+1; j--)
    {
        parent->children[j+1] = parent->children[j];
    }

    parent->children[index+1] = newNode;

    for (int j = parent->numKeys; j >= index; j--)
    {
        parent->keys[j+1] = parent->keys[j];
    }

    parent->keys[index] = child->keys[(M/2)];
    parent->numKeys += 1;
}


// Helper function that inserts key into nonfull node
template <int M>
void BTree<M>::insertNonFull(BTreeNode<M>* nonFullNode, int key)
{
    int i = nonFullNode->numKeys;
    if (nonFullNode->isLeaf == true)
    {
        while(i >= 1 && key < nonFullNode->keys[i])
        {
            nonFullNode->keys[i+1] = nonFullNode->keys[i];
            i -= 1;
        }
        nonFullNode->keys[i+1] = key;
        nonFullNode->numKeys += 1;
    }
    else
    {
        while(i >= 1 && key < nonFullNode->keys[i])
        {
            i -= 1;
        }
        i += 1;

        if (nonFullNode->children[i]->numKeys == (M-1))
        {
            splitChild(nonFullNode, nonFullNode->children[i], i);
            if (key > nonFullNode->keys[i])
            {
                i += 1;
            }
        }
        insertNonFull(nonFullNode->children[i], key);
    }
}


// END BTree Implementation
#endif //PROJECT_4_BTREE_H
