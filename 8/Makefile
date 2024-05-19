all: client server log_server
client: client.c DieWithError.c
	gcc client.c DieWithError.c -o client
server: server.c DieWithError.c
	gcc TCPEchoServer.h server.c DieWithError.c \
	HandleTCPClient.c AcceptTCPConnection.c CreateTCPServerSocket.c \
	-o server
log_server: log_server.c DieWithError.c
	gcc log_server.h log_server.c DieWithError.c \
	HandleLogClient.c AcceptTCPConnection.c CreateTCPServerSocket.c \
	-o log_server
