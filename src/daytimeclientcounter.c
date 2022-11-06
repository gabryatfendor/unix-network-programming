#include <unp.h>

int main(int argc, char **argv)
{
	int sockfd, n, counter = 0;
	char recvline[MAXLINE + 1];
	struct sockaddr_in servaddr;


	if (argc != 2) {
		err_quit("usage: a.out <IPAddress>");
	}

	sockfd = Socket(AF_INET, SOCK_STREAM, 0);

	bzero(&servaddr, sizeof(servaddr));
	servaddr.sin_family = AF_INET;
	servaddr.sin_port = htons(9999);
	Inet_pton(AF_INET, argv[1], &servaddr.sin_addr);

	Connect(sockfd, (struct sockaddr *) &servaddr, sizeof(servaddr));

	while((n = read(sockfd, recvline, MAXLINE)) > 0) {
		if(n > 0) {
			counter++;
		}
		recvline[n] = 0;
		Fputs(recvline, stdout);
	}

	if(n<0) {
		err_sys("read error");
	}
	printf("counter -> %d\n", counter);
	exit(0);
}

