#include <unp.h>
#include <time.h>

int main(int argc, char **argv) {
	int listenfd, connfd;
	struct sockaddr_in6 servaddr;
	char buff[MAXLINE];
	time_t ticks;

	listenfd = Socket(AF_INET6, SOCK_STREAM, 0);

	bzero(&servaddr, sizeof(servaddr));
	servaddr.sin6_family = AF_INET6;
	//TODO: fix addr assignment for ipv6
//	servaddr.sin6_addr.s6_addr = htonl(INADDR_ANY);
	servaddr.sin6_port = htons(13);

	Bind(listenfd, (struct sockaddr *) &servaddr, sizeof(servaddr));
	Listen(listenfd, LISTENQ);

	for(;;) {
		connfd = Accept(listenfd, (struct sockaddr *) NULL, NULL);
		ticks = time(NULL);
		snprintf(buff, sizeof(buff), "%.24s\r\n", ctime(&ticks));
		Write(connfd, buff, strlen(buff));

		Close(connfd);
	}
}
