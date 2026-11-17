# DotsAndBoxes
CMPS 241 course project – Dots and Boxes in C

Team: Elena Neemtallah, Charbel Peter El Sebaaly

## Sprint 1 – two player game
Two players (A and B) take turns drawing lines between dots on a 4x5 grid of boxes.
If your line closes a box, the box gets your letter and you play again.
When all 20 boxes are taken, whoever has more boxes wins (a 10-10 tie is possible).

## How to build and run
```
make
./dotsandboxes
```
`make clean` deletes the compiled files.

## How to play
Type the row and column of the first dot, then the row and column of the second dot, all separated by spaces.
For example `0 3 0 4` draws the line between dot (0,3) and dot (0,4).

The game rejects:
- anything that isn't exactly 4 numbers
- dots that aren't on the board
- dots that aren't next to each other (diagonals, same dot, too far)
- lines that are already drawn

## Files
| File | What it does |
|---|---|
| `board.h` / `board.c` | the board: printing it, drawing lines, checking if a box got closed |
| `game.h` / `game.c` | the game: turns, reading moves, scores, winner |
| `main.c` | starts the game |
| `Makefile` | builds everything |
| `test_tie.txt`, `test_a_wins.txt`, `test_b_wins.txt` | full games we use for testing |

## Complexity
When a line is drawn, it can only close the 2 boxes touching it, so we only check those 2 boxes (4 sides each) instead of the whole board.
That makes checking for a completed box **O(1)**.

## Testing
We play full games automatically by feeding a test file to the program:
```
./dotsandboxes < test_tie.txt
./dotsandboxes < test_a_wins.txt
./dotsandboxes < test_b_wins.txt
```