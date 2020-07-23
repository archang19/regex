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
using namespace std;

struct Node
{
    Node (bool end) {
        m_end = end;
    }
    bool m_end;
    unordered_map<char, Node*> edges;
    unordered_set<Node*> epsilon_edges;
};

void create_transition (Node* source, Node* destination, char transition) {
    source->edges[transition] = destination;
}

void create_eps_transition (Node* source, Node* destination) {
    source->epsilon_edges.insert(destination);
}

vector<Node*> build_eps_nfa ()
{
    Node* s = new Node(false);
    Node* d = new Node(true);
    create_eps_transition(s, d);
    return {s, d};
}

vector<Node*> build_nfa (char transition)
{
    Node* s = new Node(false);
    Node* d = new Node(true);
    create_transition(s, d, transition);
    return {s, d};
}

vector<Node*> concat_op (vector<Node*> one, vector<Node*> two)
{
    one[1]->m_end = false;
    create_eps_transition(one[1], two[0]);
    return {one[0], two[1]};
}

vector<Node*> union_op (vector<Node*> one, vector<Node*> two)
{
    Node* new_start = new Node(false);
    Node* new_end = new Node(true);
    create_eps_transition(new_start, one[0]);
    create_eps_transition(new_start, two[0]);
    create_eps_transition(one[1], new_end);
    create_eps_transition(two[1], new_end);
    
    one[1]->m_end = false;
    two[1]->m_end = false;

    return {new_start, new_end};
}

vector<Node*> closure_op (vector<Node*> cur)
{
    Node* new_start = new Node(false);
    Node* new_end = new Node(true);
    
    create_eps_transition(new_start, new_end);
    create_eps_transition(new_start, cur[0]);
    
    create_eps_transition(cur[1], new_end);
    create_eps_transition(cur[1], cur[0]);
    cur[1]->m_end = false;
    
    return {new_start, new_end};
}


vector<Node*> zero_plus_op (vector<Node*> cur)
{
    Node* new_start = new Node(false);
    Node* new_end = new Node(true);
    create_eps_transition(new_start, new_end);
    create_eps_transition(new_start, cur[0]);
    create_eps_transition(cur[1], new_end);
    cur[1]->m_end = false;
    return {new_start, new_end};
}

vector<Node*> one_plus_op (vector<Node*> cur)
{
    Node* new_start = new Node(false);
    Node* new_end = new Node(true);
    create_eps_transition(new_start, cur[0]);
    create_eps_transition(cur[1], new_end);
    create_eps_transition(cur[1], cur[0]);
    cur[1]->m_end = false;
    return {new_start, new_end};
}

vector<Node*> str_to_nfa (string s)
{
    if (s == "") {
        return build_eps_nfa();
    }
    
    stack<vector<Node*>> st;
    
    for (char c : s)
    {
        if (c == '*') {
            vector<Node*> tmp = st.top();
            st.pop();
            st.push(closure_op(tmp));
        }
        else if (c == '?') {
            vector<Node*> tmp = st.top();
            st.pop();
            st.push(zero_plus_op(tmp));
        }
        else if (c == '+') {
            vector<Node*> tmp = st.top();
            st.pop();
            st.push(one_plus_op(tmp));
        }
        else if (c == '|') {
            vector<Node*> tmp_r = st.top();
            st.pop();
            vector<Node*> tmp_l = st.top();
            st.pop();
            st.push(union_op(tmp_l, tmp_r));
        }
        else if (c == '.') {
            vector<Node*> tmp_r = st.top();
            st.pop();
            vector<Node*> tmp_l = st.top();
            st.pop();
            st.push(concat_op(tmp_l, tmp_r));
        }
        else {
            st.push(build_nfa(c));
        }
    }
    
    return st.top();
}

void add_next_state (Node* state, vector<Node*>& next_states, unordered_set<Node*>& visited) {
    if (state->epsilon_edges.size()) {
        for (auto it = state->epsilon_edges.begin(); it != state->epsilon_edges.end(); it++) {
            if (visited.find(*it) == visited.end()) {
                visited.insert(*it);
                add_next_state(*it, next_states, visited);
            }
        }
    }
    else {
        next_states.push_back(state);
    }
}


bool search (vector<Node*> nfa, string word) {
    vector<Node*> current_states;
    unordered_set<Node*> visited;
    add_next_state(nfa[0], current_states, visited);
    for (char c : word) {
        vector<Node*> next_states;
        for (Node* state : current_states) {
            Node* next_state = state->edges[c];
            if (next_state) {
                unordered_set<Node*> next_visited;
                add_next_state(next_state, next_states, next_visited);
            }
        }
        current_states = next_states;
    }
    for (Node* n : current_states) {
        if (n->m_end == true) {
            return true;
        }
    }
    return false;
}


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


vector<Node*> create_matcher (string exp) {
    string post_fix_exp = to_post_fix(insert_explicit_concat_operator(exp));
    return str_to_nfa(post_fix_exp);
}



int main(int argc, const char * argv[]) {
    // insert code here...

    string exp = "(a|b)*c";
    vector<Node*> nfa = create_matcher(exp);
    cout << search(nfa, "ac") << endl;
    cout << search(nfa, "abc") << endl;
    cout << search(nfa, "aabababbbc") << endl;
    cout << search(nfa, "aaaab") << endl;


    return 0;
}
