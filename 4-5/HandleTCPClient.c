#include <stdio.h>      /* for printf() and fprintf() */
#include <sys/socket.h> /* for recv() and send() */
#include <unistd.h>     /* for close() */
#include <stdlib.h>     /* for atoi() and exit() */
#include <string.h>     /* for memset() */

#define RCVBUFSIZE 32 /* Size of receive buffer */

void DieWithError(char *errorMessage); /* Error handling function */



int queue[10];
int last = 0;

char echoBuffer[RCVBUFSIZE]; /* Buffer for echo string */
int recvMsgSize;             /* Size of received message */

void fill_queue(int clntSocket) {
  if ((recvMsgSize = recv(clntSocket, echoBuffer, RCVBUFSIZE, 0)) < 0)
    DieWithError("recv() failed");
  char tmp[RCVBUFSIZE];
  int c = 0;
  printf("%s\n",echoBuffer);
  for (size_t i = 0; i < strlen(echoBuffer); i++) {
    if (echoBuffer[i] == '|') {
      tmp[c] = '\0';
      if (last < 10){
        queue[last++] = atoi(tmp); 
      } else { //выгоняем всех кто не попал в очередь
        printf("queue is full, client %s left\n", tmp);
        tmp[c] = '-';
        tmp[c+1] = '\0';
        if (send(clntSocket, tmp, recvMsgSize, 0) != recvMsgSize)
          DieWithError("send() failed");
      }
      c = 0;
      
    } else {
      tmp[c++] = echoBuffer[i];
    }
  }
}



void HandleTCPClient(int clntSocket, int workTime) {
  printf("woke up\n");
  
  
  /* Receive message from client */
  

  /* Send received string and receive again until end of transmission */
  
  
  while (1) /* zero indicates end of transmission */
  {
    fill_queue(clntSocket);
    for (int j = 0; j < RCVBUFSIZE; j++) {echoBuffer[j] = '\0';}
      while (last > 0) {
        fill_queue(clntSocket);
        printf("workig with client %d\n", queue[last-1]);
        sleep(workTime);
        sprintf(echoBuffer, "%d", queue[last-1]);
        int l = strlen(echoBuffer);
        echoBuffer[l] = '|';
        echoBuffer[l+1] = '\0';
        if (send(clntSocket, echoBuffer, recvMsgSize, 0) != recvMsgSize)
          DieWithError("send() failed");
        printf("done with current client\n");
        last--;
      }
    /* Echo message back to client */
    
    for (int j = 0; j < RCVBUFSIZE; j++) {echoBuffer[j] = '\0';}
    // printf("buffer %s \n", echoBuffer);
    /* See if there is more data to receive */
    if ((recvMsgSize = recv(clntSocket, echoBuffer, RCVBUFSIZE, 0)) < 0)
      DieWithError("recv() failed");
      
  }
    

  //close(clntSocket); /* Close client socket */
  printf("going to sleep\n");
}
