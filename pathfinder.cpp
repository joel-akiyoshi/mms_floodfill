#include <iostream>
#include <string>
#include "pathfinder.h"
#include "API.h"


char dir_chars[4] = {'n', 'e', 's', 'w'};

int dir_mask[4] = {0b1000, 0b0100, 0b0010, 0b0001};


// Queue functions
void initQueue(Queue *q) {
    q->head = 0;
    q->tail = 0;
}


bool isQEmpty(Queue q) {
    return q.head == q.tail;
}




// Movement functions
Direction clockwiseStep(Direction initialDirection) {
    return static_cast<Direction>((initialDirection + 1) % 4);
}


Direction counterClockwiseStep(Direction initialDirection) {
    return static_cast<Direction>((initialDirection + 3) % 4);
}


void rotate(Maze* maze, Direction targetDir) {

    Direction currentDir = maze->mouse_dir;

    int rotation = (currentDir - targetDir + 4) % 4;

    maze->mouse_dir = targetDir;
    if (rotation == 1) {
        API::turnLeft();

    } else if (rotation == 2) {
        API::turnLeft();
        API::turnLeft();

    } else if (rotation == 3) {
        API::turnRight();
    }
}


// Maze functions
void trySetAdj(Maze* maze, int x, int y, Direction wallDirection) {
    int dx[4] = {0, 1, 0, -1};
    int dy[4] = {1, 0, -1, 0};

    int adj_x = x + dx[wallDirection];
    int adj_y = y + dy[wallDirection];

    Direction opp_direction = static_cast<Direction>((wallDirection + 2) % 4);

    if (0 <= adj_x && 0 <= adj_y && adj_x < MAZE_SIZE && adj_y < MAZE_SIZE) {
        maze->cellWalls[adj_y][adj_x] |= dir_mask[opp_direction];
    }   
}


