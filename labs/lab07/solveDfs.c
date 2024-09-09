// DFS maze solver

#include <stdbool.h>
#include <stdio.h>
#include <stdlib.h>
#include <string.h>

#include "Cell.h"
#include "helpers.h"
#include "Maze.h"
#include "Stack.h"

#define MAX_NEIGHBOURS 4
static void fillPath(Maze m, Cell start_cell, Cell exit, Cell **parent);
static bool validCell(Maze m, Cell c);

bool solve(Maze m) {
    // TODO: Complete this function
    //       Feel free to add helper functions
    
    //basically same with Bfs
    //but replace queue operation to stack
    
    //height and width of matrix
    int h = MazeHeight(m);
    int w = MazeWidth(m);
    
    
    //gets the starting cell
    Cell start_cell =  MazeGetStart(m);

    //create visited array
    bool **visited = createBoolMatrix(h,w);
    
    //create predecessor array
    Cell **parent = createCellMatrix(h,w);

    Stack s = StackNew();
    
    StackPush(s, start_cell);
    
    bool found = false;
    
    while (!found && !StackIsEmpty(s)) {
        //get curruent cell from the queue and remove it from queue
        Cell curr = StackPop(s);
        //if visited, continue to next loop
        if (visited[curr.row][curr.col]) {
            continue;
        }
        //mark as visited
        visited[curr.row][curr.col] = true;
        if (MazeVisit(m, curr)) {
            fillPath(m, start_cell, curr, parent);
            found = true;
            break;
        }
        //initialze cell
        Cell adjacent[MAX_NEIGHBOURS] = {
            {.row = curr.row - 1, .col = curr.col    }, //up
            {.row = curr.row,     .col = curr.col - 1}, //left
            {.row = curr.row + 1, .col = curr.col    }, //down
            {.row = curr.row    , .col = curr.col + 1}, //right
        };
        for (int i = 0; i < MAX_NEIGHBOURS; i++) {
            Cell w = adjacent[i];
            //if the cell is not out of the maze nor a wall nor visited
            if (validCell(m, w) && !MazeIsWall(m, w) && !visited[w.row][w.col]) {
                StackPush(s, w);
                //mark parent
                parent[w.row][w.col] = curr;
            }
        }
    }
    
    //free stack
    StackFree(s);
    
    
    //free visited array
    freeBoolMatrix(visited);
        
    //free parent array
    freeCellMatrix(parent);

    return found;
}

//marks the path to the exit
static void fillPath(Maze m, Cell start_cell, Cell exit, Cell **parent) {
    Cell curr = exit;
    //until headed back to the start cell
    while (!(curr.col == start_cell.col && curr.row == start_cell.row)) {
        MazeMarkPath(m, curr);
        curr = parent[curr.row][curr.col];
    }
    MazeMarkPath(m, start_cell);
}
//cell row and col has to be non-negative and smaller than maze height and width
static bool validCell(Maze m, Cell c) {
    return (
        c.row >= 0 && c.row < MazeHeight(m) && c.col >= 0 && c.col < MazeWidth(m)
    );
}

