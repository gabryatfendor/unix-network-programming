/*
	This server is non concurrent: if more than one connections income they will be queued by the kernel and served one at a time
*/
#include <unp.h>
#include <time.h>

int main(int argc, char **argv) {
	int listenfd, connfd;
	struct sockaddr_in servaddr;
	char buff[MAXLINE];
	time_t ticks;

	//the 3 steps required to setup a TCP socket for listening are socket, bind and listen
	listenfd = Socket(AF_INET, SOCK_STREAM, 0);

	bzero(&servaddr, sizeof(servaddr));
	servaddr.sin_family = AF_INET;
	//INADDR_ANY means we are ready to receive connection on any interface if we have more than one
	servaddr.sin_addr.s_addr = htonl(INADDR_ANY);
	servaddr.sin_port = htons(13);

	Bind(listenfd, (struct sockaddr *) &servaddr, sizeof(servaddr));
	//listen convert a socket to a listening socket where we will accept connections from clients
	//LISTENQ, defined in the header, is the maximum client connections that will be queued for this listening descriptor 
	Listen(listenfd, LISTENQ);

	for(;;) {
		//accept put the process to sleep while waiting for an incoming connection. When the TCP 3 way handshakes is done the accept functions return a new descriptor
		connfd = Accept(listenfd, (struct sockaddr *) NULL, NULL);
		ticks = time(NULL);
		snprintf(buff, sizeof(buff), "%.24s\r\n", ctime(&ticks));
		Write(connfd, buff, strlen(buff));

		Close(connfd);
	}
}
