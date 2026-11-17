// board.c
//all the board stuff lives here: printing, drawing lines, checking boxes

#include <stdio.h>
#include "board.h"

void board_init(Board *b)
{
    int r, c;
    //starts with nothing drawn
    for (r = 0; r < DOT_ROWS; r++)
        for (c = 0; c < BOX_COLS; c++)
            b->hlines[r][c] = 0;

    for (r = 0; r < BOX_ROWS; r++)
        for (c = 0; c < DOT_COLS; c++)
            b->vlines[r][c] = 0;

    //space = nobody owns it yet
    for (r = 0; r < BOX_ROWS; r++)
        for (c = 0; c < BOX_COLS; c++)
            b->boxes[r][c] = ' ';
}

//tried to make it look like the sample output in the pdf
void board_print(const Board *b)
{
    int r, c;

    printf("  ");
    for (c = 0; c < DOT_COLS; c++)
        printf("%d ", c);
    printf("\n");

    for (r = 0; r < DOT_ROWS; r++) {
        printf("%d ", r);
        for (c = 0; c < DOT_COLS; c++) {
            printf(".");
            if (c < BOX_COLS)  //last dot in the row has nothing after it
                printf("%c", b->hlines[r][c] ? '-' : ' ');
        }
        printf("\n");

        //in between two dot rows we print the | lines and whoever owns the box
        //bottom row doesnt need this since there are no boxes under it
        if (r < BOX_ROWS) {
            printf("  ");
            for (c = 0; c < DOT_COLS; c++) {
                printf("%c", b->vlines[r][c] ? '|' : ' ');
                if (c < BOX_COLS)
                    printf("%c", b->boxes[r][c]);
            }
            printf("\n");
        }
    }
}

//all 4 sides there? then its a box
static int box_complete(const Board *b, int r, int c)
{
    return b->hlines[r][c] && b->hlines[r + 1][c] &&
           b->vlines[r][c] && b->vlines[r][c + 1];
}

//gives box (r,c) to the player if its closed and still free
//tthe bound check is needed bc lines on the edge only have one box next to them
static int try_claim(Board *b, int r, int c, char player)
{
    if (r < 0 || r >= BOX_ROWS || c < 0 || c >= BOX_COLS)
        return 0;
    if (b->boxes[r][c] != ' ')
        return 0;   //someone already has it
    if (!box_complete(b, r, c))
        return 0;

    b->boxes[r][c] = player;
    return 1;
}

static int in_range(int r, int c)
{
    return r >= 0 && r < DOT_ROWS && c >= 0 && c < DOT_COLS;
}

MoveResult board_draw_line(Board *b, int r1, int c1, int r2, int c2,
                           char player, int *completed)
{
    int tmp;
    *completed = 0;

    if (!in_range(r1, c1) || !in_range(r2, c2))
        return MOVE_OUT_OF_RANGE;

    //people can type the dots in any order, so swap them so the first one is always top/left, makes the rest simpler
    if (r2 < r1 || (r1 == r2 && c2 < c1)) {
        tmp = r1; r1 = r2; r2 = tmp;
        tmp = c1; c1 = c2; c2 = tmp;
    }

    if (r1 == r2 && c2 == c1 + 1) {
        if (b->hlines[r1][c1])
            return MOVE_ALREADY_DRAWN;
        b->hlines[r1][c1] = 1;

        //horizontal line -> only the box above and the box below can close
        //no point checking the whole board, so this part is O(1)
        *completed += try_claim(b, r1 - 1, c1, player);
        *completed += try_claim(b, r1, c1, player);
    }
    else if (c1 == c2 && r2 == r1 + 1) {
        if (b->vlines[r1][c1])
            return MOVE_ALREADY_DRAWN;
        b->vlines[r1][c1] = 1;

        //same thing for vertical, left box and right box
        *completed += try_claim(b, r1, c1 - 1, player);
        *completed += try_claim(b, r1, c1, player);
    }
    else {
        //diagonal, same dot twice, or dots too far apart
        return MOVE_NOT_ADJACENT;
    }

    return MOVE_OK;
}

int board_count(const Board *b, char player)
{
    int r, c, n = 0;
    for (r = 0; r < BOX_ROWS; r++)
        for (c = 0; c < BOX_COLS; c++)
            if (b->boxes[r][c] == player)
                n++;
    return n;
}

//game's over when A + B own all 20
int board_is_full(const Board *b)
{
    return board_count(b, 'A') + board_count(b, 'B') == TOTAL_BOXES;
}