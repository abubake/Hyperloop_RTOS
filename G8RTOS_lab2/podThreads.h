#ifndef PODTHREADS_H_
#define PODTHREADS_H_

#include "cc3100_usage.h"
#include "msp.h"
#include "stdbool.h"
#include "G8RTOS.h"

/*********************************************** Data Structures ********************************************************************/

#pragma pack ( push, 1)
/*
 * Struct to be sent from the pod/client to the hub/host
 */
typedef struct
{
    uint32_t IP_address;
    uint8_t podNumber;
    bool ready;
    bool joined;
    bool acknowledge;
} SpecificPodInfo_t;

/*
 * Struct to be sent from the host to the client
 */
typedef struct
{
    //SpecificPlayerInfo_t player;
    //GeneralPlayerInfo_t players[MAX_NUM_OF_PLAYERS];
    //balls_t balls[MAX_NUM_OF_BALLS];                     //Nick changed Ball_t to balls_t
    //uint16_t numberOfBalls;
    //bool winner;
    //bool gameDone;
    //uint8_t LEDScores[2];
    //uint8_t overallScores[2];
	uint8_t stopPod;
} PodControls_t;

#pragma pack ( pop )

/*********************************************** Data Structures ********************************************************************/

/*
 * Semaphores
 */
semaphore_t USING_WIFI;

/*********************************************** Pod/Client Threads *********************************************************************/
/*
 * Thread for pod to join hub
 */
void JoinHub();

/*
 * Thread that receives packets from hub
 */
void ReceiveDataFromHub();

/*
 * Thread that sends UDP packets to hub
 */
void SendDataToHub();

/*
 * Changes the speed on the pod
 */
void AdjustPodSpeed();

/*
 * Stops to pod automatically in case of emergency
 */
void EmergencyStop();

/*
 * Gets data from ultra-sonic sensor and stores it for use in determining potential collisions
 */
void UltraSonicSensor();

/*
 * Gets the accelerometer data from the pod
 */
void Accelerometer();

/*
 * End of game for the client
 */
void EndOfHubConnection();

/*********************************************** Pod/Client Threads *********************************************************************/

/*********************************************** Hub/Host Threads *********************************************************************/
/*
 * Thread for the hub to connect to pods
 */
void CreateConnection();

/*
 * Thread that sends data to a pod
 */
void SendDataToPod();

/*
 * Thread that receives UDP packets from pod
 */
void ReceiveDataFromPod();

/*
 * Sends data to the UI for storage and for map updating
 */
void SendDataToUI();


/*
 * Receives data from the UI for potential user control of pods
 */
void RecieveDataFromUI();


/*
 * Thread to move a single Pod
 */
void MovePod();

/*
 * Terminates a hub's connection with a pod
 */
void EndOfPodConnection();

/*********************************************** Hub/Host Threads *********************************************************************/

/*********************************************** Common Threads *********************************************************************/
/*
 * Idle thread
 */
void IdleThread();

/*********************************************** Common Threads *********************************************************************/

/*********************************************** Public Functions *********************************************************************/
/*
 * Returns either Host or Client depending on button press
 */
playerType GetPlayerRole();

/*
 * delay milliseconds when system clock is at 3 MHz
 */
void DelayMs(int n);

/*********************************************** Public Functions *********************************************************************/

#endif /* PODTHREADS_H_ */

