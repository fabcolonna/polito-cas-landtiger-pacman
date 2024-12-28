#ifndef __GLCD_TYPES_H
#define __GLCD_TYPES_H

#include "utils.h"

#define DISP_ORIENTATION 0

#if (DISP_ORIENTATION == 90) || (DISP_ORIENTATION == 270)
#define MAX_X 320
#define MAX_Y 240
#elif (DISP_ORIENTATION == 0) || (DISP_ORIENTATION == 180)
#define MAX_X 240
#define MAX_Y 320
#endif

// clang-format off

// Top row positions
#define LCD_POS_TOP_LEFT(pad_x, pad_y)                      (LCD_Coordinate){10 + (pad_x), 10 + (pad_y)}
#define LCD_POS_TOP_CENTER(pad_x, pad_y, obj_width)         (LCD_Coordinate){(MAX_X - (obj_width)) / 2 + (pad_x), 10 + (pad_y)}
#define LCD_POS_TOP_RIGHT(pad_x, pad_y, obj_width)          (LCD_Coordinate){MAX_X - 10 - (obj_width) + (pad_x), 10 + (pad_y)}

// Middle row positions
#define LCD_POS_MIDDLE_LEFT(pad_x, pad_y, obj_height)               (LCD_Coordinate){10 + (pad_x), (MAX_Y - (obj_height)) / 2 + (pad_y)}
#define LCD_POS_MIDDLE_CENTER(pad_x, pad_y, obj_width, obj_height)  (LCD_Coordinate){(MAX_X - (obj_width)) / 2 + (pad_x), (MAX_Y - (obj_height)) / 2 + (pad_y)}
#define LCD_POS_MIDDLE_RIGHT(pad_x, pad_y, obj_width, obj_height)   (LCD_Coordinate){MAX_X - 10 - (obj_width) + (pad_x), (MAX_Y - (obj_height)) / 2 + (pad_y)}

// Bottom row positions
#define LCD_POS_BOTTOM_LEFT(pad_x, pad_y, obj_height)               (LCD_Coordinate){10 + (pad_x), MAX_Y - 10 - (obj_height) + (pad_y)}
#define LCD_POS_BOTTOM_CENTER(pad_x, pad_y, obj_width, obj_height)  (LCD_Coordinate){(MAX_X - (obj_width)) / 2 + (pad_x), MAX_Y - 10 - (obj_height) + (pad_y)}
#define LCD_POS_BOTTOM_RIGHT(pad_x, pad_y, obj_width, obj_height)   (LCD_Coordinate){MAX_X - 10 - (obj_width) + (pad_x), MAX_Y - 10 - (obj_height) + (pad_y)}

// clang-format on

/// @brief Even though the LCD itself supports 260K colors, the bus interface
///        is only 16-bit, so we can only use 65K colors
typedef enum
{
    COL_WHITE = 0xFFFF,
    COL_BLACK = 0x0000,
    COL_GREY = 0xF7DE,
    COL_BLUE = 0x001F,
    COL_BLUE2 = 0x051F,
    COL_RED = 0xF800,
    COL_MAGENTA = 0xF81F,
    COL_GREEN = 0x07E0,
    COL_CYAN = 0x7FFF,
    COL_YELLOW = 0xFFE0
} LCD_Color;

typedef struct
{
    u16 x, y;
} LCD_Coordinate;

typedef enum
{
    LCD_FONT_MSGOTHIC,
    LCD_FONT_SYSTEM
} LCD_Font;

typedef u16 LCD_RGBColor;

#define NO_FILL_COLOR (u16)(-1)

/// @brief Converts RGB (24 bit) into RGB565 (16 bit):
///        - 5 bits for red (bits 11-15)
///        - 6 bits for green (bits 5-10)
///        - 5 bits for blue (bits 0-4)
#define RGB_TO_RGB565(r, g, b) (u16)(((r >> 3) << 11) | ((g >> 2) << 5) | (b >> 3))

#endif