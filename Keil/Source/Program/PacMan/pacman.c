#include "pacman.h"
#include "peripherals.h"

#include <stdio.h>
#include <string.h>

// Images
#include "Assets/Images/pacman-logo.h"
#include "Assets/Images/pacman-maze.h"

// Fonts
#include "Assets/Fonts/font-upheavtt14.h"

// PRIVATE VARIABLES

#define PILLS_COUNT 240
#define PILL_RADIUS 1

/// @brief Instead of having 240x260 cells in the maze map, we can actually divide the
///        maze into 24x26 cells, each cell being 10x10 pixels, and determine if a given
///        cell is a wall by checking if more than half of the pixels in the cell are blue.
///        This, thanks to the fact that the Pac-Man does not have to move in a pixel-perfect
///        way, but rather in a cell-per-cell way.
/// @note It's always possible to tweak this factor to get a more precise overlay grid,
///       which results in a more precise mapping.
#define MAZE_GRID_SCALE_FACTOR 10

#define MAZE_WIDTH 240
#define MAZE_HEIGHT 260

#define MAZE_MAP_WIDTH (MAZE_WIDTH / MAZE_GRID_SCALE_FACTOR)
#define MAZE_MAP_HEIGHT (MAZE_HEIGHT / MAZE_GRID_SCALE_FACTOR)

_PRIVATE LCD_FontID font_upheavtt14;
_PRIVATE PM_MazeObject maze_map[MAZE_MAP_WIDTH][MAZE_MAP_HEIGHT] = {0};

// IDS OF OBJECTS

_PRIVATE LCD_ObjID stats_text_obj_id, stats_values_obj_id;

// PRIVATE CELL/PIXELS FUNCTIONS

/// @brief Returns the bounding box of the cell, in pixels, taking into account the maze image position.
/// @param cell The cell to get the bounding box of
/// @param maze_img_pos The position of the maze image on the screen
/// @return The bounding box of the cell.
/// @note You should iterate from (x0, y0) (inclusive) to (x1, y1) (exclusive)
_PRIVATE inline BoundingBox get_cell_pixel_bbox(MazeMapCell cell, LCD_Coordinate maze_img_pos)
{
    return (BoundingBox){
        .x0 = cell.col * MAZE_GRID_SCALE_FACTOR + maze_img_pos.x,
        .y0 = cell.row * MAZE_GRID_SCALE_FACTOR + maze_img_pos.y,
        .x1 = (cell.col + 1) * MAZE_GRID_SCALE_FACTOR + maze_img_pos.x,
        .y1 = (cell.row + 1) * MAZE_GRID_SCALE_FACTOR + maze_img_pos.y,
    };
}

/// @brief Returns the cell that contains the given pixel.
/// @param pixel The pixel coordinate
/// @param maze_img_pos The position of the maze image on the screen
/// @return The cell that contains the given pixel.
_PRIVATE inline MazeMapCell map_pixel_to_cell(LCD_Coordinate pixel, LCD_Coordinate maze_img_pos)
{
    return (MazeMapCell){
        .row = (pixel.y - maze_img_pos.y) / MAZE_GRID_SCALE_FACTOR,
        .col = (pixel.x - maze_img_pos.x) / MAZE_GRID_SCALE_FACTOR,
    };
}

/// @brief Checks if two colors are almost the same, with a given threshold.
_PRIVATE inline bool almost_same_colors(LCD_Color color1, LCD_Color color2)
{
    const u8 red_diff = ((color1 >> 11) & 0x1F) - ((color2 >> 11) & 0x1F);
    const u8 green_diff = ((color1 >> 5) & 0x3F) - ((color2 >> 5) & 0x3F);
    const u8 blue_diff = (color1 & 0x1F) - (color2 & 0x1F);
    return red_diff <= 1 && green_diff <= 1 && blue_diff <= 1;
}

/// @brief Determines if a cell is mostly of a given color.
/// @param cell_bbox The bounding box of the cell
/// @param color The color to check for
/// @param threshold The threshold percentage of the color in the cell (0-100)
/// @return True if the cell is mostly of the given color, false otherwise.
_PRIVATE inline bool cell_is_mostly_color(BoundingBox cell_bbox, LCD_Color color, u8 threshold)
{
    const u16 pixels_in_cell = MAZE_GRID_SCALE_FACTOR * MAZE_GRID_SCALE_FACTOR;

    u16 color_count = 0, pixel_color;
    LCD_Coordinate coords;
    for (u16 y = cell_bbox.y0; y < cell_bbox.y1; y++)
    {
        for (u16 x = cell_bbox.x0; x < cell_bbox.x1; x++)
        {
            coords.x = x, coords.y = y;
            pixel_color = LCD_GetPointColor(coords);
            if (almost_same_colors(pixel_color, color))
                color_count++;
        }
    }

    return (color_count * 100) / pixels_in_cell > threshold;
}

// PRIVATE MAZE MAPPING FUNCTIONS

_PRIVATE void walls_to_maze_map(LCD_Color wall_color, LCD_Coordinate maze_img_pos)
{
    BoundingBox bbox;
    MazeMapCell cell;
    for (u16 row = 0; row < MAZE_MAP_HEIGHT; row++)
    {
        for (u16 col = 0; col < MAZE_MAP_WIDTH; col++)
        {
            cell.row = row, cell.col = col;
            bbox = get_cell_pixel_bbox(cell, maze_img_pos);
            if (cell_is_mostly_color(bbox, wall_color, 50))
                maze_map[row][col] = PM_MAZE_WALL;
        }
    }
}

