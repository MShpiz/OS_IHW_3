#include <stdio.h>      /* for printf() and fprintf() */
#include <sys/socket.h> /* for recv() and send() */
#include <unistd.h>     /* for close() */
#include <stdlib.h>     /* for atoi() and exit() */
#include <string.h>     /* for memset() */

#define RCVBUFSIZE 1000 /* Size of receive buffer */

void DieWithError(char *errorMessage); /* Error handling function */


char echoBuffer[RCVBUFSIZE]; /* Buffer for echo string */
int recvMsgSize;             /* Size of received message */

void HandleLogClient(int clntSocket) {
  
  /* Receive message from client */
  if ((recvMsgSize = recv(clntSocket, echoBuffer, RCVBUFSIZE, 0)) < 0)
      DieWithError("recv() failed");

  /* Send received string and receive again until end of transmission */
  while (1)      /* zero indicates end of transmission */
  {
      /* Echo message back to client */
      if (send(clntSocket, echoBuffer, recvMsgSize, 0) != recvMsgSize)
          DieWithError("send() failed");
      
    printf("INFO FROM SOKET %d: %s\n", clntSocket,  echoBuffer);

      /* See if there is more data to receive */
      if ((recvMsgSize = recv(clntSocket, echoBuffer, RCVBUFSIZE, 0)) < 0)
          DieWithError("recv() failed");
  }

  close(clntSocket);    /* Close client socket */
}
