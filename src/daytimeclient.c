/*
	To quickly install a daytime server for testing purpose (Linux environment):
	* apt install xinetd
	* edit /etc/xinetd.d/daytime enabling the tcp version of daytime (disable=no)
	* if you have a firewall allow connections from port TCP 13
	* systemctl reload xinetd
*/
#include <unp.h>

int main(int argc, char **argv)
{
	int sockfd, n;
	char recvline[MAXLINE + 1];
	/* 	sockaddr_in is imported from netinet/in
		the following fields are availabile in the structure:
		* sin_family
		* sin_port
		* sin_addr -> it's another struct with a property, s_addr
		* sin_zero -> should be set to zero, but the documentation says nothing about it*/
	struct sockaddr_in servaddr;


	if (argc != 2) {
		err_quit("usage: a.out <IPAddress>");
	}

	//AF_INET is IPv4, SOCK_STREAM is to specify TCP socket
	//the call to socket return an integer that we will use to call the socket back
	sockfd = Socket(AF_INET, SOCK_STREAM, 0);

	//bzero erases n bite starting from the first parameter
	bzero(&servaddr, sizeof(servaddr));
	servaddr.sin_family = AF_INET;
	//htons, host to network short, is part of a set of functions which convert values between host and network byte order
	servaddr.sin_port = htons(13);
	//inet_pton, presentation to numeric, convert ipv4 and ipv6 addresses from text to binary form. The parameter are ipversion (AF_INET), the string source (in this case command line parameter) and the "string" destination
	Inet_pton(AF_INET, argv[1], &servaddr.sin_addr);

	//actual socket connection. We pass as parameter the socket declared before, the sockaddr struct and the size of it
	Connect(sockfd, (struct sockaddr *) &servaddr, sizeof(servaddr));

	//everytime we read from a TCP stream we MUST enclose the reading in a loop, cause there is no way to know how much will the data be fragmented.
	while((n = read(sockfd, recvline, MAXLINE)) > 0) {
		recvline[n] = 0;
		//fputs simply writes recvline into a stream, in this case stdout
		if(fputs(recvline, stdout) == EOF) {
			err_sys("fputs error");
		}
	}

	if(n<0) {
		err_sys("read error");
	}
	
	exit(0);
}

