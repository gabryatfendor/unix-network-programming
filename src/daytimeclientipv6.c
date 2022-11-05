#include <unp.h>

int main(int argc, char **argv)
{
	int sockfd, n;
	char recvline[MAXLINE + 1];
	struct sockaddr_in6 servaddr;


	if (argc != 2) {
		err_quit("usage: a.out <IPAddress>");
	}

	//AF_INET6 is ipv6
	sockfd = Socket(AF_INET6, SOCK_STREAM, 0);

	bzero(&servaddr, sizeof(servaddr));
	servaddr.sin6_family = AF_INET6;
	servaddr.sin6_port = htons(13);
	Inet_pton(AF_INET6, argv[1], &servaddr.sin6_addr);

	Connect(sockfd, (struct sockaddr *) &servaddr, sizeof(servaddr));

	while((n = read(sockfd, recvline, MAXLINE)) > 0) {
		recvline[n] = 0;
		if(fputs(recvline, stdout) == EOF) {
			err_sys("fputs error");
		}
	}

	if(n<0) {
		err_sys("read error");
	}
	
	exit(0);
}

