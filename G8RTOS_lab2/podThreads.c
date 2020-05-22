/*
 * podThreads.c
 *
 *  Created on: May 20, 2020
 *      Author: baker
 */


#include "demo_sysctl.h"
#include "cc3100_usage.h"
#include "msp432p401r.h"


/*********************************************** Pod/Client Threads *********************************************************************/
/*
 * Thread for pod to join hub
 */
void JoinHub(){

}

/*
 * Thread that receives needed info from hub
 */
void ReceiveDataFromHub(){

}

/*
 * Thread that sends UDP packets to hub
 */
void SendDataToHub(){

}

/*
 * Changes the speed on the pod
 */
void AdjustPodSpeed(){

}

/*
 * Stops to pod automatically in case of emergency
 */
void EmergencyStop(){

}

/*
 * Gets data from ultra-sonic sensor and stores it for use in determining potential collisions
 */
void UltraSonicSensor(){

}

/*
 * Gets the accelerometer data from the pod
 */
void Accelerometer(){

}

/*
 * End of game for the client
 */
void EndOfHubConnection(){

}

/*********************************************** Pod/Client Threads *********************************************************************/

/*********************************************** Hub/Host Threads *********************************************************************/
/*
 * Thread for the hub to connect to pods
 */
void CreateConnection(){

}

/*
 * Thread that sends data to a pod
 */
void SendDataToPod(){

}

/*
 * Thread that receives UDP packets from pod
 */
void ReceiveDataFromPod(){

}

/*
 * Sends data to the UI for storage and for map updating
 */
void SendDataToUI(){

}

/*
 * Receives data from the UI for potential user control of pods
 */
void RecieveDataFromUI(){

}


/*
 * Thread to move a single Pod
 */
void MovePod(){

}

/*
 * Terminates a hub's connection with a pod
 */
void EndOfPodConnection(){

}

/*********************************************** Hub/Host Threads *********************************************************************/

/*********************************************** Common Threads *********************************************************************/
/*
 * Idle thread
 */
void IdleThread(){

}

/*********************************************** Common Threads *********************************************************************/

/*********************************************** Public Functions *********************************************************************/
/*
 * Returns either Host or Client depending on button press
 */
playerType GetPlayerRole(){

	    P1->REN |= BIT1|BIT4; //Pull-up resistor
	    P1->OUT |= BIT1|BIT4; //Set resistor to pull-up
	    while(1)
	    {
	        if(!(P1->IN & BIT1))
	        {
	            DelayMs(10);
	            if(!(P1->IN & BIT1))
	            {
	                return Client;
	            }
	        }
	        if(!(P1->IN & BIT4))
	        {
	            DelayMs(10);
	            if(!(P1->IN & BIT4))
	            {
	                return Host;
	            }
	        }
	    }
}
/*********************************************** Public Functions *********************************************************************/
