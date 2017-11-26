/*
 * Ethics Statement:
 * Other than anything noted in the disclosures below, the code submitted is of my design and my design alone.
 * I recieved no help nor used any references, resources, and had no discussion with other classmates.
 * I understand the University policy concerning ethics and
 * understand that violating this policy will result in drastic repercussions.
 *
 * Disclosures:
 * No disclosures.
*/

#include <iostream>
#include <fstream>
#include <string>
#include <stdlib.h>
#include "BTree.h"

using namespace std;

template <int M>
void userInput(BTree<M> btree)
{
    do
    {
        cout << endl << "Please enter a command..." << endl;
        cout << "Add Key: a #" << endl;
        cout << "Remove Key: r" << endl;
        cout << "Quit: q" << endl << endl;

        char userChoice[1];
        cin >> userChoice;

        if (userChoice[0] == 'q')
        {
            exit(0);
        }
        else if (userChoice[0] == 'r')
        {
            int key;
            cin >> key;
//            btree.remove(key);
            btree.printBTree();
        }
        else
        {
            int key;
            cin >> key;
            btree.insert(key);
            btree.printBTree();
        }
    }
    while(true);
}

int main(int argc, const char *argv[])
{
    // Creates 7 BTrees of orders 4-10 as specified by Professor Bolton
    BTree<4> btree4;
    BTree<5> btree5;
    BTree<6> btree6;
    BTree<7> btree7;
    BTree<8> btree8;
    BTree<9> btree9;
    BTree<10> btree10;


    // Gets filepath and creates ifstream object.
    string filePath = string(argv[1]);
    ifstream dataFile;
    dataFile.open(filePath.c_str());
    int order;
    int key;

    // Validates file
    if(dataFile.good())
    {
        dataFile >> order;
        cout << endl << "order = " << order << endl;
        switch(order)
        {
            case 4:
                while(dataFile >> key)
                {
                    btree4.insert(key);
                }
                cout << endl << "Your " << order << " order B-tree has been initialized to: ";
                btree4.printBTree();
                userInput(btree4);
                break;
            case 5:
                while(dataFile >> key)
                {
                    btree5.insert(key);
                }
                cout << endl << "Your " << order << " order B-tree has been initialized to: ";
                btree5.printBTree();
                userInput(btree5);
                break;
            case 6:
                while(dataFile >> key)
                {
                    btree6.insert(key);
                }
                cout << endl << "Your " << order << " order B-tree has been initialized to: ";
                btree6.printBTree();
                userInput(btree6);
                break;
            case 7:
                while(dataFile >> key)
                {
                    btree7.insert(key);
                }
                cout << endl << "Your " << order << " order B-tree has been initialized to: ";
                btree7.printBTree();
                userInput(btree7);
                break;
            case 8:
                while(dataFile >> key)
                {
                    btree8.insert(key);
                }
                cout << endl << "Your " << order << " order B-tree has been initialized to: ";
                btree8.printBTree();
                userInput(btree8);
                break;
            case 9:
                while(dataFile >> key)
                {
                    btree9.insert(key);
                }
                cout << endl << "Your " << order << " order B-tree has been initialized to: ";
                btree9.printBTree();
                userInput(btree9);
                break;
            case 10:
                while(dataFile >> key)
                {
                    btree10.insert(key);
                }
                cout << endl << "Your " << order << " order B-tree has been initialized to: ";
                btree10.printBTree();
                userInput(btree10);
                break;
            default:
                break;
        }

        // Closes file after successfully reading data BTree.
        dataFile.close();
    }
    else
    {
        // Closes files since something went wrong.
        // Normally I would include a cout here but the project stated there should be no other output.
        dataFile.close();
    }
}
