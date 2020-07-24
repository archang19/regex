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
using namespace std;

int main(int argc, const char * argv[]) {
    nfa a;
    string exp = "(a|b)*c";
    
    vector<Node*> nfa = a.create_matcher(exp);
    cout << a.search(nfa, "ac") << endl;
    cout << a.search(nfa, "abc") << endl;
    cout << a.search(nfa, "aabababbbc") << endl;
    cout << a.search(nfa, "aaaab") << endl;
    
    return 0;
}
