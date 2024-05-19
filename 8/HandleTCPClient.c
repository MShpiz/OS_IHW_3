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

void sendQueueRes(int pid, int logSock) {
  char str[100];
  sprintf(str, "Client %d came in and waits in line", pid);
  int len = strlen(str);
  //printf("log_soket %d\n", logSock);
  //printf("%s %d", str, len);
  if (send(logSock, str, len, 0) == -1)
    DieWithError("send() Queue sent a different number of bytes than expected");
}

void sendClientOut(int res, int pid, int logSock) {
  char str[100];
  if (res) {
    sprintf(str, "Client %d is done", pid);
  } else {
    sprintf(str, "Client %d left, queue is full", pid);
  }
  if (send(logSock, str, strlen(str), 0) == -1)
    DieWithError("send() Client sent a different number of bytes than expected");
}

char tmp[RCVBUFSIZE];
int c = 0;

int fill_queue(int clntSocket, int logSocket) {
  if ((recvMsgSize = recv(clntSocket, echoBuffer, RCVBUFSIZE, 0)) < 0)
    DieWithError("recv() failed");
  
  printf("%s\n",echoBuffer);
  for (size_t i = 0; i < strlen(echoBuffer); i++) {
    if (echoBuffer[i] == '|') {
      tmp[c] = '\0';
      // printf("%s\n", tmp);
      if (strcmp(tmp, "end") == 0) {
        close(clntSocket);
        return 0;
      }
      if (last < 10){
        queue[last++] = atoi(tmp); 
        sendQueueRes(queue[last-1], logSocket);
      } else { //выгоняем всех кто не попал в очередь
        printf("queue is full, client %s left\n", tmp);
        sendClientOut(1, atoi(tmp), logSocket);
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
  return 1;
}


void start_work(int pid, int logSock) {
  char str[100];
  sprintf(str, "Hairdresser works on client %d", pid);
  int len = strlen(str);
  printf("log_soket %d\n", logSock);
  printf("%s %d", str, len);
  if (send(logSock, str, len, 0) == -1)
    DieWithError("send() Queue sent a different number of bytes than expected");
}


void HandleTCPClient(int clntSocket, int workTime, int logSocket) {
  printf("woke up\n");
  
  
  /* Receive message from client */
  

  /* Send received string and receive again until end of transmission */
  
  int stop = 1;
  while (stop) /* zero indicates end of transmission */
  {
    stop = fill_queue(clntSocket, logSocket);
    for (int j = 0; j < RCVBUFSIZE; j++) {echoBuffer[j] = '\0';}
    while (last > 0) {
      
      stop = fill_queue(clntSocket, logSocket);
      if (stop == 0) {
        printf("client got disconnected");
        exit(0);
      }
      start_work(queue[last-1], logSocket);
      printf("workig with client %d\n", queue[last-1]);
      sleep(workTime);
      //sprintf(echoBuffer, "%d", queue[last-1]);
      int l = strlen(echoBuffer);
      echoBuffer[l] = '|';
      echoBuffer[l+1] = '\0';
      if (send(clntSocket, echoBuffer, recvMsgSize, 0) != recvMsgSize)
        DieWithError("send() failed");
      printf("done with current client\n");
      last--;
      sendClientOut(1, queue[last], logSocket);
    }
    
    
    for (int j = 0; j < RCVBUFSIZE; j++) {echoBuffer[j] = '\0';}
    // printf("buffer %s \n", echoBuffer);
    /* See if there is more data to receive */
    if ((recvMsgSize = recv(clntSocket, echoBuffer, RCVBUFSIZE, 0)) < 0)
      DieWithError("recv() failed");
  }
    

  //close(clntSocket); /* Close client socket */
  printf("going to sleep\n");
}
