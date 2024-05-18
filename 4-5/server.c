#include "TCPEchoServer.h"
#include "signal.h"
void ProcessMain(int servSock, int);         /* Main program of process */

int servSock;                    /* Socket descriptor for server*/
void my_handler(int nsig) {
  if (servSock != -1) {
    close(servSock);
  }
  exit(0);
}

int main(int argc, char *argv[])
{
    unsigned short echoServPort;     /* Server port */
    pid_t processID;                 /* Process ID */
    unsigned int processLimit;       /* Number of child processes to create */
    unsigned int processCt;          /* Process counter */

    if (argc != 3)     /* Test for correct number of arguments */
    {
        fprintf(stderr,"Usage:  %s <SERVER PORT> <WORK TIME>\n", argv[0]);
        exit(1);
    }

    echoServPort = atoi(argv[1]);  /* First arg: local port */

    int work_time = atoi(argv[2]);

    servSock = CreateTCPServerSocket(echoServPort);
    for (processCt=0; processCt < 10; processCt++)
        /* Fork child process and report any errors */
        if ((processID = fork()) < 0)
            DieWithError("fork() failed");
        else if (processID == 0)  /* If this is the child process */
            ProcessMain(servSock, work_time);

    exit(0);  /* The children will carry on */
}

void ProcessMain(int servSock, int work_time)
{
    int clntSock;                  /* Socket descriptor for client connection */

    for (;;)  /* Run forever */
    {
        clntSock = AcceptTCPConnection(servSock);
        printf("with child process: %d\n", (unsigned int) getpid());
        HandleTCPClient(clntSock, work_time);
    }
}
