#include <iostream>
#include <sstream>
#include <cstdint>
#include <cmath>


#include "./state.hpp"
#include "../config.hpp"

using namespace std;

/**
 * @brief evaluate the state
 * 
 * @return int 
 */


static const int move_table_rook_bishop[8][7][2] = {
  {{0, 1}, {0, 2}, {0, 3}, {0, 4}, {0, 5}, {0, 6}, {0, 7}},
  {{0, -1}, {0, -2}, {0, -3}, {0, -4}, {0, -5}, {0, -6}, {0, -7}},
  {{1, 0}, {2, 0}, {3, 0}, {4, 0}, {5, 0}, {6, 0}, {7, 0}},
  {{-1, 0}, {-2, 0}, {-3, 0}, {-4, 0}, {-5, 0}, {-6, 0}, {-7, 0}},
  {{1, 1}, {2, 2}, {3, 3}, {4, 4}, {5, 5}, {6, 6}, {7, 7}},
  {{1, -1}, {2, -2}, {3, -3}, {4, -4}, {5, -5}, {6, -6}, {7, -7}},
  {{-1, 1}, {-2, 2}, {-3, 3}, {-4, 4}, {-5, 5}, {-6, 6}, {-7, 7}},
  {{-1, -1}, {-2, -2}, {-3, -3}, {-4, -4}, {-5, -5}, {-6, -6}, {-7, -7}},
};
static const int move_table_knight[8][2] = {
  {1, 2}, {1, -2},
  {-1, 2}, {-1, -2},
  {2, 1}, {2, -1},
  {-2, 1}, {-2, -1},
};
static const int move_table_king[8][2] = {
  {1, 0}, {0, 1}, {-1, 0}, {0, -1}, 
  {1, 1}, {1, -1}, {-1, 1}, {-1, -1},
};

