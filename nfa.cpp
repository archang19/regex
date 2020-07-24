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


nfa::nfa(string exp) {
    m_master.push_back(nullptr);
    m_master.push_back(nullptr);
    create_matcher(exp);
}

nfa::~nfa() {
    for (int i = 0; i < m_nodes.size(); i++) {
        if (m_nodes[i] != nullptr) {
             free(m_nodes[i]);
        }
    }
}

void nfa::create_transition (node* source, node* destination, char transition) {
    source->edges[transition] = destination;
}

void nfa::create_eps_transition (node* source, node* destination) {
    source->epsilon_edges.insert(destination);
}

vector<node*> nfa::create_matcher (string exp) {
    string post_fix_exp = to_post_fix(insert_explicit_concat_operator(exp));
    return str_to_nfa(post_fix_exp);
}

vector<node*> nfa::build_eps_nfa ()
{
    node* s = new node(false);
    node* d = new node(true);
    m_nodes.push_back(s);
    m_nodes.push_back(d);
    create_eps_transition(s, d);
    m_master[0] = s;
    m_master[1] = d;
    return m_master;
}

vector<node*> nfa::build_nfa (char transition)
{
    node* s = new node(false);
    node* d = new node(true);
    m_nodes.push_back(s);
    m_nodes.push_back(d);
    create_transition(s, d, transition);
    m_master[0] = s;
    m_master[1] = d;
    return m_master;
}

vector<node*> nfa::concat_op (vector<node*> one, vector<node*> two)
{
    one[1]->m_end = false;
    create_eps_transition(one[1], two[0]);
    
    m_master[0] = one[0];
    m_master[1] = two[1];
    return m_master;
}

vector<node*> nfa::union_op (vector<node*> one, vector<node*> two)
{
    node* new_start = new node(false);
    node* new_end = new node(true);
    m_nodes.push_back(new_start);
    m_nodes.push_back(new_end);
    
    create_eps_transition(new_start, one[0]);
    create_eps_transition(new_start, two[0]);
    create_eps_transition(one[1], new_end);
    create_eps_transition(two[1], new_end);
    
    one[1]->m_end = false;
    two[1]->m_end = false;

    m_master[0] = new_start;
    m_master[1] = new_end;
    return m_master;
}

vector<node*> nfa::closure_op (vector<node*> cur)
{
    node* new_start = new node(false);
    node* new_end = new node(true);
    m_nodes.push_back(new_start);
    m_nodes.push_back(new_end);
    
    create_eps_transition(new_start, new_end);
    create_eps_transition(new_start, cur[0]);
    
    create_eps_transition(cur[1], new_end);
    create_eps_transition(cur[1], cur[0]);
    cur[1]->m_end = false;
    
    m_master[0] = new_start;
    m_master[1] = new_end;
    return m_master;
}


vector<node*> nfa::zero_plus_op (vector<node*> cur)
{
    node* new_start = new node(false);
    node* new_end = new node(true);
    
    m_nodes.push_back(new_start);
    m_nodes.push_back(new_end);
    
    create_eps_transition(new_start, new_end);
    create_eps_transition(new_start, cur[0]);
    create_eps_transition(cur[1], new_end);
    
    cur[1]->m_end = false;
    m_master[0] = new_start;
    m_master[1] = new_end;
    return m_master;
}

vector<node*> nfa::one_plus_op (vector<node*> cur)
{
    node* new_start = new node(false);
    node* new_end = new node(true);
    
    m_nodes.push_back(new_start);
    m_nodes.push_back(new_end);
    
    create_eps_transition(new_start, cur[0]);
    create_eps_transition(cur[1], new_end);
    create_eps_transition(cur[1], cur[0]);
    
    cur[1]->m_end = false;
    
    m_master[0] = new_start;
    m_master[1] = new_end;
    return m_master;
}

vector<node*> nfa::str_to_nfa (string s)
{
    if (s == "") {
        return build_eps_nfa();
    }
    
    stack<vector<node*>> st;
    
    for (char c : s)
    {
        if (c == '*') {
            vector<node*> tmp = st.top();
            st.pop();
            st.push(closure_op(tmp));
        }
        else if (c == '?') {
            vector<node*> tmp = st.top();
            st.pop();
            st.push(zero_plus_op(tmp));
        }
        else if (c == '+') {
            vector<node*> tmp = st.top();
            st.pop();
            st.push(one_plus_op(tmp));
        }
        else if (c == '|') {
            vector<node*> tmp_r = st.top();
            st.pop();
            vector<node*> tmp_l = st.top();
            st.pop();
            st.push(union_op(tmp_l, tmp_r));
        }
        else if (c == '.') {
            vector<node*> tmp_r = st.top();
            st.pop();
            vector<node*> tmp_l = st.top();
            st.pop();
            st.push(concat_op(tmp_l, tmp_r));
        }
        else {
            st.push(build_nfa(c));
        }
    }
    
    return st.top();
}

void nfa::add_next_state (node* state, vector<node*>& next_states, unordered_set<node*>& visited) {
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


bool nfa::search (string word) {
    vector<node*> current_states;
    unordered_set<node*> visited;
    add_next_state(m_master[0], current_states, visited);
    for (char c : word) {
        vector<node*> next_states;
        for (node* state : current_states) {
            node* next_state = state->edges[c];
            if (next_state) {
                unordered_set<node*> next_visited;
                add_next_state(next_state, next_states, next_visited);
            }
        }
        current_states = next_states;
    }
    for (node* n : current_states) {
        if (n->m_end == true) {
            return true;
        }
    }
    return false;
}
