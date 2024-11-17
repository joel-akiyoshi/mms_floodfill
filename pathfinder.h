#ifndef PATHFINDER_H
#define PATHFINDER_H

#include "API.h"  // Include this if the API is required

// Constant definitions
const int MAZE_SIZE = 16;
const int MAX_QUEUE_SIZE = 256;
const int MAX_COST = 255;

// Directional characters (n = North, e = East, s = South, w = West)
extern char dir_chars[4];

// Directional bit masks for updating the cell walls
extern int dir_mask[4];

// Direction enum for navigating the maze
enum Direction {
    NORTH = 0,
    EAST = 1,
    SOUTH = 2,
    WEST = 3
};

// Direction bitmasks for setting walls
enum DirectionBitmask {
    NORTH_MASK = 0b1000,
    EAST_MASK  = 0b0100,
    SOUTH_MASK = 0b0010,
    WEST_MASK  = 0b0001
};

// Structure to store coordinates
struct Coord {
    int x;
    int y;
};

// Structure for representing a maze cell
struct Cell {
    Coord pos;
    Direction dir;
};

// Structure for a list of cells
struct CellList {
    int size;
    Cell* cells;
};

// Structure for queue (to be used for BFS or DFS)
struct Queue {
    int head;
    int tail;
    // Cell object array with max size
    Cell cells[MAX_QUEUE_SIZE];  // assuming Cell array will be used for storing queue elements
};

// Maze structure
struct Maze {
    Coord mouse_pos;
    Direction mouse_dir;

    int cellWalls[MAZE_SIZE][MAZE_SIZE];
    int distances[MAZE_SIZE][MAZE_SIZE];

    Coord* goalPos;
};

// Queue functions
void initQueue(Queue* q);  // Initialize the queue
bool isQEmpty(Queue q);  // Check if the queue is empty

// Functions for clockwise and counterclockwise movement
Direction clockwiseStep(Direction initialDirection);  // return new direction
Direction counterClockwiseStep(Direction initialDirection);  // return new direction

// Maze-related functions
void scanWalls(Maze* maze);  // Detect and update walls (in memory) around the mouse
void updateSimulator(Maze maze);  // Update the maze simulator based on mouse's memory
void updateMousePos(Coord* pos, Direction dir);

CellList* getNeighborCells(Maze* maze, Coord c);
Cell getBestCell(CellList* candidateCells, Maze* maze);
void floodFill(Maze* maze, bool to_start);

#endif // PATHFINDER_H