int State::evaluate(bool my_turn,int who_am_I){
  // [TODO] design your own evaluation function
  int chess_value[7]={0, 2, 6, 7, 8, 20, 100};
  float weight[10]={15,8,8,2,15};
  int ans=0;
  int now_piece=0,oppo_piece=0;

  auto self_board=this->board.board[who_am_I];//自己的板永遠是固定的
  auto oppo_board=this->board.board[1-who_am_I];
  
  for(int i=0;i<BOARD_H;i++){
    for(int j=0;j<BOARD_W;j++){

      if(self_board[i][j]){//加自己的
        now_piece=self_board[i][j];
        ans+=chess_value[now_piece]*weight[0];
        

        if(who_am_I){
          if(i == 2 || i== 3){
            ans+=5;
          }
          if(i ==4 || i == 5){
            ans+=10;
          }
          if(now_piece == 1){
            if(i>1){
              ans+=chess_value[now_piece]*(i+3);
            }
          }
        }
        else{
          if(i == 2 || i== 3){
            ans+=5;
          }
          if(i == 0 || i == 1){
            ans+=10;
          }
          if(now_piece == 1){
            if(i>=0 && i<=3){
              ans+=chess_value[now_piece]*(9-i);
            }
          }
        }

      }
      else if(oppo_board[i][j]){//扣別人的
        oppo_piece=oppo_board[i][j];
        ans-=chess_value[oppo_piece]*weight[4];
        
      }  
        
        /*if(my_turn){//我的turn
          switch(now_piece){//判斷能不能吃掉別人
            case 1://pawn
              if(who_am_I){//black
                if(i < BOARD_H-1){
                  if(j < BOARD_W-1 && (oppo_piece=oppo_board[i+1][j+1])){
                    ans+=chess_value[oppo_piece]*weight[1];
                    
                  }
                  if(j > 0 && (oppo_piece=oppo_board[i+1][j-1])){
                    ans+=chess_value[oppo_piece]*weight[1];
                  }
                }
              }
              else{//white
                if(i > 0){
                  if(j < BOARD_W-1 && (oppo_piece=oppo_board[i-1][j+1])){
                    ans+=chess_value[oppo_piece]*weight[1];
                  }
                  if(j > 0 && (oppo_piece=oppo_board[i-1][j-1])){
                    ans+=chess_value[oppo_piece]*weight[1];
                  }
                }
              }
              break;
            case 2://rook
            case 4://bishop
            case 5://queen
              int start,end;

              switch(now_piece){
                case 2:
                  start=0; end=4;
                  break;
                case 4:
                  start=4; end=8;
                  break;
                case 5:
                  start=0; end=8;
                  break;
                default:
                  start=0; end=-1;
                  break;
              }
              for(int part=start;part<end;part++){
                for(int block=0;block<6;block++){
                  int target[2]={move_table_rook_bishop[part][block][0]+i,move_table_rook_bishop[part][block][1]+j};

                  if(target[0] >= BOARD_W || target[0] < 0 || target[1] >= BOARD_H || target[1] < 0) break;//出界
                  if(self_board[target[0]][target[1]]) break;//被友軍擋住

                  if(oppo_board[target[0]][target[1]]){//可以吃別人
                    oppo_piece=oppo_board[target[0]][target[1]];
                    ans+=chess_value[oppo_piece]*weight[1];
                    
                    break;
                  }
                }
              }
              break;
            case 3:
              for(int block=0;block<8;block++){
                int target[2]={move_table_knight[block][0]+i,move_table_knight[block][1]+j};

                if(target[0] >= BOARD_W || target[0] < 0 || target[1] >= BOARD_H || target[1] < 0) continue;;//出界
                if(self_board[target[0]][target[1]]) continue;;//被友軍擋住

                if(oppo_board[target[0]][target[1]]){//可以吃別人
                  oppo_piece=oppo_board[target[0]][target[1]];
                  ans+=chess_value[oppo_piece]*weight[1];
                }
              }
              break;
            case 6:
              for(int block=0;block<8;block++){
                
                int target[2]={move_table_king[block][0]+i,move_table_king[block][1]+j};

                if(target[0] >= BOARD_W || target[0] < 0 || target[1] >= BOARD_H || target[1] < 0) continue;;//出界
                if(self_board[target[0]][target[1]]) continue;;//被友軍擋住

                if(oppo_board[target[0]][target[1]]){//可以吃別人
                  oppo_piece=oppo_board[target[0]][target[1]];
                  ans+=chess_value[oppo_piece]*weight[1];
                }
              }
              break;
          }
        }
        else if(!my_turn){//別人的turn
          oppo_piece=oppo_board[i][j];
          switch(oppo_piece){//判斷我會不會被別人吃掉
            case 1://pawn
              if(!who_am_I){//black
                if(i < BOARD_H-1){
                  if(j < BOARD_W-1 && (now_piece=self_board[i+1][j+1])){
                    ans-=chess_value[now_piece]*weight[2];
                    
                  }
                  if(j > 0 && (now_piece=self_board[i+1][j-1])){
                    ans-=chess_value[now_piece]*weight[2];
                  }
                }
              }
              else{//white
                if(i > 0){
                  if(j < BOARD_W-1 && (now_piece=self_board[i-1][j+1])){
                    ans-=chess_value[now_piece]*weight[2];
                  }
                  if(j > 0 && (now_piece=self_board[i-1][j-1])){
                    ans-=chess_value[now_piece]*weight[2];
                  }
                }
              }
              break;
            case 2://rook
            case 4://bishop
            case 5://queen
              int start,end;

              switch(now_piece){
                case 2:
                  start=0; end=4;
                  break;
                case 4:
                  start=4; end=8;
                  break;
                case 5:
                  start=0; end=8;
                  break;
                default:
                  start=0; end=-1;
                  break;
              }
              for(int part=start;part<end;part++){
                for(int block=0;block<6;block++){
                  int target[2]={move_table_rook_bishop[part][block][0]+i,move_table_rook_bishop[part][block][1]+j};

                  if(target[0] >= BOARD_W || target[0] < 0 || target[1] >= BOARD_H || target[1] < 0) break;//出界
                  if(oppo_board[target[0]][target[1]]) break;//被友軍擋住

                  if(self_board[target[0]][target[1]]){//我會被他吃
                    now_piece=self_board[target[0]][target[1]];
                    ans-=chess_value[now_piece]*weight[2];
                    break;
                  }
                }
              }
            break;
            case 3:
              for(int block=0;block<8;block++){
                
                int target[2]={move_table_knight[block][0]+i,move_table_knight[block][1]+j};

                if(target[0] >= BOARD_W || target[0] < 0 || target[1] >= BOARD_H || target[1] < 0) continue;;//出界
                if(oppo_board[target[0]][target[1]]) continue;;//被友軍擋住

                if(self_board[target[0]][target[1]]){//我會被他吃
                  now_piece=self_board[target[0]][target[1]];
                  ans-=chess_value[now_piece]*weight[2];
                }
              }
              break;
            case 6:
              for(int block=0;block<8;block++){
                int target[2]={move_table_king[block][0]+i,move_table_king[block][1]+j};

                if(target[0] >= BOARD_W || target[0] < 0 || target[1] >= BOARD_H || target[1] < 0) continue;;//出界
                if(oppo_board[target[0]][target[1]]) continue;;//被友軍擋住

                if(self_board[target[0]][target[1]]){//我會被牠吃
                  now_piece=self_board[target[0]][target[1]];
                  ans-=chess_value[now_piece]*weight[2];
                }
              }
              break;
          }
        }*/
    }
  }
  cout<<ans<<"#######"<<"\n\n\n\n\n\n\n";
  return ans;
  //return 0;
}


