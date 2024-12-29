#include "includes.h"
#include "pacman.h"

int main(void)
{
    SystemInit();
    LCD_Init();

    // Initialize the PacMan game
    PACMAN_Init();

    POWER_Init(POWR_CFG_SLEEP_ON_EXIT);
    POWER_PowerDownOnWFI();
    POWER_WaitForInterrupts();
}