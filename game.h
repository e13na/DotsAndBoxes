//game.h
//keeps track of the match: the board, whose turn it is, and the scores

#ifndef GAME_H
#define GAME_H

#include "board.h"  //we need the Board type in here

typedef struct {
    Board board;
    char current; //whose turn it is right now, 'A' or 'B'
    int score[2];   //score[0] is A, score[1] is B
} Game;

void game_init(Game *g); //fresh board, A starts, both scores at 0
//the main loop of the game, keeps going until all boxes are taken. returns 0 if the game finished normally, 1 if the input ran out early
int game_run(Game *g);

#endif