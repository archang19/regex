//
//  formatter.cpp
//  regex
//
//  Created by Andrew Chang on 7/23/20.
//  Copyright © 2020 Andrew Chang. All rights reserved.
//

#include "formatter.h"
#include <stack>
#include <unordered_map>
using namespace std;


string to_post_fix (string exp) {
    unordered_map<char,int> precedence;
    precedence['|'] = 0;
    precedence['.'] = 1;
    precedence['?'] = 2;
    precedence['*'] = 2;
    precedence['+'] = 2;
    
    string res = "";
    stack<char> st;
    for (char c : exp) {
        if (c == '.' || c == '|' || c == '*' || c == '?' || c =='+') {
            while (st.size() && st.top() != '(' && precedence[st.top()] >= precedence[c]) {
                res += st.top();
                st.pop();
            }
            st.push(c);
        }
        else if (c == '(' || c == ')') {
            if (c == '(') {
                st.push(c);
            }
            else {
                while (st.top() != '(') {
                    res += st.top();
                    st.pop();
                }
                st.pop();
            }
        }
        else {
            res += c;
        }
    }
    
    while (st.size()) {
        res += st.top();
        st.pop();
    }
    return res;
}

string insert_explicit_concat_operator(string exp) {
    string output = "";
    for (int i = 0; i < exp.size(); i++) {
        char c = exp[i];
        output += c;
        if (c == '(' || c == '|') {
            continue;
        }
        if (i < exp.size() - 1) {
            char look_ahead = exp[i+1];
            
            if (look_ahead == '*' || look_ahead == '?' || look_ahead == '+' || look_ahead == '|' || look_ahead == ')') {
                continue;
            }

            output += '.';
            
        }
    }
    return output;
}
