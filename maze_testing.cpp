#include <iostream>
#include <string>

#include "API.h"
#include "pathfinder.h"

void print_arr(int arr[MAZE_SIZE][MAZE_SIZE]) {
    for (int i = 0; i < MAZE_SIZE; i++) {
        for (int j = 0; j < MAZE_SIZE; j++) {
            std::cout << arr[i][j] << " ";  // Print the element
        }
        std::cout << std::endl;  // Move to the next line after each row
    }
}

void scanWallsTest() {

}

int main() {
    Maze testMaze;

    testMaze.cellWalls[MAZE_SIZE][MAZE_SIZE] = {0};  // Initialize to 0

    // Print the cellWalls
    print_arr(testMaze.cellWalls);

    testMaze.cellWalls[0][0] = 0b1000;  // put a wall on the North

    return 0;
}
