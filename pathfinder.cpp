#include <iostream>
#include <string>

#include "API.h"


const int MAZE_SIZE = 16;

// direction
char dir_chars[4] = {'n', 'e', 's', 'w'};

// allows you to use bitwise OR when updating which walls are present in a cell
// also allows you to bitwise AND
int dir_mask[4] = {0b1000, 0b0100, 0b0010, 0b0001};

enum Direction {
    NORTH = 0,
    EAST = 1,
    SOUTH = 2,
    WEST = 3
};

enum DirectionBitmask {
    NORTH_MASK = 0b1000,
    EAST_MASK  = 0b0100,
    SOUTH_MASK = 0b0010,
    WEST_MASK  = 0b0001
};

struct Coord {
    int x;
    int y;
};

struct Cell {
    Coord pos;
    Direction dir;
};

struct CellList {
    int size;
    Cell* cells;
};

struct Queue {
    int head;
    int tail;
    // Cell object array with max size
    // Two int objects for head and tail
};

struct Maze {
    Coord mouse_pos;
    Direction mouse_dir;

    // 2, 2D arrays for distances of each cell, and values for cell walls
    int cellWalls[MAZE_SIZE][MAZE_SIZE];
    int distances[MAZE_SIZE][MAZE_SIZE];

    Coord* goalPos;
};

/*

// Queue functions
void initQueue(Queue *q) { //initalize empty queue
    
}

bool isQEmpty(Queue q) {
    
}




// Maze functions
void scanWalls(Maze* maze) { // fill in code for changing value of the cell walls
    if (API::wallFront()) {
        
    }
    if (API::wallRight()) {
        
    }
    if (API::wallLeft()) {
        
    }
}

*/

void updateSimulator(Maze maze) { // redraws the maze in simulator after each loop in main
    for(int x = 0; x < MAZE_SIZE; x++) 
    {
        for(int y = 0; y < MAZE_SIZE; y++) 
        {
            if (maze.cellWalls[y][x] & NORTH_MASK)
                // API set walls for NORTH
                API::setWall(x, y, dir_chars[NORTH]);

            if (maze.cellWalls[y][x] & EAST_MASK)
                // API set walls for EAST
                API::setWall(x, y, dir_chars[EAST]);

            if (maze.cellWalls[y][x] & SOUTH_MASK)
                // API set walls for SOUTH
                API::setWall(x, y, dir_chars[SOUTH]);

            if (maze.cellWalls[y][x] & WEST_MASK)
                // API set walls for WEST
                API::setWall(x, y, dir_chars[WEST]);
        }
    }
}

/*
void updateMousePos(Coord* pos, Direction dir) {
    // depending on the mouse direction, increment position by one
    if (dir == NORTH)
        // increment in some direction
    if (dir == SOUTH)
        // increment in some direction
    if (dir == WEST)
        // increment in some direction
    if (dir == EAST)
        // increment in some direction
}

CellList* getNeighborCells(Maze* maze, Coord c) { //to be called in a while loop within Floodfill when setting each cell
    
};

void floodFill(Maze* maze, bool to_start) { // function to be called everytime you move into a new cell
    
}

*/


void log(const std::string& text) {
    std::cerr << text << std::endl;
}

int main(int argc, char* argv[]) {
    log("Running...");
    API::setColor(0, 0, 'G');
    API::setText(0, 0, "START");

    Maze testMaze;

    for (int i = 0; i < MAZE_SIZE; i++) {
        for (int j = 0; j < MAZE_SIZE; j++) {
            testMaze.cellWalls[i][j] = 0;
        }
    }

    testMaze.cellWalls[15][2] = 0b001;

    updateSimulator(testMaze);

    while (true) {
        if (!API::wallLeft()) {
            API::turnLeft();
        }
        while (API::wallFront()) {
            API::turnRight();
        }
        API::moveForward();
    }
}
