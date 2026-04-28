#include <ncurses.h>
#include <vector>

enum class Direction {
    kUp,
    kDown,
    kLeft,
    kRight,
    kNone,
};

struct Tile {
    int row = 0;
    int col = 0;
    Direction direction = Direction::kNone;
    
    friend bool operator==(const Tile& a, const Tile& b) {
        return ((a.row == b.row) && (a.col == b.col));
    }
};

using Snake = std::vector<Tile>;
using Targets = std::vector<Tile>;
Snake snake_;
Targets targets_;


void SnakeGame::SpawnSnake() {
    
}

