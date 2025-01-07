#include "peripherals.h"
#include "system.h"
#include "pacman.h"

int main(void)
{
    SystemInit();
    LCD_Init(LCD_ORIENT_VER);
    PACMAN_Init();

    POWER_Init(POWR_CFG_SLEEP_ON_EXIT);
    POWER_SleepOnWFI();
    POWER_WaitForInterrupts();
}