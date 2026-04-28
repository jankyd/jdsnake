#include <stdlib.h>
#include <ncurses.h>
#include <iostream>
#include <deque>
#include <random>

using namespace std;

#define WIN_HEIGHT 12
#define WIN_WIDTH 30
// Centers play window
#define WIN_Y (LINES - WIN_HEIGHT)/2
#define WIN_X (COLS - WIN_WIDTH)/2
#define SNAKE_CHAR '#'
#define FOOD_CHAR '$'

/* TODO:

Add keyboard input handling
Add snake/food rendering
Point handling
Game over handling
Restart vs quit

*/
// ! Global Variables !
bool GAME_OVER = false;
int points = 0;
struct food *f;

// randomizer for food placement
random_device rd;
mt19937 gen(rd());
uniform_int_distribution<> distX(1, WIN_WIDTH-1); // for X ranges
uniform_int_distribution<> distY(1,WIN_HEIGHT-1); // for Y range

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
    mvwprintw(win, 0, 1, "* Snake *");
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

// Set up other game parameters (points -> 0, first food placement)
void initGame() {
    points = 0;
    f = new struct food();
    while (true) {
        f->x = distX(gen);
        f->y = distY(gen);
        // ensure it doesn't generate on the snake head
        if (!(f->x == 4 && f->y == WIN_HEIGHT / 2)) break;
    }

    return;
}

/**
 * Handle updating the snake data (POST MOVE INPUT!)
 */
void movSnake(snake *s) {
    // determine movement case
    switch (s->currDir) {
        case DRIGHT:
            s->x += 1;
            break;
        case DLEFT:
            s->x -= 1;
            break;
        case DUP:
            s->y -= 1;
            break;
        case DDOWN:
            s->y += 1;
            break;
    }
    // move the head
    s->snakeQueue.push_front(new struct snakeNode(s->x,s->y));
    // if snake did not eat, pop the last snakeNode, else leave it (grows)
    if (!(s->y == f->y && s->x == f->y)) {
        s->snakeQueue.pop_back();
    }
    else points += 1;
    return;
}

/**
 * Called on each game 'turn' to update the screen.
 * Redraws the entire game bounds with snake and food placements
 */
void updateGameWindow(WINDOW *win, snake *s) {
    wclear(win);
    box(win,0,0);
    mvwprintw(win, 0, 1, "* Snake Points: %d *", points);

}


int main() {
    WINDOW *win = displayInit();
    struct snake *s = initSnake(win);
    initGame();
    wrefresh(win);
    getch(); // pause for debugging
    
    endwin();
    return 0;

}