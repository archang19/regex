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



struct node
{
    node (bool end) {
        m_end = end;
    }
    bool m_end;
    std::unordered_map<char, node*> edges;
    std::unordered_set<node*> epsilon_edges;
};

class nfa
{
    
public:
    nfa(std::string exp);
    ~nfa();
    void create_transition (node* source, node* destination, char transition);
    void create_eps_transition (node* source, node* destination);
    std::vector<node*> create_matcher (std::string exp);
    std::vector<node*> build_eps_nfa ();
    std::vector<node*> build_nfa (char transition);
    std::vector<node*> concat_op (std::vector<node*> one, std::vector<node*> two);
    std::vector<node*> union_op (std::vector<node*> one, std::vector<node*> two);
    std::vector<node*> closure_op (std::vector<node*> cur);
    std::vector<node*> zero_plus_op (std::vector<node*> cur);
    std::vector<node*> one_plus_op (std::vector<node*> cur);

    std::vector<node*> str_to_nfa (std::string s);

    void add_next_state (node* state, std::vector<node*>& next_states, std::unordered_set<node*>& visited);
    bool search (std::string word);

private:
    std::vector<node*> m_nodes;
    std::vector<node*> m_master;
};

#endif /* nfa_h */
