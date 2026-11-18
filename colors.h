//colors.h
//colors for the game using the 256 color codes
//RESET puts it back to normal

#ifndef COLORS_H
#define COLORS_H

#define RESET   "\033[0m"
#define BOLD    "\033[1m"
#define RED     "\033[38;5;218m" //pastel pink, player A
#define BLUE    "\033[38;5;153m"  //baby blue, player B
#define GREEN   "\033[38;5;158m"  //mint, for the winner
#define YELLOW  "\033[38;5;229m" //butter yellow, warnings
#define CYAN    "\033[38;5;183m" //lavender, row and column numbers

//picks the right color for a player so we dont repeat this if everywhere
#define PLAYER_COLOR(p) ((p) == 'A' ? RED : BLUE)

#endif