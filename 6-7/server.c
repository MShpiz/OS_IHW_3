#include "TCPEchoServer.h"
#include "signal.h"

void ProcessMain(int servSock, int, int);         /* Main program of process */

int servSock;                    /* Socket descriptor for server*/
void my_handler(int nsig) {
  if (servSock != -1) {
    close(servSock);
  }
  exit(0);
}

int main(int argc, char *argv[])
{
    (void)signal(SIGINT, my_handler);
    
    unsigned short echoServPort;     /* Server port */
    pid_t processID;                 /* Process ID */
    unsigned int processLimit;       /* Number of child processes to create */
    unsigned int processCt;          /* Process counter */

    struct sockaddr_in logServAddr; /* Log server address */
    unsigned short logServPort;     /* Log server port */
    char *servIP;

    if (argc != 5)     /* Test for correct number of arguments */
    {
        fprintf(stderr,"Usage:  %s <SERVER PORT> <WORK TIME> <LOG IP> <LOG PORT>\n", argv[0]);
        exit(1);
    }

    echoServPort = atoi(argv[1]);  /* First arg: local port */

    int work_time = atoi(argv[2]);
    servIP = argv[3];
    logServPort = atoi(argv[4]);
    int logsock;
    if ((logsock = socket(PF_INET, SOCK_STREAM, IPPROTO_TCP)) < 0)
        DieWithError("socket() failed");
    printf("log soket %d\n", logsock);

      /* Construct the server address structure */
      memset(&logServAddr, 0, sizeof(logServAddr)); /* Zero out structure */
          logServAddr.sin_family = AF_INET;              /* Internet address family */
          logServAddr.sin_addr.s_addr = inet_addr(servIP); /* Server IP address */
          logServAddr.sin_port = htons(logServPort);      /* Server port */

     if (connect(logsock, (struct sockaddr *)&logServAddr, sizeof(logServAddr)) < 0)
        DieWithError("connect() failed");

    servSock = CreateTCPServerSocket(echoServPort);
    for (processCt=0; processCt < 10; processCt++)
        /* Fork child process and report any errors */
        if ((processID = fork()) < 0)
            DieWithError("fork() failed");
        else if (processID == 0)  /* If this is the child process */
            ProcessMain(servSock, work_time, logsock);

    exit(0);  /* The children will carry on */
}

void ProcessMain(int servSock, int work_time, int logsock)
{
    int clntSock;                  /* Socket descriptor for client connection */

    for (;;)  /* Run forever */
    {
        clntSock = AcceptTCPConnection(servSock);
        printf("with child process: %d\n", (unsigned int) getpid());
        HandleTCPClient(clntSock, work_time, logsock);
    }
}
