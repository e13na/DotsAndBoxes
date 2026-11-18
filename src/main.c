//main.c
//starting point of the program, all it does is set up a game and run it

#include <stdio.h>
#include "game.h"

int main(void)
{
    Game game;

    printf("=====================================\n");
    printf("     DOTS AND BOXES - 2 Players\n");
    printf("=====================================\n\n");

    game_init(&game);

    //game_run gives back 0 if the game actually finished
    if (game_run(&game) == 0) {
        //we wait for enter so the winner stays on the screen
        //on the alpine vm the game restarts by itself after this)
        printf("\nPress Enter to start a new game...");
        fflush(stdout);
        getchar();
    }

    return 0;
}