/**
 * @brief return next state after the move
 * 
 * @param move 
 * @return State* 
 */
State* State::next_state(Move move){
  Board next = this->board;
  Point from = move.first, to = move.second;
  
  int8_t moved = next.board[this->player][from.first][from.second];
  //promotion for pawn
  if(moved == 1 && (to.first==BOARD_H-1 || to.first==0)){
    moved = 5;
  }
  if(next.board[1-this->player][to.first][to.second]){//吃掉對方的子
    next.board[1-this->player][to.first][to.second] = 0;
  }
  
  next.board[this->player][from.first][from.second] = 0;
  next.board[this->player][to.first][to.second] = moved;
  
  State* next_state = new State(next, 1-this->player);//只需要更新對方的版面
  
  if(this->game_state != WIN)
    next_state->get_legal_actions();
  return next_state;
}




/**
 * @brief get all legal actions of now state
 * 
 */
void State::get_legal_actions(){
  // [Optional]
  // This method is not very efficient
  // You can redesign it
  this->game_state = NONE;
  std::vector<Move> all_actions;
  auto self_board = this->board.board[this->player];//分派黑白兩個棋盤給兩個board
  auto oppn_board = this->board.board[1 - this->player];
  
  int now_piece, oppn_piece;
  for(int i=0; i<BOARD_H; i+=1){
    for(int j=0; j<BOARD_W; j+=1){
      if((now_piece=self_board[i][j])){
        // std::cout << this->player << "," << now_piece << ' ';
        switch (now_piece){
          case 1: //pawn
            if(this->player && i<BOARD_H-1){
              //black
              if(!oppn_board[i+1][j] && !self_board[i+1][j])//往前走
                all_actions.push_back(Move(Point(i, j), Point(i+1, j)));
              if(j<BOARD_W-1 && (oppn_piece=oppn_board[i+1][j+1])>0){//斜右下吃子
                all_actions.push_back(Move(Point(i, j), Point(i+1, j+1)));
                if(oppn_piece==6){
                  this->game_state = WIN;
                  this->legal_actions = all_actions;
                  return;
                }
              }
              if(j>0 && (oppn_piece=oppn_board[i+1][j-1])>0){//斜左下吃子
                all_actions.push_back(Move(Point(i, j), Point(i+1, j-1)));
                if(oppn_piece==6){
                  this->game_state = WIN;
                  this->legal_actions = all_actions;
                  return;
                }
              }
            }else if(!this->player && i>0){
              //white
              if(!oppn_board[i-1][j] && !self_board[i-1][j])
                all_actions.push_back(Move(Point(i, j), Point(i-1, j)));
              if(j<BOARD_W-1 && (oppn_piece=oppn_board[i-1][j+1])>0){
                all_actions.push_back(Move(Point(i, j), Point(i-1, j+1)));
                if(oppn_piece==6){
                  this->game_state = WIN;
                  this->legal_actions = all_actions;
                  return;
                }
              }
              if(j>0 && (oppn_piece=oppn_board[i-1][j-1])>0){
                all_actions.push_back(Move(Point(i, j), Point(i-1, j-1)));
                if(oppn_piece==6){
                  this->game_state = WIN;
                  this->legal_actions = all_actions;
                  return;
                }
              }
            }
            break;
          
          case 2: //rook
          case 4: //bishop
          case 5: //queen
            int st, end;
            switch (now_piece){
              case 2: st=0; end=4; break; //rook
              case 4: st=4; end=8; break; //bishop
              case 5: st=0; end=8; break; //queen
              default: st=0; end=-1;
            }
            for(int part=st; part<end; part+=1){
              auto move_list = move_table_rook_bishop[part];
              for(int k=0; k<std::max(BOARD_H, BOARD_W); k+=1){
                int p[2] = {move_list[k][0] + i, move_list[k][1] + j};
                
                if(p[0]>=BOARD_H || p[0]<0 || p[1]>=BOARD_W || p[1]<0) break;
                now_piece = self_board[p[0]][p[1]];
                if(now_piece) break;//那個位置有友軍
                
                all_actions.push_back(Move(Point(i, j), Point(p[0], p[1])));
                
                oppn_piece = oppn_board[p[0]][p[1]];
                if(oppn_piece){
                  if(oppn_piece==6){
                    this->game_state = WIN;
                    this->legal_actions = all_actions;
                    return;
                  }else
                    break;
                };
              }
            }
            break;
          
          case 3: //knight
            for(auto move: move_table_knight){
              int x = move[0] + i;
              int y = move[1] + j;
              
              if(x>=BOARD_H || x<0 || y>=BOARD_W || y<0) continue;
              now_piece = self_board[x][y];
              if(now_piece) continue;
              all_actions.push_back(Move(Point(i, j), Point(x, y)));
              
              oppn_piece = oppn_board[x][y];
              if(oppn_piece==6){
                this->game_state = WIN;
                this->legal_actions = all_actions;
                return;
              }
            }
            break;
          
          case 6: //king
            for(auto move: move_table_king){
              int p[2] = {move[0] + i, move[1] + j};
              
              if(p[0]>=BOARD_H || p[0]<0 || p[1]>=BOARD_W || p[1]<0) continue;
              now_piece = self_board[p[0]][p[1]];
              if(now_piece) continue;
              
              all_actions.push_back(Move(Point(i, j), Point(p[0], p[1])));
              
              oppn_piece = oppn_board[p[0]][p[1]];
              if(oppn_piece==6){
                this->game_state = WIN;
                this->legal_actions = all_actions;
                return;
              }
            }
            break;
        }
      }
    }
  }
  std::cout << "\n";
  this->legal_actions = all_actions;
}