void scanWalls(Maze* maze) { // fill in code for changing value of the cell walls
    Direction current_direction = maze->mouse_dir;
    int current_x = maze->mouse_pos.x;
    int current_y = maze->mouse_pos.y;

    if (API::wallFront()) {
        // bitwise OR the current cell walls attribute, with the proper mask
        maze->cellWalls[current_y][current_x] |= dir_mask[current_direction];
        trySetAdj(maze, current_x, current_y, current_direction);
    }

    if (API::wallRight()) {
        // calculate the "right" direction with modulo
        Direction wall_direction = static_cast<Direction>((current_direction + 1) % 4);
        maze->cellWalls[current_y][current_x] |= dir_mask[wall_direction];
        trySetAdj(maze, current_x, current_y, wall_direction);
    }

    if (API::wallLeft()) {
        // calculate the "left" direction with modulo
        Direction wall_direction = static_cast<Direction>((current_direction + 3) % 4);
        maze->cellWalls[current_y][current_x] |= dir_mask[wall_direction];
        trySetAdj(maze, current_x, current_y, wall_direction);
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
    // depending on the mouse direction, increment position by one and update the UI
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
    
    API::moveForward();
}


CellList* getNeighborCells(Maze* maze, Coord c) {

    CellList* neighbors = (CellList*)malloc(sizeof(CellList));
    neighbors->cells = (Cell*)malloc(sizeof(Cell) * 4);
    neighbors->size = 0;

    int currentWalls = maze->cellWalls[c.y][c.x];

    int dx[4] = {0, 1, 0, -1};
    int dy[4] = {1, 0, -1, 0};

    for (int i = 0; i < 4; i++) {
        int new_x = c.x + dx[i];
        int new_y = c.y + dy[i];

        if (0 <= new_x && new_x < MAZE_SIZE && 0 <= new_y && new_y < MAZE_SIZE) {
            
            Direction dir = static_cast<Direction>(i);

            if (!(currentWalls & dir_mask[i])) {

                Coord targetCoord{new_x, new_y};
                neighbors->cells[neighbors->size] = (Cell){targetCoord, dir};
                neighbors->size++;
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

Coord* getGoalCoords() {
    int half_max = MAZE_SIZE / 2;
    
    if (MAZE_SIZE % 2) {  // Odd-sized maze: Single center cell
        Coord* goalCellArr = (Coord*)malloc(sizeof(Coord) * 1); 
        goalCellArr[0] = {half_max, half_max};  // Center coordinate
        return goalCellArr;

    } else {  // Even-sized maze: 4 center cells
        Coord* goalCellArr = (Coord*)malloc(sizeof(Coord) * 4);
        goalCellArr[0] = {half_max - 1, half_max - 1};  // Top-left 
        goalCellArr[1] = {half_max - 1, half_max};      // Top-right
        goalCellArr[2] = {half_max, half_max - 1};      // Bottom-left
        goalCellArr[3] = {half_max, half_max};          // Bottom-right 
        return goalCellArr;
    }
}

bool atGoal(Maze* maze) {
    int numGoalCoords =  (MAZE_SIZE % 2) ? 1 : 4;
    Coord* goalCoords = getGoalCoords();
    for (int i = 0; i < numGoalCoords; i++) {
        if (goalCoords[i].x == maze->mouse_pos.x && goalCoords[i].y == maze->mouse_pos.y) {
            free(goalCoords);
            return true;
        }
    }
    free(goalCoords);
    return false;
}

void floodFill(Maze* maze, bool to_start) {
    // Initialize distances
    for (int y = 0; y < MAZE_SIZE; y++) {
        for (int x = 0; x < MAZE_SIZE; x++) {
            maze->distances[y][x] = MAX_COST;
            API::setText(x, y, std::to_string(MAX_COST));
        }
    }

    // Initialize queue
    Queue q;
    initQueue(&q);

    // Add goal cells
    Coord* goals = getGoalCoords();
    int goalCount = (MAZE_SIZE % 2) ? 1 : 4;

    for (int i = 0; i < goalCount; i++) {
        Coord goal = goals[i];
        maze->distances[goal.y][goal.x] = 0;
        API::setColor(goal.x, goal.y, 'G');
        API::setText(goal.x, goal.y, "0");
        q.coords[q.tail++] = goal;  // enqueue goals
    }
    free(goals);


    while (!isQEmpty(q)) {
        Coord current = q.coords[q.head++];  // pop from the queue
        int currentDist = maze->distances[current.y][current.x];

        CellList* neighbors = getNeighborCells(maze, current);

        for (int i = 0; i < neighbors->size; i++) {
            Coord neighbor = neighbors->cells[i].pos;

            if (maze->distances[neighbor.y][neighbor.x] > currentDist + 1) {
                maze->distances[neighbor.y][neighbor.x] = currentDist + 1;
                API::setText(neighbor.x, neighbor.y, std::to_string(currentDist + 1));
                

                q.coords[q.tail++] = neighbor;  // enqueue the neighbor
            }
        }
        free(neighbors->cells);
        free(neighbors);
    }
}


int main(int argc, char* argv[]) {
    API::setColor(0, 0, 'G');
    API::setText(0, 0, "START");

    Maze testMaze;
    for (int i = 0; i < MAZE_SIZE; i++) {
        for (int j = 0; j < MAZE_SIZE; j++) {
            testMaze.cellWalls[i][j] = 0;
        }
    }

    Maze* mazePtr = &testMaze;
    mazePtr->mouse_dir = NORTH;
    mazePtr->mouse_pos.x = 0;
    mazePtr->mouse_pos.y = 0;
    

    while (!atGoal(mazePtr)) {
        scanWalls(mazePtr);
        updateSimulator(testMaze);
        floodFill(mazePtr, true);

        Cell best = getBestCell(getNeighborCells(mazePtr, mazePtr->mouse_pos), mazePtr);
        rotate(mazePtr, best.dir);

        updateMousePos(&mazePtr->mouse_pos, mazePtr->mouse_dir);
        API::setColor(mazePtr->mouse_pos.x, mazePtr->mouse_pos.y, 'C');
    }
}
