//
//  nfa.cpp
//  regex
//
//  Created by Andrew Chang on 7/23/20.
//  Copyright © 2020 Andrew Chang. All rights reserved.
//

#include "nfa.h"
#include "formatter.h"
#include <stack>
using namespace std;

void nfa::create_transition (Node* source, Node* destination, char transition) {
    source->edges[transition] = destination;
}

void nfa::create_eps_transition (Node* source, Node* destination) {
    source->epsilon_edges.insert(destination);
}

vector<Node*> nfa::create_matcher (string exp) {
    string post_fix_exp = to_post_fix(insert_explicit_concat_operator(exp));
    return str_to_nfa(post_fix_exp);
}

vector<Node*> nfa::build_eps_nfa ()
{
    Node* s = new Node(false);
    Node* d = new Node(true);
    create_eps_transition(s, d);
    return {s, d};
}

vector<Node*> nfa::build_nfa (char transition)
{
    Node* s = new Node(false);
    Node* d = new Node(true);
    create_transition(s, d, transition);
    return {s, d};
}

vector<Node*> nfa::concat_op (vector<Node*> one, vector<Node*> two)
{
    one[1]->m_end = false;
    create_eps_transition(one[1], two[0]);
    return {one[0], two[1]};
}

vector<Node*> nfa::union_op (vector<Node*> one, vector<Node*> two)
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

vector<Node*> nfa::closure_op (vector<Node*> cur)
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


vector<Node*> nfa::zero_plus_op (vector<Node*> cur)
{
    Node* new_start = new Node(false);
    Node* new_end = new Node(true);
    create_eps_transition(new_start, new_end);
    create_eps_transition(new_start, cur[0]);
    create_eps_transition(cur[1], new_end);
    cur[1]->m_end = false;
    return {new_start, new_end};
}

vector<Node*> nfa::one_plus_op (vector<Node*> cur)
{
    Node* new_start = new Node(false);
    Node* new_end = new Node(true);
    create_eps_transition(new_start, cur[0]);
    create_eps_transition(cur[1], new_end);
    create_eps_transition(cur[1], cur[0]);
    cur[1]->m_end = false;
    return {new_start, new_end};
}

vector<Node*> nfa::str_to_nfa (string s)
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

void nfa::add_next_state (Node* state, vector<Node*>& next_states, unordered_set<Node*>& visited) {
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


bool nfa::search (vector<Node*> nfa, string word) {
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
