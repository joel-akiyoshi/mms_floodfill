#include <iostream>
#include <string>

#include "pathfinder.h"
#include "API.h"

// direction
char dir_chars[4] = {'n', 'e', 's', 'w'};

// allows you to use bitwise OR when updating which walls are present in a cell
// also allows you to bitwise AND
int dir_mask[4] = {0b1000, 0b0100, 0b0010, 0b0001};



// Queue functions
void initQueue(Queue *q) { //initalize empty queue
    // IMPLEMENT THIS
}

bool isQEmpty(Queue q) {
    // IMPLEMENT THIS
    return true;
}

// clockwise and counterclockwise step functions
Direction clockwiseStep(Direction initial_direction) {
    API::turnRight();
    return static_cast<Direction>((initial_direction + 1) % 4);
}

Direction counterClockwiseStep(Direction initial_direction) {
    API::turnLeft();
    return static_cast<Direction>((initial_direction - 1) % 4);
}


// Maze functions
void scanWalls(Maze* maze) { // fill in code for changing value of the cell walls
    Direction current_direction = maze->mouse_dir;
    int current_x = maze->mouse_pos.x;
    int current_y = maze->mouse_pos.y;

    if (API::wallFront()) {
        // bitwise OR the current cell walls attribute, with the proper mask
        maze->cellWalls[current_y][current_x] |= dir_mask[current_direction];
    }

    if (API::wallRight()) {
        // calculate the "right" direction with modulo
        Direction right_direction = static_cast<Direction>((current_direction + 1) % 4);
        maze->cellWalls[current_y][current_x] |= dir_mask[right_direction];
    }

    if (API::wallLeft()) {
        // calculate the "left" direction with modulo
        Direction left_direction = static_cast<Direction>((current_direction - 1) % 4);
        maze->cellWalls[current_y][current_x] |= dir_mask[left_direction];
    }
}



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

    testMaze.cellWalls[1][1] = 0b001;

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
