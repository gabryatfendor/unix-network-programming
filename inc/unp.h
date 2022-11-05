#ifndef __unp_h
#define __unp_h

#include <stdio.h>
#include <arpa/inet.h>
#include <netinet/in.h>
#include <stdlib.h>
#include <stdarg.h>
#include <syslog.h>
#include <errno.h>
#include <string.h>
#include <unistd.h>

#define MAXLINE	4096
#define LISTENQ 1024

//basic error handling

int daemon_proc;

static void err_doit(int errnoflag, int level, const char *fmt, va_list ap) {
	int errno_save, n;
	char buf[MAXLINE+1];

	errno_save = errno;

#ifdef HAVE_VSNPRINTF
	vsnprintf(buf, MAXLINE, fmt, ap);
#else
	vsprintf(buf, fmt, ap);
#endif
	n = strlen(buf);
	if(errnoflag) {
		snprintf(buf + n, MAXLINE - n, ": %s", strerror(errno_save));
	}
	strcat(buf, "\n");

	if(daemon_proc) {
		syslog(level, buf);
	} else {
		fflush(stdout);
		fputs(buf, stderr);
		fflush(stderr);
	}
	return;
}

void err_sys(const char *fmt, ...) {
	va_list ap;
	va_start(ap, fmt);

	err_doit(1, LOG_ERR, fmt, ap);
	va_end(ap);
	exit(1);
}
void err_quit(const char *fmt, ...) {
	va_list ap;
	va_start(ap, fmt);

	err_doit(0, LOG_ERR, fmt, ap);
	va_end(ap);
	exit(1);
}

//function wrappers
int Accept(int fd, struct sockaddr *sa, socklen_t *salenptr) {
	int n;
again:
	if((n=accept(fd, sa, salenptr)) < 0) {
#ifdef EPROTO
		if(errno == EPROTO || errno == ECONNABORTED) {
#else
		if(errno == ECONNABORTED) {
#endif
			goto again;
		} else {
			err_sys("accept error");
		}
	}
	return n;
}

void Bind(int fd, const struct sockaddr *sa, socklen_t salen) {
	if(bind(fd, sa, salen) < 0) {
		err_sys("bind error");
	}
}

void Connect(int fd, const struct sockaddr *sa, socklen_t salen) {
	if(connect(fd, sa, salen) < 0) {
		err_sys("connect error");
	}
}

void Listen(int fd, int backlog) {
	char *ptr;

	if((ptr=getenv("LISTENQ")) != NULL) {
		backlog=atoi(ptr);
	}
	if(listen(fd, backlog) < 0) {
		err_sys("listen error");
	}
}

int Socket(int family, int type, int protocol) {
	int n;
	if((n=socket(family, type, protocol)) < 0) {
		err_sys("socket error");
	}
	
	return n;
}

void Inet_pton(int family, const char *strptr, void *addrptr) {
	int n;
	if((n=inet_pton(family, strptr, addrptr)) < 0) {
		err_sys("inet_pton error for %s", strptr);
	} else if (n==0) {
		err_quit("inet_pton error for %s", strptr);
	}
}

void Fputs(const char *ptr, FILE *stream) {
	if(fputs(ptr, stream) == EOF) {
		err_sys("fputs error");
	}
}

void Write(int fd, void *ptr, size_t nbytes) {
	if(write(fd, ptr, nbytes) != nbytes) {
		err_sys("write error");
	}
}

void Close(int fd) {
	if(close(fd) == -1) {
		err_sys("close error");
	}
}

#endif
