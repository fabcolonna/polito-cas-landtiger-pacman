#include "pacman.h"
#include "includes.h"

#include "pacman_maze.h"

void PACMAN_Init(void)
{
    LCD_ClearWith(COL_BLACK);

    const LCD_Coordinate maze_pos = {.x = 0, .y = 40};
    LCD_DrawImage32(PACMAN_Maze, PACMAN_MazeWidth, PACMAN_MazeHeight, maze_pos);
}
