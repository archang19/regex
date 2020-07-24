//
//  main.cpp
//  regex
//
//  Created by Andrew Chang on 7/16/20.
//  Copyright © 2020 Andrew Chang. All rights reserved.
//

#include <iostream>
#include <unordered_map>
#include <unordered_set>
#include <vector>
#include <stack>
#include "nfa.h"
#include <fstream>
using namespace std;

int main(int argc, const char * argv[]) {
    
    string to_search;
    string exp;

    std::string filename;
    std::ifstream inFile;
    
    cout << "Please enter regular expression" << endl;
    getline(cin, exp);
    
    std::cout << "Please enter the full path of the input file: ";
    std::getline( std::cin, filename );

    inFile.open( filename.c_str() );

    if( !inFile )
    {
      std::cout << "Unable to open file: " << filename << std::endl;
      return -1;
    }

    // File operations here
    inFile >> to_search;
    inFile.close();
    
    nfa a(exp);

    if (a.search(to_search)) {
        cout << "Found match for: " << exp << endl;
    }
    else {
        cout << "Did not find match for: " << exp << endl;
    }
    return 0;
}
