#include <stdlib.h>
#include <ncurses.h>
#include <iostream>

using namespace std;

#define WIN_HEIGHT 10
#define WIN_WIDTH 25
#define WIN_Y (LINES - WIN_HEIGHT)/2
#define WIN_X (COLS - WIN_WIDTH)/2

enum Direction {
    DUP,
    DDOWN,
    DRIGHT,
    DLEFT,
};

struct food {
    int x;
    int y;
};

struct snakeNode {
    int x;
    int y;
    struct snakeNode *link;
};

struct snake {
    int x;
    int y;
    int length;
    struct snakeNode *head;
    Direction currDir = DRIGHT;
};

/**
 * Returns a window object that the game will be played in
 */
WINDOW* displayInit() {
    
    initscr();
    noecho();
    curs_set(0);
    cbreak();
    timeout(20);
    WINDOW *win = newwin(WIN_HEIGHT, WIN_WIDTH, WIN_Y, WIN_X);
    return win;
}

int main() {
    initscr();
    
    WINDOW *win = newwin(WIN_HEIGHT, WIN_WIDTH, WIN_Y, WIN_X);
    curs_set(0);
    clear();
    refresh();
    box(win, 0, 0);

    mvwprintw(win, 0, 1,"Centered Window!");
    wrefresh(win);
    getch();
    
    endwin();
    return 0;

}