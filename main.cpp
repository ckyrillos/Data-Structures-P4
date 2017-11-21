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

int main(int argc, const char *argv[])
{
    // Gets filepath and creates ifstream object.
    string filePath = string(argv[1]);
    ifstream dataFile;
    dataFile.open(filePath.c_str());

    // Validates file
    if(dataFile.good())
    {
        // Closes file after successfully reading data into doubly linked list.
        dataFile.close();
    }
    else
    {
        // Closes files since something went wrong.
        // Normally I would include a cout here but the project stated there should be no other output.
        dataFile.close();
    }

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
            cout << "Remove" << endl;
//            heap.remove();
//            cout << heap << endl;
        }
        else
        {
            int key;
            cin >> key;
            cout << "Add" << endl;
//            heap.add(key);
//            cout << heap << endl;
        }
    }
    while(true);

}