#include <stdlib.h>
#include <stdio.h>
#include "clock_manager.h"
#include "clockMan1.h"
#include "pin_mux.h"

void delay(void)
{
    volatile int loop;
    for(loop = 0;loop  < 1000000;loop++)
    {
        
    }
}

int main(int argc,char ** argv)
{
    /*  init clock*/
    CLOCK_SYS_Init(g_clockManConfigsArr, CLOCK_MANAGER_CONFIG_CNT, g_clockManCallbacksArr, CLOCK_MANAGER_CALLBACK_CNT);
    CLOCK_SYS_UpdateConfiguration(0U, CLOCK_MANAGER_POLICY_FORCIBLE);
    
    /* init gpio */
    PINS_DRV_Init(NUM_OF_CONFIGURED_PINS, g_pin_mux_InitConfigArr);

    /* test code */
    while(1)
    {
        PINS_DRV_WritePin(PTC, 17,1);
        PINS_DRV_WritePin(PTD, 17,1);
        delay();
        PINS_DRV_WritePin(PTC, 17,0);
        PINS_DRV_WritePin(PTD, 17,0);
        delay();
    }
    return 0;
}