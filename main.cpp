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
    string exp = "(c|d)*e";
    vector<Node*> n = a.create_matcher(exp);
    cout << a.search(n, "ce") << endl;
    cout << a.search(n, "cde") << endl;
    cout << a.search(n, "cccdddcde") << endl;
    cout << a.search(n, "cdcdee") << endl;
    return 0;
}
