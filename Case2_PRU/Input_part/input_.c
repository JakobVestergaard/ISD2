#include <stdint.h>
#include <pru_cfg.h>
#include "resource_table_empty.h"
#include "prugpio.h"


volatile register uint32_t __R30; // output
volatile register uint32_t __R31; // input


#define SHARED_MEM   0x00010000
volatile unsigned int *shared = (unsigned int *) (SHARED_MEM);

#define MASK(x) (1UL << (x))

//PRU1_R31_16
//P9_26
#define gpio_pos 16

void main(void)
{
    /* Clear SYSCFG[STANDBY_INIT] to enable OCP master port */
    CT_CFG.SYSCFG_bit.STANDBY_INIT = 0;

    // GPIO-value holder may change at any time
    volatile uint32_t GPIO;

    while (1)
    {
        GPIO = __R31 & MASK(gpio_pos);

        shared[0] = GPIO;
    }
}


