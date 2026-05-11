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
#define HEAD_UP '^'
#define HEAD_RIGHT '>'
#define HEAD_DOWN 'v'
#define HEAD_LEFT '<'

/* TODO:

Restart vs quit?

*/
// ! Global Variables !
bool GAME_OVER = false;
int points = 0;

// randomizer for food placement
random_device rd;
mt19937 gen(rd());
uniform_int_distribution<> distX(1, WIN_WIDTH-2); // for X ranges
uniform_int_distribution<> distY(1,WIN_HEIGHT-2); // for Y range

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

struct food f;
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
    std::deque<snakeNode> snakeQueue;
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
    timeout(1000); // sets getch() timeout to 1 second
    refresh();
    WINDOW *win = newwin(WIN_HEIGHT, WIN_WIDTH, WIN_Y, WIN_X);
    curs_set(0);
    keypad(stdscr, TRUE);
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
    snake->snakeQueue.push_front(snakeNode(snake->x, snake->y));
    snake->length = 1;
    return snake;
}

// Set up other game parameters (points -> 0, first food placement)
void initGame() {
    points = 0;
    while (true) {
        f.x = distX(gen);
        f.y = distY(gen);
        // ensure it doesn't generate on the snake head
        if (!(f.x == 4 && f.y == WIN_HEIGHT / 2)) break;
    }

    return;
}


/**
 * Called on each game 'turn' to update the screen.
 * Redraws the entire game bounds with snake and food placements
 * 
 * Runs AFTER all game logic has been updated
 */
void updateGameWindow(WINDOW *win, snake *s) {
    wclear(win);
    box(win,0,0);
    mvwprintw(win, 0, 1, "* Snake Points: %d *", points);
    // print snake
    for (int i = 0; i < s->length; i++) {
        // Print the head directionally
        if (i == 0) {
            char head;
            switch (s->currDir) {
                case DUP:
                    head = HEAD_UP;
                    break;
                case DDOWN:
                    head = HEAD_DOWN;
                    break;
                case DRIGHT:
                    head = HEAD_RIGHT;
                    break;
                case DLEFT:
                    head = HEAD_LEFT;
                    break;
            }
            mvwprintw(win, s->snakeQueue[i].y, s->snakeQueue[i].x, "%c", head);
            continue;    
        }
        // Print rest of body
        mvwprintw(win, s->snakeQueue[i].y, s->snakeQueue[i].x, "%c", SNAKE_CHAR);
    }
    // print food
    mvwprintw(win, f.y, f.x, "%c", FOOD_CHAR);
    // Print game over if Game is over :P
    if (GAME_OVER) {
        mvwprintw(win, (WIN_HEIGHT/2)-1, 4, "GAME OVER!");
        mvwprintw(win, (WIN_HEIGHT/2), 4, "Your Score: %d", points);
        wrefresh(win);
        timeout(-1);
        getch();
    }
    wrefresh(win);
    
    
}

void placeFood(snake *s) {
    bool placed = false;
    bool collision;
    while (!placed) {
        collision = false;
        f.x = distX(gen);
        f.y = distY(gen);
        
        for (int i = 0; i < s->length; i++) {
            if (f.x == s->snakeQueue[i].x && f.y == s->snakeQueue[i].y) {
                collision = true;
                break;
            }
        }
        if (collision) continue;
        else placed = true;
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
    // check for wall collision
    if (s->y == 0 || s->x == 0 || s->y == WIN_HEIGHT-1 || s->x == WIN_WIDTH-1) {
        GAME_OVER = true;
        return;
    }
    // check for collision with self
    for (int i = 0; i < s->length; i++) {
        if (s->x == s->snakeQueue[i].x && s->y == s->snakeQueue[i].y) {
            GAME_OVER = true;
            return;
        }
    }
    // move the head
    s->snakeQueue.push_front(snakeNode(s->x, s->y));
    // if snake did not eat, pop the last snakeNode, else leave it (grows)
    if (!(s->y == f.y && s->x == f.x)) {
        s->snakeQueue.pop_back();
    }
    else {
        points += 1;
        s->length += 1;
        placeFood(s);
    }
    return;
}

int main() {
    WINDOW *win = displayInit();
    struct snake *s = initSnake(win);
    initGame();
    updateGameWindow(win,s);
    
    int mv;
    
    // game loop
    while (true) {
        
        if (GAME_OVER) break;
        
        mv = getch();
        switch (mv) {
            case KEY_RIGHT:
                if (s->currDir == DLEFT) break; // prevent 180s
                s->currDir=DRIGHT;
                break;
            case KEY_LEFT:
                if (s->currDir == DRIGHT) break; // prevent 180s
                s->currDir=DLEFT;
                break;
            case KEY_UP:
                if (s->currDir == DDOWN) break; // prevent 180s
                s->currDir=DUP;
                break;
            case KEY_DOWN:
                if (s->currDir == DUP) break; // prevent 180s
                s->currDir=DDOWN;
                break;
        }

        movSnake(s);
        updateGameWindow(win, s);
    }

    nocbreak();
    endwin();
    delete s;
    std::cout << "Try and beat your score of " << points << " next time!" << std::endl;
    return 0;

}