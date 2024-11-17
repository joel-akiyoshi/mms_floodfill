#include <iostream>
#include <string>

#include "API.h"
#include "pathfinder.h"

void log(const std::string& text);  // print to the mms output

void print_arr(int arr[MAZE_SIZE][MAZE_SIZE]) {
    // Print from bottom to top, so (0, 0) is at the bottom-left
    for (int y = 0; y < MAZE_SIZE; y++) {  // Loop over rows (y) from bottom to top
        for (int x = 0; x < MAZE_SIZE; x++) {  // Loop over columns (x) from left to right
            std::cerr << arr[MAZE_SIZE - 1 - y][x] << " ";
        }
        std::cerr << std::endl;
    }
}


void initializeMaze(Maze* maze) {
    // Initialize maze with 0 for no walls, and walls represented by 1 for simplicity
    for (int i = 0; i < MAZE_SIZE; i++) {
        for (int j = 0; j < MAZE_SIZE; j++) {
            maze->cellWalls[i][j] = 0;  // No walls initially
        }
    }

    print_arr(maze->cellWalls);
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
    for (int i = 0; i < MAZE_SIZE; i++) {
        for (int j = 0; j < MAZE_SIZE; j++) {
            // Set higher values for most cells, but make (5, 5) the lowest distance (e.g., 0)
            if (i == 5 && j == 5) {
                maze.distances[i][j] = 0;  // Best cell
            } else {
                maze.distances[i][j] = 100 - (i + j);  // Just some values for testing
            }
        }
    }

    print_arr(maze.distances);

    // Test CellList with some candidate cells
    CellList candidateCells;
    candidateCells.size = 4;
    candidateCells.cells = (Cell*)malloc(sizeof(Cell) * candidateCells.size);

    // Initialize candidate cells with arbitrary coordinates and directions
    candidateCells.cells[0] = { {0, 0}, NORTH };
    candidateCells.cells[1] = { {1, 1}, EAST };
    candidateCells.cells[2] = { {2, 2}, SOUTH };
    candidateCells.cells[3] = { {3, 3}, WEST };

    // Call getBestCell to find the cell with the lowest distance
    Cell bestCell = getBestCell(&candidateCells, &maze);

    // Print the results
    std::cout << "Best cell: (" << bestCell.pos.x << ", " << bestCell.pos.y << ") with Direction: " << bestCell.dir << std::endl;

    // Free the allocated memory
    free(candidateCells.cells);

    return 0;
}
