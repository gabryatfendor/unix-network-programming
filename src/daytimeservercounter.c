#include <unp.h>
#include <time.h>

int main(int argc, char **argv) {
	int listenfd, connfd;
	struct sockaddr_in servaddr;
	char buff[MAXLINE];
	time_t ticks;

	listenfd = Socket(AF_INET, SOCK_STREAM, 0);

	bzero(&servaddr, sizeof(servaddr));
	servaddr.sin_family = AF_INET;
	servaddr.sin_addr.s_addr = htonl(INADDR_ANY);
	servaddr.sin_port = htons(9999);

	Bind(listenfd, (struct sockaddr *) &servaddr, sizeof(servaddr));
	Listen(listenfd, LISTENQ);

	for(;;) {
		connfd = Accept(listenfd, (struct sockaddr *) NULL, NULL);
		ticks = time(NULL);
		snprintf(buff, sizeof(buff), "%.24s\r\n", ctime(&ticks));
		//Excercise 1.5: we iterate through the whole buff string
		//calling write once for every char. The relative client 
		//will print out the number of time it will read
		for(int i = 0;i < strlen(buff); i++) {
			Write(connfd, &buff[i], sizeof(char));
		}
		Close(connfd);
	}
}
