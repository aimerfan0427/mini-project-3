#include <cstdlib>
#include <vector>
#include <algorithm>
#include <iostream>

#include "../state/state.hpp"
#include "./ab.hpp"

using namespace std;

/**
 * @brief Randomly get a legal action
 * 
 * @param state Now state
 * @param depth You may need this for other policy
 * @return Move 
 */

Move ab::get_move(State *state, int depth){//在這裡call evaluate()
  
  /*if(!state->legal_actions.size())
    state->get_legal_actions();
  */
  //auto actions = state->legal_actions;
  construct_tree(state,depth);

  int who_am_I=state->player;//0:我是白,1:我是黑

  Move move;

  int value=-100000000;
  for(auto the_next_state:state->next_states){
    the_next_state->state_value=go_minimax(state,depth-1,!state->player,who_am_I,-100000000,100000000);
    //value=max(value,the_next_state->state_value);
  }
  for(size_t i=0;i <state->next_states.size();i++){
    if(state->next_states[i]->state_value > value){
      value=state->next_states[i]->state_value;
      move=state->legal_actions[i];
    }
  }

  cout<<value<<"???????\n\n\n\n\n"<<endl;

  /*for(size_t i=0;i < (state->legal_actions.size());i++){
    if(state->next_states[i]->state_value == value){
      cout<<state->next_states[i]->state_value<<"!!!!!!!!!\n\n\n\n\n"<<endl;
      move=state->legal_actions[i];
      break;
    }
    
  }*/

  return move;
}

int ab::go_minimax(State *state,int depth,int player,int who_am_I,int alpha,int beta){

  int value=0;
  bool my_turn;
  my_turn=!(player ^ who_am_I);

  if(depth == 0){
    state->state_value=state->evaluate(my_turn,who_am_I);
    return state->state_value;
  }
  //if(state->game_state == WIN ) return 10000;

  if(my_turn){//should maximize the value
    int maximum=-100000000;

    for(auto the_next_state:state->next_states){
      value=go_minimax(the_next_state,depth-1,!player,who_am_I,alpha,beta);
      maximum=max(value,maximum);
      alpha=max(alpha,value);
      if(alpha >= beta){
        break;
      }
    }

    //state->state_value=maximum;

    return maximum;
  }
  else{
    int minimum=100000000;

    for(auto the_next_state:state->next_states){
      value=go_minimax(the_next_state,depth-1,!player,who_am_I,alpha,beta);
      minimum=min(value,minimum);
      beta=min(beta,value);
      if(alpha >= beta){
        break;
      }
    }

    //state->state_value=minimum;

    return minimum;
  }
}

void ab::construct_tree(State *state,int depth){

  if(depth == 0) return ;

  State *next;
  state->get_legal_actions();

  for(size_t i=0;i < state->legal_actions.size();i++){
    Move move=state->legal_actions[i];
    next=state->next_state(move);
    state->next_states.push_back(next);

    construct_tree(next,depth-1);
  }

  return;
}