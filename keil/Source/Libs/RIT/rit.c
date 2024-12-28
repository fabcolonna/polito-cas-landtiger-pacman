#include "rit.h"
#include "power.h"
#include "utils.h"
#include <LPC17xx.h>

/// @brief RIT_CLK frequency in MHz. Can be checked in the RIT GUI in Keil
_PRIVATE const u32 rit_clk_mhz = 100;

_DECL_EXTERNALLY void allocate_jobs_array(u8);
_DECL_EXTERNALLY void free_jobs_array(void);

void RIT_Init(u32 ival_ms, u16 int_priority)
{
    // Powering up the RIT
    POWER_TurnOnPeripheral(POW_PCRIT);

    CLR_BITS(LPC_SC->PCLKSEL1, 3, 26); // Clear
    SET_BIT(LPC_SC->PCLKSEL1, 26);     // Set PCLK_RIT to CCLK
    SET_BIT(LPC_SC->PCONP, 16);        // Enable power to RIT

    LPC_RIT->RICOMPVAL = (rit_clk_mhz * ival_ms * 1000);
    LPC_RIT->RICTRL = 6; // Clear on match + Timer enable for debug
    LPC_RIT->RICOUNTER = 0;

    // Enabling interrupts coming from RIT
    NVIC_EnableIRQ(RIT_IRQn);

    if (!IS_DEF_PRIORITY(int_priority) && IS_BETWEEN_EQ(int_priority, 0, 15))
        NVIC_SetPriority(RIT_IRQn, int_priority);
}

void RIT_Deinit(void)
{
    NVIC_DisableIRQ(RIT_IRQn);
    POWER_TurnOffPeripheral(POW_PCRIT);
}

void RIT_Enable(void)
{
    SET_BIT(LPC_RIT->RICTRL, 3);
}

void RIT_Disable(void)
{
    CLR_BIT(LPC_RIT->RICTRL, 3);
}

bool RIT_IsEnabled(void)
{
    return LPC_RIT->RICTRL & (1 << 3);
}