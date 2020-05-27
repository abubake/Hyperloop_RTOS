/*
 * podThreads.c
 *
 *  Created on: May 20, 2020
 *      Author: baker
 */


//#include "demo_sysctl.h" // for using the delayMs function
#include "cc3100_usage.h"
#include "msp.h"
#include "podThreads.h"
#include "G8RTOS.h"

SpecificPodInfo_t podLink; // Structure contain essential pod data and used for wireless connection
uint8_t podID = 0; // identification number for each pod
/*********************************************** Pod/Client Threads *********************************************************************/
/*
 * Thread for pod to join hub
 */
void JoinHub(){ //FIXME: JoinHub currently based on connection via
				//one static and a dynamic IP, maybe need all static for multiple embedded devices?
	//isClient = true;
		podLink.IP_address = getLocalIP();
		podLink.acknowledge = true;
		podLink.joined = true;
		podLink.podNumber = podID; //FIXME: Make it so podID increments when a new pod is added to system
		podLink.ready = true;

		/* Sends the pod/Client's IP address to the hub/host */
		int retval = -1;

		//pinging
		while(retval < 0 || !podLink.acknowledge){
		    podLink.acknowledge = true;
		    SendData((uint8_t *)&podLink, HOST_IP_ADDR, sizeof(podLink));
		    podLink.acknowledge = false;
		    retval = ReceiveData((uint8_t *)&podLink, sizeof(podLink));
		}

		//tell the host that I am ready to join
	    SendData((uint8_t *)&podLink, HOST_IP_ADDR, sizeof(podLink));


		/* Connection established, launch RTOS */
		BITBAND_PERI(P2->DIR, 0) = 1;
		BITBAND_PERI(P2->OUT, 0) = 1;

	    G8RTOS_AddThread(ReceiveDataFromHub, 3, "ReceiveDataFromHost");//2
	    G8RTOS_AddThread(SendDataToHub, 3, "SendDataToHost");//3
	    G8RTOS_AddThread(IdleThread, 5, "idle");//54
		G8RTOS_KillSelf();
		DelayMs(1); //FIXME: Get a new delay function
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
	//isClient = false;
		int retval = -1;
		podLink.acknowledge = false;
	    //waiting for a client to connect
		    //Not Received  OR  not Acknowledged
	    while(retval < 0 || !podLink.acknowledge){
	        retval = ReceiveData((uint8_t *)&podLink, sizeof(podLink));
	    }

	    //Send acknowledge to client allowing to connect
	    podLink.acknowledge = true;
	    SendData((uint8_t *)&podLink, podLink.IP_address, sizeof(podLink));
	    podLink.acknowledge = false;

	    //wait until the client says it has joined
	    while(retval < 0 || !podLink.acknowledge){
	        retval = ReceiveData((uint8_t *)&podLink, sizeof(podLink));
	    }

	    // LED indicating connection
	    BITBAND_PERI(P2->DIR, 0) = 1;
	    BITBAND_PERI(P2->OUT, 0) = 1;

		/* Add these threads. (Need better priority definitions) */
	    G8RTOS_AddThread(SendDataToPod, 3, "SendDataToClient");
	    G8RTOS_AddThread(ReceiveDataFromPod, 3, "ReceiveDataFromClient");
	    G8RTOS_AddThread(IdleThread, 5, "idle");
		G8RTOS_KillSelf();
		DelayMs(1);
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
	while(1);
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

/* delay milliseconds when system clock is at 3 MHz */
void DelayMs(int n) {

    for (int j = 0; j < n; j++)
        for (int i = 750; i > 0; i--);      /* Delay */
}
/*********************************************** Public Functions *********************************************************************/
