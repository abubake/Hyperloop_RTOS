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
#include "driverlib.h"

PodControls_t podCommand; // Struct containing control data sent from the hub
SpecificPodInfo_t podLink; // Struct contain essential pod data and used for wireless connection (pod to hub)
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


		/* Connection established, launch RTOS (Turns on red LED to indicate connection */
		BITBAND_PERI(P2->DIR, 0) = 1;
		BITBAND_PERI(P2->OUT, 0) = 1;

	    G8RTOS_AddThread(ReceiveDataFromHub, 3, "ReceiveDataFromHost");//2
	    G8RTOS_AddThread(SendDataToHub, 3, "SendDataToHost");//3
	    G8RTOS_AddThread(IdleThread, 5, "idle");//54
		G8RTOS_KillSelf();
		DelayMs(1);

}

/*
 * Thread that receives needed info from hub
 */
void ReceiveDataFromHub(){
	/*
	• Continually receive data until a return value greater than zero is returned (meaning valid data has been read)
	o Note: Remember to release and take the semaphore again so you’re still able to send data
	o Sleeping here for 1ms would avoid a deadlock
	*/
	while(1){
		G8RTOS_WaitSemaphore(&USING_WIFI);
		//G8RTOS_WaitSemaphore(&USING_SPI);
		ReceiveData((uint8_t *)&podCommand, sizeof(podCommand));
		G8RTOS_SignalSemaphore(&USING_WIFI);
		//G8RTOS_SignalSemaphore(&USING_SPI);
		sleep(1); // may need to change this
	}
}

/*
 * Thread that sends UDP packets to hub
 */
void SendDataToHub(){
	while(1){
		//send data to host and sleep (need to fill in parameters of function (from cc3100_usage.h))
		G8RTOS_WaitSemaphore(&USING_WIFI);
		SendData((uint8_t *)&podLink, HOST_IP_ADDR, sizeof(podLink));
		G8RTOS_SignalSemaphore(&USING_WIFI);
		sleep(2);
	}
}

/*
 * Changes the speed on the pod
 */
void AdjustPodSpeed(){
	//FIXME: write this thread
}

/*
 * Stops to pod automatically in case of emergency
 */
void EmergencyStop(){
	//FIXME: write this thread
}

/*
 * Gets data from ultra-sonic sensor and stores it for use in determining potential collisions
 */
void UltraSonicSensor(){
	//FIXME: write this thread
}

/*
 * Gets the accelerometer data from the pod
 */
void Accelerometer(){
	//FIXME: write this thread
}

/*
 * End of game for the client
 */
void EndOfHubConnection(){
	//FIXME: write this thread
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
	while(1){
		/*
		• Fill packet for client
		• Send packet
		• Check if game is done
		o If done, Add EndOfGameHost thread with highest priority
		• Sleep for 5ms (found experimentally to be a good amount of time for synchronization)
		*/
		G8RTOS_WaitSemaphore(&USING_WIFI);
		SendData((uint8_t *)&podCommand, podLink.IP_address, sizeof(podCommand));
		G8RTOS_SignalSemaphore(&USING_WIFI);
		sleep(5);

		    }
}

/*
 * Thread that receives UDP packets from pod
 */
void ReceiveDataFromPod(){
	while(1){
	/*
	• Continually receive data until a return value greater than zero is returned (meaning valid data has been read)
    o Note: Remember to release and take the semaphore again so you’re still able to send data
	o Sleeping here for 1ms would avoid a deadlock
	*/
		G8RTOS_WaitSemaphore(&USING_WIFI);
	    //G8RTOS_WaitSemaphore(&USING_SPI);
	    ReceiveData((uint8_t *)&podLink, sizeof(podLink));
	    G8RTOS_SignalSemaphore(&USING_WIFI);
	    //G8RTOS_SignalSemaphore(&USING_SPI);
	    sleep(1);
	}
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
