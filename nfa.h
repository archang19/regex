//
//  nfa.h
//  regex
//
//  Created by Andrew Chang on 7/23/20.
//  Copyright © 2020 Andrew Chang. All rights reserved.
//

#ifndef nfa_h
#define nfa_h

#include <unordered_map>
#include <unordered_set>
#include <vector>
#include <string>



struct Node
{
    Node (bool end) {
        m_end = end;
    }
    bool m_end;
    std::unordered_map<char, Node*> edges;
    std::unordered_set<Node*> epsilon_edges;
};

struct nfa
{
    void create_transition (Node* source, Node* destination, char transition);
    void create_eps_transition (Node* source, Node* destination);
    std::vector<Node*> create_matcher (std::string exp);
    std::vector<Node*> build_eps_nfa ();
    std::vector<Node*> build_nfa (char transition);
    std::vector<Node*> concat_op (std::vector<Node*> one, std::vector<Node*> two);
    std::vector<Node*> union_op (std::vector<Node*> one, std::vector<Node*> two);
    std::vector<Node*> closure_op (std::vector<Node*> cur);
    std::vector<Node*> zero_plus_op (std::vector<Node*> cur);
    std::vector<Node*> one_plus_op (std::vector<Node*> cur);

    std::vector<Node*> str_to_nfa (std::string s);

    void add_next_state (Node* state, std::vector<Node*>& next_states, std::unordered_set<Node*>& visited);
    bool search (std::vector<Node*> nfa, std::string word);
};

#endif /* nfa_h */
