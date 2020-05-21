#ifndef PODTHREADS_H_
#define PODTHREADS_H_

/*********************************************** Pod/Client Threads *********************************************************************/
/*
 * Thread for pod to join hub
 */
void JoinHub();

/*
 * Thread that receives game state packets from hub
 */
void ReceiveDataFromHub();

/*
 * Thread that sends UDP packets to hub
 */
void SendDataToHub();


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

#endif /* PODTHREADS_H_ */

