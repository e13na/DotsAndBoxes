// board.h
//basically the "plan" for the board. the struct + the list of functions,
//the actual code for them is in board.c

#ifndef BOARD_H
#define BOARD_H   //so it doesnt get included twice and break everything

//4x5 boxes
#define BOX_ROWS 4
#define BOX_COLS 5

//theres always one more dot than boxes 
#define DOT_ROWS (BOX_ROWS + 1)
#define DOT_COLS (BOX_COLS + 1)

#define TOTAL_BOXES (BOX_ROWS * BOX_COLS)   //20, when all are taken the game ends

typedef struct {
    //the lines going across. hlines[r][c] is between dot (r,c) and dot (r,c+1)
    //0 if nobody drew it yet, 1 if someone did
    int hlines[DOT_ROWS][BOX_COLS];

    //lines going down, from (r,c) to (r+1,c)
    int vlines[BOX_ROWS][DOT_COLS];

    //who has each box. space means no one yet
    char boxes[BOX_ROWS][BOX_COLS];
} Board;

//used this instead of returning 0/1/2/3 so we know what went wrong
typedef enum {
    MOVE_OK,
    MOVE_OUT_OF_RANGE, //dot isnt even on the board
    MOVE_NOT_ADJACENT,  //dots arent next to each other
    MOVE_ALREADY_DRAWN
} MoveResult;

void board_init(Board *b);
void board_print(const Board *b);

//draws the line if the move is ok
//completed tells you how many boxes that line closed, could be 0 1 or 2
MoveResult board_draw_line(Board *b, int r1, int c1, int r2, int c2,
                           char player, int *completed);

int board_count(const Board *b, char player);
int board_is_full(const Board *b);  //1 when all boxes are taken

#endif