/*const char piece_table[2][7][5] = {
  {" ", "♙", "♖", "♘", "♗", "♕", "♔"},
  {" ", "♟", "♜", "♞", "♝", "♛", "♚"}
};*/
/**
 * @brief encode the output for command line output
 * 
 * @return std::string 
 */
std::string State::encode_output(){
  std::stringstream ss;
  int now_piece;
  for(int i=0; i<BOARD_H; i+=1){
    for(int j=0; j<BOARD_W; j+=1){
      if((now_piece = this->board.board[0][i][j])){
        ss << std::string(piece_table[0][now_piece]);
      }else if((now_piece = this->board.board[1][i][j])){
        ss << std::string(piece_table[1][now_piece]);
      }else{
        ss << " ";
      }
      ss << " ";
    }
    ss << "\n";
  }
  return ss.str();
}


/**
 * @brief encode the state to the format for player
 * 
 * @return std::string 
 */
std::string State::encode_state(){
  std::stringstream ss;
  ss << this->player;
  ss << "\n";
  for(int pl=0; pl<2; pl+=1){
    for(int i=0; i<BOARD_H; i+=1){
      for(int j=0; j<BOARD_W; j+=1){
        ss << int(this->board.board[pl][i][j]);
        ss << " ";
      }
      ss << "\n";
    }
    ss << "\n";
  }
  return ss.str();
}