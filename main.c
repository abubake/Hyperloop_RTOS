#include "msp.h"
#include "G8RTOS.h"
#include "podThreads.h"
#include "stdlib.h"
#include "cc3100_usage.h"
#include "sl_common.h"
#include "G8RTOS.h"

#define MAIN_LAB5
#ifdef MAIN_LAB5


void main(void){
    WDT_A->CTL = WDT_A_CTL_PW | WDT_A_CTL_HOLD;

    G8RTOS_Init(); /* Sets needed variables and hardware */

    /* Sets up a semaphores for controlling resource use */
    G8RTOS_InitSemaphore(&USING_WIFI, 1);

    playerType Role = GetPlayerRole(); // role is either host or client

    initCC3100(Role);

    /* Interrupt for Host/ Hub reset */
    P1DIR &= ~BIT4;
    P1IFG &= ~BIT4; // clear interrupt flag bit
    P1IE |= BIT4; // enable interrupt on pin 4
    P1IES |= BIT4; //enables low to high transition
    P1REN |= BIT4; // pull up resistor
    P1OUT |= BIT4; // sets resistor to pull up

	//Add the appropriate starter thread for the chosen role
	if(Role == Host){
	    G8RTOS_AddThread(CreateConnection, 1, "station launch");
	}
	else if(Role == Client){
	    G8RTOS_AddThread(JoinHub, 1, "joining");
	}

	/* Initializes the Systick to trigger every 1ms and sets the priority for both PendSV and Systick  */
	/* Sets the first thread control block as the current thread, and calls the start_os assembly function */
	G8RTOS_Launch();

}

#endif /* MAIN_LAB5 */
