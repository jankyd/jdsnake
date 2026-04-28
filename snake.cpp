#include <stdlib.h>
#include <ncurses.h>
#include <iostream>
#include <deque>

using namespace std;

#define WIN_HEIGHT 12
#define WIN_WIDTH 30
#define WIN_Y (LINES - WIN_HEIGHT)/2
#define WIN_X (COLS - WIN_WIDTH)/2
#define SNAKE_CHAR '#'
#define FOOD_CHAR '$'


// ! Global Variables !
bool GAME_OVER = false;
int points = 0;

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

/**
 * Uses a deque to store 
 */
struct snakeNode {
    int x;
    int y;
    snakeNode() : x(0), y(0) {}
    snakeNode(int dx, int dy) : x(dx), y(dy) {}
};

struct snake {
    // coords of the head
    int x;
    int y;

    int length;
    std::deque<snakeNode*> snakeQueue;
    Direction currDir = DRIGHT;
};

/**
 * Returns a window object that the game will be played in
 */
WINDOW* displayInit() {
    
    initscr();
    clear();
    noecho();
    cbreak();
    //timeout(20);
    refresh();
    WINDOW *win = newwin(WIN_HEIGHT, WIN_WIDTH, WIN_Y, WIN_X);
    curs_set(0);
    box(win, 0, 0);
    mvwprintw(win, 0, 1, "* Snake Game *");
    wrefresh(win);
    return win;
}

snake* initSnake(WINDOW *win) {
    snake *snake = new struct snake();
    snake->y = WIN_HEIGHT / 2;
    snake->x = 4;
    // place the head
    snake->snakeQueue.push_front(new struct snakeNode(snake->x, snake->y));
    snake->length = 1;
    return snake;
}

/**
 * Handle updating the snake data (POST MOVE INPUT!)
 * Does NOT handle food overlap (yet?)
 */
void movSnake(snake *s) {
    switch (s->currDir) {
        case DRIGHT:

            s->snakeQueue.push_front(new struct snakeNode())
    }
}

int main() {
    WINDOW *win = displayInit();
    wrefresh(win);
    getch(); // pause for debugging
    
    
    
    
    endwin();
    return 0;

}