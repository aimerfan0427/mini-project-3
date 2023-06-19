#pragma once
#include "../state/state.hpp"
#include<vector>

using namespace std;


/**
 * @brief Policy class for random policy, 
 * your policy class should have get_move method
 */
class ab{
public:
  static Move get_move(State *state, int depth);
  static int go_minimax(State *state,int depth,int player,int who_am_I,int alpha,int beta);
  static void construct_tree(State *state,int depth);
};