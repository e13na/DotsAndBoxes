//game.c
//this is where the actual game happens: turns, reading moves, scores, winner

#include <stdio.h>
#include "game.h"

#define STARS "*************************************************************************"

void game_init(Game *g)
{
    board_init(&g->board);
    g->current = 'A';   //A always goes first
    g->score[0] = 0;
    g->score[1] = 0;
}

//throws away whatever is left on the line the user typrd
//we need this bc if someone types letters, scanf leaves them there
//and wed keep reading the same junk forever
//returns 0 if we hit the end of input
static int skip_line(void)
{
    int ch;
    while ((ch = getchar()) != '\n')
        if (ch == EOF)
            return 0;
    return 1;
}

//reads the 4 numbers of a move (row col row col)
//1 = got the 4 numbers, 0 = typed something wrong, -1 = input ended
static int read_move(int *r1, int *c1, int *r2, int *c2)
{
    int n = scanf("%d %d %d %d", r1, c1, r2, c2);

    if (n == EOF)
        return -1;
    if (n != 4) {
        //not 4 numbers, clear the line so they can try again
        if (!skip_line())
            return -1;
        return 0;
    }
    skip_line();//ignore anything extra they typed after the 4 numbers
    return 1;
}

//same score box as in the pdf screenshots
static void print_scores(const Game *g)
{
    printf("%s\n", STARS);
    printf("Player A score: %d\n", g->score[0]);
    printf("Player B score: %d\n", g->score[1]);
    printf("%s\n", STARS);
}

static void print_winner(const Game *g)
{
    printf("\n%s\nGAME OVER\n", STARS);
    printf("Player A: %d boxes\n", g->score[0]);
    printf("Player B: %d boxes\n", g->score[1]);

    if (g->score[0] > g->score[1])
        printf("Player A wins!\n");
    else if (g->score[1] > g->score[0])
        printf("Player B wins!\n");
    else
        printf("It's a tie!\n"); //10-10 is possible since there are 20 boxes

    printf("%s\n", STARS);
}

int game_run(Game *g)
{
    int r1, c1, r2, c2, completed, status;
    MoveResult res;

    board_print(&g->board);

//keep going until every box has an owner
    while (!board_is_full(&g->board)) {
        printf("Player %c's turn. Enter the row and column of the first dot "
               "(e.g., A0 -> 0 0) and second dot:\n", g->current);
        fflush(stdout);

        status = read_move(&r1, &c1, &r2, &c2);
        if (status == -1) {
            printf("\nInput ended. Exiting game.\n");
            return 1;
        }
        if (status == 0) {
            printf("Invalid input. Please enter 4 numbers, e.g. 0 0 0 1\n");
            continue;  //same player tries again
        }

        res = board_draw_line(&g->board, r1, c1, r2, c2, g->current, &completed);

//for any bad move we print why and let the same player go again
        if (res == MOVE_OUT_OF_RANGE) {
            printf("Invalid move: rows must be 0-%d and columns 0-%d.\n",
                   DOT_ROWS - 1, DOT_COLS - 1);
            continue;
        }
        if (res == MOVE_NOT_ADJACENT) {
            printf("Invalid move: the two dots must be next to each other.\n");
            continue;
        }
        if (res == MOVE_ALREADY_DRAWN) {
            printf("Invalid move: that line is already drawn.\n");
            continue;
        }

//completed is 0, 1 or 2, so we just add it to whoever played
        if (g->current == 'A')
            g->score[0] += completed;
        else
            g->score[1] += completed;

        print_scores(g);
        board_print(&g->board);

//if you close a box you play again otherwise its the other player's turn
        if (completed == 0)
            g->current = (g->current == 'A') ? 'B' : 'A';
    }

    print_winner(g);
    return 0;
}