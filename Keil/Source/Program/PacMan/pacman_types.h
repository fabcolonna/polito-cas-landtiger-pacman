#ifndef __PACMAN_TYPES_H
#define __PACMAN_TYPES_H

#include "peripherals.h"

// INTERNAL

typedef struct
{
    u16 row, col;
} MazeMapCell;

typedef struct
{
    u16 x0, y0, x1, y1;
} BoundingBox;

// GAME RELATED

typedef enum
{
    PM_ERR_NONE = 0,
    PM_ERR_UNINIT_LCD,
    PM_ERR_UNINIT_TP
} PM_Error;

typedef enum
{
    PM_MAZE_WALL,
    PM_MAZE_PILL,
    PM_MAZE_EMPTY,
} PM_MazeObject;

typedef struct
{
    u16 score, record;
    u16 time_left;
} PM_GameStats;

#endif