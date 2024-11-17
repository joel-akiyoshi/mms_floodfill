#include <iostream>
#include <string>

#include "API.h"
#include "pathfinder.h"

void initializeMaze(Maze* maze) {
    // Initialize maze with 0 for no walls, and walls represented by 1 for simplicity
    for (int i = 0; i < MAZE_SIZE; i++) {
        for (int j = 0; j < MAZE_SIZE; j++) {
            maze->cellWalls[i][j] = 0;  // No walls initially
        }
    }
}

void testGetNeighborCells(Maze* maze, Coord c) {
    CellList* neighbors = getNeighborCells(maze, c);

    // Print out the neighbors
    printf("Neighbors for cell (%d, %d):\n", c.x, c.y);
    for (int i = 0; i < neighbors->size; i++) {
        Cell cell = neighbors->cells[i];
        printf("Neighbor at (%d, %d), Direction: %d\n", cell.pos.x, cell.pos.y, cell.dir);
    }

    // Free allocated memory for neighbors
    free(neighbors->cells);
    free(neighbors);
}

int main() {
    Maze maze;
    initializeMaze(&maze);

    // Test with a few coordinates
    Coord testCoord1 = {1, 1};  // Cell at (1, 1)
    testGetNeighborCells(&maze, testCoord1);

    Coord testCoord2 = {2, 2};  // Cell at (2, 2)
    testGetNeighborCells(&maze, testCoord2);

    Coord testCoord3 = {0, 0};  // Cell at (0, 0)
    testGetNeighborCells(&maze, testCoord3);

    return 0;
}
