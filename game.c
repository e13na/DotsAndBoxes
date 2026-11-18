//game.c
//this is where the actual game happens: turns, reading moves, scores, winner

#include <stdio.h>
#include "game.h"
#include "colors.h"

#define STARS "*************************************************************************"

void game_init(Game *g)
{
    board_init(&g->board);
    g->current = 'A';   //A always goes first
    g->score[0] = 0;
    g->score[1] = 0;
}

//reads one whole line and tries to get the 4 numbers of a move out of it(row col row col). we read the full line with fgets first so that if
//someone types too few numbers we can tell them right away instead of the program just sitting there waiting for more
//1 = got the 4 numbers, 0 = typed something wrong, -1 = input ended
static int read_move(int *r1, int *c1, int *r2, int *c2)
{
    char line[100];
    char extra;

    if (fgets(line, sizeof(line), stdin) == NULL)
        return -1;

    //the extra %c catches stuff after the 4 numbers like "0 0 0 1 5"
    if (sscanf(line, "%d %d %d %d %c", r1, c1, r2, c2, &extra) != 4)
        return 0;

    return 1;
}
//same score box as in the pdf screenshots
static void print_scores(const Game *g)
{
    printf("%s\n", STARS);
    printf(RED "Player A" RESET " score: %d\n", g->score[0]);
    printf(BLUE "Player B" RESET " score: %d\n", g->score[1]);
    printf("%s\n", STARS);
}

static void print_winner(const Game *g)
{
    printf("\n%s\n" BOLD "GAME OVER" RESET "\n", STARS);
    printf("Player A: %d boxes\n", g->score[0]);
    printf("Player B: %d boxes\n", g->score[1]);

    if (g->score[0] > g->score[1])
        printf(GREEN "Player A wins!" RESET "\n");
    else if (g->score[1] > g->score[0])
        printf(GREEN "Player B wins!" RESET "\n");
    else
        printf(GREEN "It's a tie!" RESET "\n");  //10-10 is possible since there are 20 boxes

    printf("%s\n", STARS);
}

int game_run(Game *g)
{
    int r1, c1, r2, c2, completed, status;
    MoveResult res;

    board_print(&g->board);

//keep going until every box has an owner
    while (!board_is_full(&g->board)) {
        printf("%sPlayer %c's turn." RESET " Enter the row and column of the first dot "
            "(e.g., A0 -> 0 0) and second dot:\n", PLAYER_COLOR(g->current), g->current);
        fflush(stdout);

        status = read_move(&r1, &c1, &r2, &c2);
        if (status == -1) {
            printf("\nInput ended. Exiting game.\n");
            return 1;
        }
        if (status == 0) {
            printf(YELLOW "Invalid input. Please enter exactly 4 numbers separated by spaces, e.g. 0 0 0 1" RESET "\n");
            continue;  //same player tries again
        }

        res = board_draw_line(&g->board, r1, c1, r2, c2, g->current, &completed);

//for any bad move we print why and let the same player go again
        if (res == MOVE_OUT_OF_RANGE) {
            printf(YELLOW "Invalid move: rows must be 0-%d and columns 0-%d." RESET "\n",
                   DOT_ROWS - 1, DOT_COLS - 1);
            continue;
        }
        if (res == MOVE_NOT_ADJACENT) {
            printf(YELLOW "Invalid move: the two dots must be next to each other." RESET "\n");
            continue;
        }
        if (res == MOVE_ALREADY_DRAWN) {
            printf(YELLOW "Invalid move: that line is already drawn." RESET "\n");
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