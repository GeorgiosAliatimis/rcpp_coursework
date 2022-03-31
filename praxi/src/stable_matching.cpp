#include <unordered_map>
#include <iostream>
#include <list>
#include <vector>
#include <set>
#include <cassert>

#include <Rcpp.h>
using namespace Rcpp;

int int_max = 2147483647; 

/* This function checks all table assertions. */
template <class T>
void assumptions(
    std::unordered_map<T,std::list<T>> proposer_table,
    std::unordered_map<T,std::vector<T>> acceptor_table
){
    int n = proposer_table.size(); // table sizes
    assert(n<int_max); // ensures that the imaginary undesirable proposer has the highest rank
    std::set<T> p_sym, a_sym; // sets of proposer and acceptor symbols
    long int int_counter = 0;
    for(auto p: proposer_table) {
      if ((int_counter++)%1000 == 0) Rcpp::checkUserInterrupt();
      p_sym.insert(p.first);
    }
    for(auto a: acceptor_table) {
      if ((int_counter++)%1000 == 0) Rcpp::checkUserInterrupt();
      a_sym.insert(a.first);
    }
    for (auto p : proposer_table){
        // A preference list in proposer_table may have the wrong dimension
        assert( p.second.size() == n );
        std::set<T> remaining = a_sym; 
        for(auto a: p.second) {
          if ((int_counter++)%1000 == 0) Rcpp::checkUserInterrupt();
          remaining.erase(a);
        }
        // Mismatch between preference list symbols of proposer_table and acceptor symbols if remaining is not empty.
        assert(remaining.empty());
    };

    for (auto a : acceptor_table){
        // A preference list in acceptor_table may have wrong dimension
        assert( a.second.size() == n );
        std::set<T> remaining = p_sym; 
        for(auto p: a.second) {
          if ((int_counter++)%1000 == 0) Rcpp::checkUserInterrupt();
          remaining.erase(p);
        }
        // Mismatch between preference list symbols of acceptor_table and proposer symbols if remaining is not empty.
        assert(remaining.empty());
    };
}


/*
    The Stable Matching Problem is solved using the algorithm descripted in Chapter 2 page 9
    of 'Stable Marriage and Its Relation to Other Combinatorial Problems'.
    A matching is returned.
*/

template <class T>
std::unordered_map<T,T> solve_stable_matching(
    std::unordered_map<T,std::list<T>> proposer_table,
    std::unordered_map<T,std::vector<T>> acceptor_table
){
    assumptions(proposer_table,acceptor_table);
    long int int_counter = 0;
    // Proposer struct    
    struct p {
        std::unordered_map<T,std::list<T>> table;
        std::unordered_map<T,T> matching;
    } proposer;
    proposer.table = proposer_table;
    
    // Acceptor struct
    struct a {
        std::unordered_map<T,std::vector<T>> table;
        std::unordered_map<T,std::unordered_map<T,int>> ranks;
        std::unordered_map<T,int> curr_rank;
    } acceptor;
    acceptor.table = acceptor_table;
    // For any acceptor a, acceptor.ranks[a] is an unordered_map mapping proposers to their rank 
    // according to the preferences of acceptor a.
    for (auto a : acceptor.table){ 
        for (int i = 0; i < a.second.size(); i++){
            if ((int_counter++)%1000 == 0) Rcpp::checkUserInterrupt();
            acceptor.ranks[a.first][a.second[i]] = i; 
        };
    };
    // For any acceptor a, acceptor.curr_rank[a] is the rank of the proposer p that is currently matched with a.
    // In other words if proposer.matching[p]=a, then acceptor.curr_rank[a]=acceptor.ranks[a][p].
    // If acceptor a has not been matched with anyone yet, we say that they have been matched with the "imaginary undesirable proposer", 
    // whose rank is int_max (larger than all other ranks).
    for (auto a : acceptor.table){
      if ((int_counter++)%1000 == 0) Rcpp::checkUserInterrupt();
      acceptor.curr_rank[a.first] = int_max;
    }

    // This algorithm closely follows the implementation referenced above
    for(auto pl : proposer.table){
        T p = pl.first; // current proposer considered
        while(true){
            if ((int_counter++)%1000 == 0) Rcpp::checkUserInterrupt();
            T a = proposer.table[p].front(); // favourite acceptor of p
            proposer.table[p].pop_front(); // popped so that it is not used again
            if (acceptor.ranks[a][p] > acceptor.curr_rank[a]) continue; // in that case a prefers their current proposer to p
            // From now on, "a" prefers "p" to the current matched proposer 
            int prev_rank = acceptor.curr_rank[a];
            acceptor.curr_rank[a] = acceptor.ranks[a][p]; // set curr_rank to the rank of the new proposer
            proposer.matching[p]=a; // match p with a
            if (prev_rank == int_max) break; // break if the previous proposer was the imaginary undesirable proposer
            p = acceptor.table[a][prev_rank]; // else set p to be the previous proposer who should now find a new acceptor
        }
    }
    return proposer.matching; 
}

template <class T>
std::unordered_map<T,T> stable_matching(
    std::vector<std::vector<T>> proposer_matrix,
    std::vector<T> proposer_syms,
    std::vector<std::vector<T>> acceptor_matrix,
    std::vector<T> acceptor_syms
){
  int n = proposer_matrix.size();
  assert(acceptor_matrix.size()==n);
  std::unordered_map<T,std::list<T>> proposer_table;
  std::unordered_map<T,std::vector<T>> acceptor_table;
  for(int i=0;i<n;i++){
    std::list<T> l(proposer_matrix[i].begin(),proposer_matrix[i].end());
    proposer_table[proposer_syms[i]] = l;
    acceptor_table[acceptor_syms[i]] = acceptor_matrix[i];
  }
  auto matching = solve_stable_matching <T> (proposer_table,acceptor_table);
  return matching;
}

//[[Rcpp::export]]
std::unordered_map<std::string,std::string> stable_matching_string(
    std::vector<std::vector<std::string>> proposer_matrix,
    std::vector<std::string> proposer_syms,
    std::vector<std::vector<std::string>> acceptor_matrix,
    std::vector<std::string> acceptor_syms
){
  auto s = stable_matching<std::string>(proposer_matrix,proposer_syms,acceptor_matrix,acceptor_syms);
  return s;
}

//[[Rcpp::export]]
std::unordered_map<int,int> stable_matching_int(
    std::vector<std::vector<int>> proposer_matrix,
    std::vector<int> proposer_syms,
    std::vector<std::vector<int>> acceptor_matrix,
    std::vector<int> acceptor_syms
){
  auto s = stable_matching<int>(proposer_matrix,proposer_syms,acceptor_matrix,acceptor_syms);
  return s;
}

