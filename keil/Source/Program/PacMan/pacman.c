#include "pacman.h"
#include "peripherals.h"

// Images
#include "Assets/Images/pacman-logo.h"
#include "Assets/Images/pacman-maze.h"

// Fonts
#include "Assets/Fonts/font-upheavtt14.h"

PM_Error PACMAN_Init(void)
{
    if (!LCD_IsInitialized())
        return PM_ERR_UNINIT_LCD;

    TP_Init();

    // Displaying the welcome screen
    LCD_SetBackgroundColor(LCD_COL_BLACK);

    const LCD_FontID font_upheavtt14 = LCD_FMAddFont(Font_Upheavtt14);

    LCD_ObjID welcome_id;
    const u16 logo_x = (LCD_GetWidth() - Image_PACMAN_Logo.width) / 2;
    const u16 logo_y = (LCD_GetHeight() - Image_PACMAN_Logo.height) / 2;
    LCD_BEGIN_DRAWING;
    welcome_id = LCD_OBJECT(welcome, 3, {
        LCD_TEXT(79, logo_y - 6, {
            .text = "Welcome to",
            .font = font_upheavtt14,
            .text_color = LCD_COL_WHITE,
            .bg_color = LCD_COL_NONE,
        }),
        LCD_IMAGE(logo_x, logo_y, Image_PACMAN_Logo),
        LCD_TEXT(27, logo_y + Image_PACMAN_Logo.height + 12, {
            .text = "Press anywhere to start!",
            .font = font_upheavtt14,
            .text_color = LCD_COL_WHITE,
            .bg_color = LCD_COL_NONE,
        }),
    });
    LCD_END_DRAWING;
    
    TP_WaitForTouch();
    LCD_RQRemoveObject(welcome_id, false);

    // Showing the maze & top infos

    LCD_BEGIN_DRAWING;
    LCD_OBJECT(maze_infos, 4, {
        LCD_TEXT(5, 5, {
            .text = "GAME OVER IN",
            .font = font_upheavtt14,
            .text_color = LCD_COL_WHITE,
            .bg_color = LCD_COL_NONE,
        }),
        LCD_TEXT(LCD_GetWidth() / 2 - 10, 5, {
            .text = "SCORE",
            .font = font_upheavtt14,
            .text_color = LCD_COL_WHITE,
            .bg_color = LCD_COL_NONE,
        }),
        LCD_TEXT(LCD_GetWidth() - 60, 5, {
            .text = "RECORD",
            .font = font_upheavtt14,
            .text_color = LCD_COL_WHITE,
            .bg_color = LCD_COL_NONE,
        }),
        LCD_IMAGE(0, LCD_GetHeight() - Image_PACMAN_Maze.height, Image_PACMAN_Maze),  
    });
    LCD_END_DRAWING;
    return PM_ERR_NONE;
}