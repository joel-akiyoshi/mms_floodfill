#include <iostream>
#include <string>

#include "pathfinder.h"
#include "API.h"

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

// Movement functions
Direction clockwiseStep(Direction initialDirection) {
    return static_cast<Direction>((initialDirection + 1) % 4);
}

Direction counterClockwiseStep(Direction initialDirection) {
    return static_cast<Direction>((initialDirection + 3) % 4);
}


// Maze functions
void scanWalls(Maze* maze) { // fill in code for changing value of the cell walls
    Direction current_direction = maze->mouse_dir;
    int current_x = maze->mouse_pos.x;
    int current_y = maze->mouse_pos.y;

    if (API::wallFront()) {
        // bitwise OR the current cell walls attribute, with the proper mask
        maze->cellWalls[current_y][current_x] |= dir_mask[current_direction];
        std::cerr << "at position: " << current_x << ", " << current_y << ", " << "wall in front" << std::endl;
    }

    if (API::wallRight()) {
        // calculate the "right" direction with modulo
        Direction right_direction = static_cast<Direction>((current_direction + 1) % 4);
        maze->cellWalls[current_y][current_x] |= dir_mask[right_direction];
        std::cerr << "at position: " << current_x << ", " << current_y << ", " << "wall on right" << std::endl;
    }

    if (API::wallLeft()) {
        // calculate the "left" direction with modulo
        Direction left_direction = static_cast<Direction>((current_direction + 3) % 4);
        maze->cellWalls[current_y][current_x] |= dir_mask[left_direction];
        std::cerr << "at position: " << current_x << ", " << current_y << ", " << "wall on left" << std::endl;
    }
}


void updateSimulator(Maze maze) { // redraws the maze in simulator after each loop in main
    for(int x = 0; x < MAZE_SIZE; x++) 
    {
        for(int y = 0; y < MAZE_SIZE; y++) 
        {
            if (maze.cellWalls[y][x] & NORTH_MASK) {
                API::setWall(x, y, dir_chars[NORTH]);
            }

            if (maze.cellWalls[y][x] & EAST_MASK) {
                API::setWall(x, y, dir_chars[EAST]);
            }

            if (maze.cellWalls[y][x] & SOUTH_MASK) {
                API::setWall(x, y, dir_chars[SOUTH]);
            }

            if (maze.cellWalls[y][x] & WEST_MASK) {
                API::setWall(x, y, dir_chars[WEST]);
            }
        }
    }
}


void updateMousePos(Coord* pos, Direction dir) {
    // depending on the mouse direction, increment position by one
    if (dir == NORTH) {
        pos->y++;
    }

    if (dir == SOUTH) {
        pos->y--;
    }

    if (dir == WEST) {
        pos->x--;
    }

    if (dir == EAST) {
        pos->x++;
    }
}


CellList* getNeighborCells(Maze* maze, Coord c) {
    CellList* neighbors = (CellList*)malloc(sizeof(CellList));  // Allocate memory for the CellList struct
    neighbors->cells = (Cell*)malloc(sizeof(Cell) * 4);  // Allocate memory for 4 Cells in the "cells" attribute
    neighbors->size = 0;  // Initialize size to 0

    // original x and y
    int x_val = c.x;
    int y_val = c.y;

    // Direction changes for N, E, S, W (North, East, South, West)
    int dx[4] = {0, 1, 0, -1};
    int dy[4] = {1, 0, -1, 0};

    // Iterate over the 4 possible directions
    for (int i = 0; i < 4; i++) {
        // Calculate new x and y values based on direction
        int new_x = x_val + dx[i];
        int new_y = y_val + dy[i];

        // Check bounds
        if (0 <= new_x && new_x < MAZE_SIZE && 0 <= new_y && new_y < MAZE_SIZE) {
            
            Direction dir = (Direction)i;  // Direction as enum

            // Get the wall descriptor for the target cell
            int wallDescriptor = maze->cellWalls[new_y][new_x];

            // Check if there's no wall in the current direction
            if (!(wallDescriptor & dir_mask[dir])) {
                // construct and add a new neighbor cell
                Coord targetCoord{new_x, new_y};
                neighbors->cells[neighbors->size] = (Cell){targetCoord, dir};
                neighbors->size++;  // Increment the size of the list
            }
        }
    }

    return neighbors;
}

Cell getBestCell(CellList* candidateCells, Maze* maze) {
    Cell bestCell = candidateCells->cells[0];
    int bestDistance = maze->distances[bestCell.pos.y][bestCell.pos.x];  // distance of the first cell.

    // iterate through candidateCells
    for (int i = 1; i < candidateCells->size; i++) {
        Cell currentCell = candidateCells->cells[i];
        int currentDistance = maze->distances[currentCell.pos.y][currentCell.pos.x];
        
        // If the current cell has a lower distance, update the bestCell.
        if (currentDistance < bestDistance) {
            bestCell = currentCell;
            bestDistance = currentDistance;
        }
    }

    return bestCell;  // Return the cell with the lowest distance.
}


void floodFill(Maze* maze, bool to_start) { // function to be called everytime you move into a new cell
    
}


void log(const std::string& text) {
    std::cerr << text << std::endl;
}

/*

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

    print_arr(testMaze.cellWalls);

    Maze* mazePtr = &testMaze;
    

    while (true) {
        scanWalls(mazePtr);
        print_arr(testMaze.cellWalls);
        updateSimulator(testMaze);

        if (!API::wallLeft()) {
            API::turnLeft();
            mazePtr->mouse_dir = counterClockwiseStep(mazePtr->mouse_dir);

        }
        while (API::wallFront()) {
            API::turnRight();
            mazePtr->mouse_dir = clockwiseStep(mazePtr->mouse_dir);
        }

        API::moveForward();
        updateMousePos(&mazePtr->mouse_pos, mazePtr->mouse_dir);
    }
}

*/