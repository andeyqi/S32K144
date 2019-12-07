#include <stdlib.h>
#include <stdio.h>
#include <string.h>
#include "clock_manager.h"
#include "clockMan1.h"
#include "pin_mux.h"
#include <stdint.h>
#include "lpuart_driver.h"
#include "easyflash.h"

void delay(void)
{
    volatile int loop;
    for(loop = 0;loop  < 1000000;loop++)
    {
        
    }
}

static void test_env(void);

 ftm_state_t ftm2_state;
int main(int argc,char ** argv)
{
   // static ftm_state_t ftm2_state;
    /*  init clock*/
    CLOCK_SYS_Init(g_clockManConfigsArr, CLOCK_MANAGER_CONFIG_CNT, g_clockManCallbacksArr, CLOCK_MANAGER_CALLBACK_CNT);
    CLOCK_SYS_UpdateConfiguration(0U, CLOCK_MANAGER_POLICY_FORCIBLE);
    
    /* init gpio */
    PINS_DRV_Init(NUM_OF_CONFIGURED_PINS, g_pin_mux_InitConfigArr);
    
    LPUART_DRV_Init(0,&lpuart1_State,&lpuart1_InitConfig0);

    EDMA_DRV_Init(&dmaController1_State,&dmaController1_InitConfig0,edmaChnStateArray,edmaChnConfigArray,EDMA_CONFIGURED_CHANNELS_COUNT);

    /* init pwm timer */
    FTM_DRV_Init(2, &flexTimer_pwm1_InitConfig, &ftm2_state);
    FTM_DRV_InitPwm(2, &flexTimer_pwm1_PwmConfig);
    FTM_DRV_UpdatePwmChannel(2, 0U, FTM_PWM_UPDATE_IN_DUTY_CYCLE, 0x4000 ,0U, true);
    /* init pwm channel */
    
    uint16_t duty = 0x00;
    /* test code */
    if (easyflash_init() == EF_NO_ERR) {
        /* test Env demo */
        test_env();
    } 
    while(1)
    {
        PINS_DRV_WritePin(PTC, 17,1);
        PINS_DRV_WritePin(PTD, 17,1);
        delay();
        PINS_DRV_WritePin(PTC, 17,0);
        PINS_DRV_WritePin(PTD, 17,0);
        delay();
        //uint8_t buff[10] = {'h','e','l','l','o','\r','\n'};
        //LPUART_DRV_SendData(0,buff,7);
        //FTM_DRV_UpdatePwmChannel(2, 0U, FTM_PWM_UPDATE_IN_DUTY_CYCLE, duty ,0U, true);
        printf("hello world. %f \r\n",3.141/2);
        duty = (duty + 0x1000)%0x9000;
        delay();
        delay();
    }
    return 0;
}

/**
 * Env demo.
 */
static void test_env(void) {
    uint32_t i_boot_times = 0;
    char *c_old_boot_times, c_new_boot_times[11] = {0};

    /* get the boot count number from Env */
    c_old_boot_times = ef_get_env("boot_times");
    //assert_param(c_old_boot_times);
    i_boot_times = atol(c_old_boot_times);
    /* boot count +1 */
    i_boot_times ++;
    printf("The system now boot %ld times\n\r", i_boot_times);
    /* interger to string */
    sprintf(c_new_boot_times,"%ld", i_boot_times);
    /* set and store the boot count number to Env */
    ef_set_env("boot_times", c_new_boot_times);
    ef_save_env();
}