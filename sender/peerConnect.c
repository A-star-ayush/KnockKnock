#include <arpa/inet.h>
#include <stdlib.h>
#include <sys/types.h>
#include <sys/socket.h>
#include <sys/time.h>
#include <sys/select.h>
#include <errno.h>
#include <poll.h>
#include "utility.h"
#include "peerConnect.h"

int establishConnection(int tcp_fd, int wait_time){
	
	static struct pollfd x;
	x.fd = tcp_fd;
	x.events = POLLIN;

	int rt = poll(&x, 1, wait_time*1000);   // the time argument is in milliseconds
	if(rt == 0) return -1;  // timed out
	if(rt < 0) exit_on_error("establishConnection: poll");

	struct sockaddr_in padd;
	unsigned int sz = sizeof(struct sockaddr_in);

	int peer = accept(tcp_fd, aCast(&padd), &sz);
	if(peer < 0) exit_on_error("establishConnection: accept");

	return peer;

}
