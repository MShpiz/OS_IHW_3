all: client server
client: client.c DieWithError.c
	gcc client.c DieWithError.c -o client
server: server.c DieWithError.c
	gcc TCPEchoServer.h server.c DieWithError.c \
	HandleTCPClient.c AcceptTCPConnection.c CreateTCPServerSocket.c \
	-o server
