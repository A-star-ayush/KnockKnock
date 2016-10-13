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

	int peer = accept(tcp_fd, NULL, NULL);
	if(peer < 0) exit_on_error("establishConnection: accept");

	return peer;

}

/*fd_set rfds;
	FD_ZERO(&rfds);
	FD_SET(tcp_fd, &rfds);
	
	int nfds = tcp_fd + 1;

	struct timeval timeout;      
    timeout.tv_sec = wait_time;
    timeout.tv_usec = 0;

	int rt = select(nfds, &rfds, NULL, NULL, &timeout);
	if(rt < 0) exit_on_error("select");
	if(rt == 0) return -1;

	int peer = -1;

	if(FD_ISSET(tcp_fd, &rfds)){
		peer = accept(tcp_fd, NULL, NULL);
		if(peer < 0) exit_on_error("accept");
	}
	
	return peer;
*/
