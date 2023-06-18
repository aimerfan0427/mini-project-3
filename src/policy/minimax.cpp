#include <cstdlib>
#include <vector>
#include <algorithm>

#include "../state/state.hpp"
#include "./minimax.hpp"

using namespace std;

/**
 * @brief Randomly get a legal action
 * 
 * @param state Now state
 * @param depth You may need this for other policy
 * @return Move 
 */

Move minimax::get_move(State *state, int depth){//在這裡call evaluate()
  
  //if(depth == 0 || state->game_state == WIN || state->game_state == DRAW)
  
  if(!state->legal_actions.size())
    state->get_legal_actions();
  
  //auto actions = state->legal_actions;
  state->next_states=construct_tree(state,depth-1);

  int who_am_I=state->player;//0:我是白,1:我是黑

  Move move;

  int value=-100000000;
  for(auto the_next_state:state->next_states){
    the_next_state->state_value=go_minimax(state,depth,!state->player,who_am_I);
    value=max(value,the_next_state->state_value);
  }
  for(size_t i=0;i < (state->legal_actions.size());i++){
    if(state->next_states[i]->state_value == value){
      move=state->legal_actions[i];
    }
  }

  return move;
}

int minimax::go_minimax(State *state,int depth,int player,int who_am_I){

  int value;
  bool my_turn;
  my_turn=!(player ^ who_am_I);

  if(depth ==0 || state->game_state == WIN || state->game_state == DRAW){
    state->state_value=state->evaluate(my_turn);
    return state->state_value;
  }

  if(my_turn){//should maximize the value
    value=-100000000;

    for(auto the_next_state:state->next_states){
      value=max(value,go_minimax(the_next_state,depth-1,!player,who_am_I));
    }
  }
  else{
    value=100000000;

    for(auto the_next_state:state->next_states){
      value=min(value,go_minimax(the_next_state,depth-1,!player,who_am_I));
    }
  }
  state->state_value=value;

  return value;
}

vector<State*> minimax::construct_tree(State *state,int depth){//有錯

  State *next;
  for(auto move:state->legal_actions){
    next=state->next_state(move);
    next->next_states=construct_tree(next,depth-1);

    state->next_states.push_back(next);
  }

  if(depth == 0 || state->game_state == WIN || state->game_state == DRAW) return state->next_states;
}