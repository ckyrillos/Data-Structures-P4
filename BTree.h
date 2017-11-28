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
    if (this->isLeaf == false)
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
    void insert(int);
    void remove(int);
    void removeKeyFromNode(BTreeNode<M>*, int);
    void printBTree() const;

private:
    // Private Operations
    void insertNonFull(BTreeNode<M>*, int);
    void splitChild(BTreeNode<M>*, BTreeNode<M>*, int);

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
    if (root->numKeys == (M-1))
    {
        BTreeNode<M> *newNode = new BTreeNode<M>();
        newNode->isLeaf = false;
        newNode->numKeys = 0;
        newNode->children[0] = root;

        // Splits old root and moves key into new root
        this->splitChild(newNode, newNode->children[0], 0);

        // Decides which of the two children will get new key
        int i = 0;
        if (newNode->keys[0] < key)
        {
            i++;
        }

        // Inserts key into appropriate child
        this->insertNonFull(newNode->children[i], key);

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
void BTree<M>::insertNonFull(BTreeNode<M>* nonFullNode, int key)
{
    // Creates an index i to be the index of the rightmost element
    int i = nonFullNode->numKeys-1;

    if (nonFullNode->isLeaf == true)
    {
        // Finds location where new key will be added and add all greater keys ahead of it
        while (i >= 0 && nonFullNode->keys[i] > key)
        {
            nonFullNode->keys[i+1] = nonFullNode->keys[i];
            i--;
        }

        // Inserts the new key
        nonFullNode->keys[i+1] = key;
        nonFullNode->numKeys = nonFullNode->numKeys+1;
    }
    else
    {
        while (i >= 0 && nonFullNode->keys[i] > key)
        {
            i--;
        }

        // Checks if child is full
        if (nonFullNode->children[i+1]->numKeys == (M-1))
        {
            this->splitChild(nonFullNode, nonFullNode->children[i+1], (i+1));

            if (nonFullNode->keys[i+1] < key)
            {
                i++;
            }
        }
        insertNonFull(nonFullNode->children[i+1], key);
    }
}


// Utility function to split child into two nodes which allows BTree to grow
template <int M>
void BTree<M>::splitChild(BTreeNode<M>* parent, BTreeNode<M> *child, int i)
{
    BTreeNode<M>* newNode = new BTreeNode<M>();
    newNode->isLeaf = child->isLeaf;
    newNode->numKeys = (M-1)/2;

    // Copies second half of keys to new node
    for (int j = 0; j < ((M-1)/2); j++)
    {
        newNode->keys[j] = child->keys[j+(M/2)];
        child->keys[j+(M/2)] = 0;
    }

    // Copies second half of children to new node
    if (child->isLeaf == false)
    {
        for (int j = 0; j < ((M+1)/2); j++)
        {
            newNode->children[j] = child->children[j+(M/2)];
            child->children[j+(M/2)] = 0;
        }
        child->numKeys = M-((M-1)/2)-1;
    }

    // Creates room for new child in parent->child
    for (int j = parent->numKeys + 1; j > i+1; j--)
    {
        parent->children[j] = parent->children[j-1];
    }

    // Inserts the new child
    parent->children[i+1] = newNode;

    // Makes room for the new key for parent
    for (int j = parent->numKeys; j > i; j--)
    {
        parent->keys[j] = parent->keys[j-1];
    }

    // Inserts the new key in parent
    parent->keys[i] = child->keys[(M/2)-1];
    child->keys[(M/2)-1] = 0;
    child->numKeys = (M/2)-1;
    parent->numKeys++;
}

// Remove Operator for BTree
template <int M>
void BTree<M>::remove(int key)
{
    this->removeKeyFromNode(root, key);
}


// Utility function which actually does the removal
template <int M>
void BTree<M>::removeKeyFromNode(BTreeNode<M> * thisNode, int key)
{
    int i = 0;
    while (i < thisNode->numKeys && key > thisNode->keys[i])
    {
        i++;
    }

    // Case 1: Key is in thisNode and thisNode is a leaf node
    if (thisNode->isLeaf)
    {
        // If key is found, begin execution. If key is not in tree, do nothing.
        if(thisNode->keys[i] == key)
        {
            while (i <= thisNode->numKeys-2)
            {
                thisNode->keys[i] = thisNode->keys[i+1];
                i++;
            }
            thisNode->keys[i] = 0;
            thisNode->numKeys--;
        }
    }

    // Case 2: Key is in thisNode and thisNode is internal
    else if (i < thisNode->numKeys && thisNode->keys[i] == key)
    {
        // Subcase 2A: The child before the key has extra keys
        if(thisNode->children[i]->numKeys > ((M/2)-1))
        {
            BTreeNode<M>* child1 = thisNode->children[i];
            while (child1->isLeaf == false)
            {
                child1 = child1->children[child1->numKeys];
            }

            thisNode->keys[i] = child1->keys[child1->numKeys-1];
            removeKeyFromNode(thisNode->children[i], child1->keys[child1->numKeys-1]);
        }

        // Subcase 2B:The child before the key has no extra keys but the child after it does
        else if ((i+1) <= thisNode->numKeys && thisNode->children[i+1]->numKeys > ((M/2)-1))
        {
            BTreeNode<M>* child2 = thisNode->children[i+1];
            while (child2->isLeaf == false)
            {
                child2 = child2->children[0];
            }
            thisNode->keys[i] = child2->keys[0];
            removeKeyFromNode(thisNode->children[i+1], child2->keys[0]);
        }

        // Subcase 2C: Neither the child before the key nor the child after it have extra keys
        else
        {
            BTreeNode<M>* child1 = thisNode->children[i];
            BTreeNode<M>* child2 = thisNode->children[i+1];

            // Checks if thisNode is the root and only has one key because if so it will need to be deleted at end
            bool isRootOne = false;
            if (thisNode == root && thisNode->numKeys == 1)
            {
                isRootOne = true;
            }

            // Puts key in child1
            child1->keys[child1->numKeys] = key;
            child1->numKeys++;

            // If child2 exists, merges child2 into child1
            if (child2 != 0)
            {
                for (int j = 0; j < child2->numKeys; j++)
                {
                    child1->keys[child1->numKeys] = child2->keys[j];
                    child1->children[child1->numKeys] = child2->children[j];
                    child1->numKeys++;
                }
                child1->children[child1->numKeys] = child2->children[child2->numKeys];
            }

            // Deletes key and child2's pointer from child 1, and then moves everything over
            for (int j = i; j < thisNode->numKeys; j++)
            {
                thisNode->keys[j] = thisNode->keys[j+1];
                thisNode->children[j+1] = thisNode->children[j+2];
            }
            thisNode->numKeys--;

            // Reassigns root since thisNode was determined to be the root and only have one key at beginning
            if (isRootOne)
            {
                root = child1;
            }

            removeKeyFromNode(child1, key);
        }
    }

    // Case 3: Key is not in thisNode
    else
    {
        BTreeNode<M>* child = thisNode->children[i];

        if (child->numKeys <= ((M/2)-1))
        {
            // Subcase 3A: Either the left child or right child has extra keys
            if ((i-1) >= 0 && thisNode->children[i-1]->numKeys > ((M/2)-1))
            {
                // Left child has extra keys
                BTreeNode<M>* leftChild = thisNode->children[i-1];

                // Makes space in child for a new key
                for (int j = child->numKeys; j > 0; j--)
                {
                    child->keys[j] = child->keys[j-1];
                    child->children[j+1] = child->children[j];
                }
                child->children[1] = child->children[0];
                child->numKeys++;

                // Moves a key from thisNode into child
                child->keys[0] = thisNode->keys[i-1];

                // Moves a pointer from leftNode into child
                child->children[0] = leftChild->children[leftChild->numKeys];

                // Brings a key from leftNode into thisNode and fixes leftNode since it lost a key
                thisNode->keys[i-1] = leftChild->keys[leftChild->numKeys-1];
                leftChild->keys[leftChild->numKeys-1] = 0;
                leftChild->children[leftChild->numKeys] = 0;
                leftChild->numKeys--;

            }
            else if ((i+1) <= thisNode->numKeys && thisNode->children[i+1]->numKeys > ((M/2)-1))
            {
                // Right child has extra keys
                BTreeNode<M>* rightChild = thisNode->children[i+1];

                // Moves a key from thisNode into child
                child->keys[child->numKeys] = thisNode->keys[i];
                child->numKeys++;

                // Moves a pointer from rightNode into child
                child->children[child->numKeys] = rightChild->children[0];

                // Brings a key from rightNode into thisNode and fixes rightNode since it lost a key
                thisNode->keys[i] = rightChild->keys[0];
                rightChild->numKeys--;
                for (int j = 0; j < rightChild->numKeys; j++)
                {
                    rightChild->keys[j] = rightChild->keys[j+1];
                    rightChild->children[j] = rightChild->children[j+1];
                }
                rightChild->keys[rightChild->numKeys] = 0;
                rightChild->children[rightChild->numKeys] = rightChild->children[rightChild->numKeys+1];
                rightChild->children[rightChild->numKeys+1] = 0;
            }

            // Subcase 3B: Neither left child nor right child has extra keys
            else
            {
                BTreeNode<M>* leftChild = 0;
                BTreeNode<M>* rightChild = 0;

                // Checks if thisNode is the root and only has one key because if so it will need to be deleted at end
                bool isRootOne = false;
                if (thisNode == root && thisNode->numKeys == 1)
                {
                    isRootOne = true;
                }

                // Determines if child and siblings are left or right from each other and adjusts accordingly
                if ((i+1) > thisNode->numKeys)
                {
                    rightChild = child;
                    leftChild = thisNode->children[i-1];
                    leftChild->keys[leftChild->numKeys] = thisNode->keys[i-1];
                }
                else
                {
                    rightChild = thisNode->children[i+1];
                    leftChild = child;
                    leftChild->keys[leftChild->numKeys] = thisNode->keys[i];
                }
                leftChild->numKeys++;

                // Moves rightChild into leftChild
                for (int j = 0; j < rightChild->numKeys; j++)
                {
                    leftChild->keys[leftChild->numKeys] = rightChild->keys[j];
                    leftChild->children[leftChild->numKeys] = rightChild->children[j];
                    leftChild->numKeys++;
                }
                leftChild->children[leftChild->numKeys] = rightChild->children[rightChild->numKeys];

                // Moves everything over to delete key and sets child to leftChild
                for (int j = i; j < thisNode->numKeys-1; j++)
                {
                    thisNode->keys[j] = thisNode->keys[j+1];
                    thisNode->children[j+1] = thisNode->children[j+2];
                }
                thisNode->numKeys--;
                thisNode->children[thisNode->numKeys+1] = 0;
                child = leftChild;

                // Reassigns root since thisNode was determined to be the root and only have one key at beginning
                if(isRootOne)
                {
                    root = child;
                }
            }
        }

        // Recursively executes when the key isn't found
        removeKeyFromNode(child, key);
    }
}
// END BTree Implementation

#endif //PROJECT_4_BTREE_H
