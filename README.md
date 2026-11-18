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

Player A is shown in pink, player B in blue, errors in yellow and the winner in green.

## Project structure
| File / folder | What it does |
|---|---|
| `src/board.h`, `src/board.c` | the board: printing it, drawing lines, checking if a box got closed |
| `src/game.h`, `src/game.c` | the game: turns, reading moves, scores, winner |
| `src/main.c` | starts the game |
| `src/colors.h` | the color codes used for the players, errors and winner |
| `tests/` | full games we use for testing (tie, A wins, B wins) |
| `Makefile` | builds everything, plus `make valgrind`, `make gdb`, `make install` |

## Complexity
When a line is drawn, it can only close the 2 boxes touching it, so we only check those 2 boxes (4 sides each) instead of the whole board.
That makes checking for a completed box **O(1)**.

## Testing
We play full games automatically by feeding a test file to the program:
```
./dotsandboxes < tests/tie.txt
./dotsandboxes < tests/a_wins.txt
./dotsandboxes < tests/b_wins.txt
```
Expected endings: tie (10-10), A wins (19-1), B wins (0-20).

## Debugging
We compile with `-g` so GDB and Valgrind can show function names and line numbers.

**Valgrind** (memory errors and leaks):
```
valgrind --leak-check=full ./dotsandboxes < tests/tie.txt
```
Result on all 3 test games and on a game full of invalid input: `All heap blocks were freed -- no leaks are possible` and `ERROR SUMMARY: 0 errors`.

**GDB** (stepping through a move):
```
gdb ./dotsandboxes
(gdb) break board_draw_line
(gdb) run
(gdb) print b->hlines[0]
(gdb) next
```
We used this to check that a move like `0 0 0 1` gets stored as `hlines[0][0] = 1` and that the right boxes get checked.

**Bug we found and fixed:** at first the game used `scanf` to read the 4 numbers, so if someone typed fewer than 4 (like `9999`) it just sat there waiting forever.
We switched to reading the whole line with `fgets` and then `sscanf`, so any wrong input now gets an error message right away.

## Running on Alpine Linux (boots straight into the game)
1. Installed Alpine (aarch64, virtual) in a UTM virtual machine with `setup-alpine`.
2. Installed the tools and the game:
```
apk add build-base git
git clone https://github.com/e13na/DotsAndBoxes.git
cd DotsAndBoxes
make
make install
```
3. Made the first console launch the game instead of the login prompt, in `/etc/inittab`:
```
tty1::respawn:/sbin/getty -n -l /usr/local/bin/dotsandboxes 38400 tty1
```
`-n` skips asking for a username, `-l` runs our game instead of the login program, and `respawn` starts a new game when one ends.

4. Rebooted, and the VM now opens directly on the game with no username or password.
