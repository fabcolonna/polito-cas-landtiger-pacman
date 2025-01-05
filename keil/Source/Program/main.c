#include "peripherals.h"
#include "system.h"

int main(void)
{
    SystemInit();

    // Add your application code here

    POWER_Init(POWR_CFG_SLEEP_ON_EXIT);
    POWER_SleepOnWFI();
    POWER_WaitForInterrupts();
}