// PRIVATE RENDERING FUNCTIONS

_PRIVATE inline void render_maze(void)
{
    const LCD_Coordinate maze_img_pos = {
        .x = (LCD_GetWidth() - Image_PACMAN_Maze.width) / 2,
        .y = LCD_GetHeight() - Image_PACMAN_Maze.height,
    };

    // We can render the maze in immediate mode, since we do not need to
    // change its state during the game (for now).

    // clang-format off
    LCD_OBJECT_IMMEDIATE(1, {
        LCD_IMAGE(maze_img_pos.x, maze_img_pos.y, Image_PACMAN_Maze),
    });
    // clang-format on

    // Mapping the walls onto the maze map
    walls_to_maze_map(LCD_COL_BLUE, maze_img_pos);
}

_PRIVATE inline void render_game_infos(PM_GameStats stats)
{
    char *const score_txt = "SCORE", *const record_txt = "RECORD", *const time_left_txt = "TIME LEFT";
    const LCD_Coordinate time_left_txt_pos = {5, 5}, record_txt_pos = {LCD_GetWidth() - 60, 5},
                         score_txt_pos = {LCD_GetWidth() / 2 - 10, 5};

    // clang-format off
    LCD_BEGIN_DRAWING;
    stats_text_obj_id = LCD_OBJECT(stats_text, 3, {
        LCD_TEXT(time_left_txt_pos.x, time_left_txt_pos.y, {
            .text = time_left_txt, .font = font_upheavtt14,
            .text_color = LCD_COL_YELLOW, .bg_color = LCD_COL_NONE,
        }),
        LCD_TEXT(score_txt_pos.x, score_txt_pos.y, {
            .text = score_txt, .font = font_upheavtt14,
            .text_color = LCD_COL_YELLOW, .bg_color = LCD_COL_NONE,
        }),
        LCD_TEXT(record_txt_pos.x, record_txt_pos.y, {
            .text = record_txt, .font = font_upheavtt14,
            .text_color = LCD_COL_YELLOW, .bg_color = LCD_COL_NONE,
        }),
    });
    LCD_END_DRAWING;
    // clang-format on

    const u8 val_len = 10, val_y_padding = 15;
    char score_val[val_len], record_val[val_len], time_left_val[val_len];

    snprintf(score_val, val_len, "%d", stats.score);
    snprintf(record_val, val_len, "%d", stats.record);
    snprintf(time_left_val, val_len, "%d", stats.time_left);

    // clang-format off
    LCD_BEGIN_DRAWING;
    stats_values_obj_id = LCD_OBJECT(stats_vals, 3, {
        LCD_TEXT(time_left_txt_pos.x, time_left_txt_pos.y + val_y_padding, {
            .text = time_left_val, .font = font_upheavtt14,
            .text_color = LCD_COL_WHITE, .bg_color = LCD_COL_NONE,
        }),
        LCD_TEXT(score_txt_pos.x, score_txt_pos.y + val_y_padding, {
            .text = score_val, .font = font_upheavtt14,
            .text_color = LCD_COL_YELLOW, .bg_color = LCD_COL_NONE,
        }),
        LCD_TEXT(record_txt_pos.x, record_txt_pos.y + val_y_padding, {
            .text = record_val, .font = font_upheavtt14,
            .text_color = LCD_COL_WHITE, .bg_color = LCD_COL_NONE,
        }),
    });
    LCD_END_DRAWING;
    // clang-format on
}

_PRIVATE void render_splash(void)
{
    LCD_ObjID welcome_id;
    const LCD_Coordinate logo_pos = {
        .x = (LCD_GetWidth() - Image_PACMAN_Logo.width) / 2,
        .y = (LCD_GetHeight() - Image_PACMAN_Logo.height) / 2,
    };

    const TP_Button start_button = {
        .pos = {.x = LCD_GetWidth() / 2 - 15, .y = logo_pos.y + Image_PACMAN_Logo.height + 12},
        .width = 30,
        .height = 15,
    };

    // clang-format off
    LCD_BEGIN_DRAWING;
    welcome_id = LCD_OBJECT(welcome, 4, {
        LCD_TEXT(83, logo_pos.y - 6, {
            .text = "Welcome to",
            .font = font_upheavtt14,
            .text_color = LCD_COL_WHITE,
            .bg_color = LCD_COL_NONE,
        }),
        LCD_IMAGE(logo_pos.x, logo_pos.y, Image_PACMAN_Logo),
        LCD_TEXT(start_button.pos.x, start_button.pos.y, {
            .text = "Start!",
            .font = font_upheavtt14,
            .text_color = LCD_COL_YELLOW,
            .bg_color = LCD_COL_NONE,
        }),
    });
    LCD_END_DRAWING;
    // clang-format on

    TP_WaitForButtonPress(start_button);
    LCD_RQRemoveObject(welcome_id, false);
}

// PUBLIC FUNCTIONS

PM_Error PACMAN_Init(void)
{
    if (!LCD_IsInitialized())
        return PM_ERR_UNINIT_LCD;

    if (!TP_IsInitialized())
        return PM_ERR_UNINIT_TP;

    // LCD_SetBackgroundColor(LCD_COL_BLACK);
    font_upheavtt14 = LCD_FMAddFont(Font_Upheavtt14);

    render_splash();
    render_game_infos((PM_GameStats){.score = 0, .record = 0, .time_left = 0});
    return PM_ERR_NONE;
}