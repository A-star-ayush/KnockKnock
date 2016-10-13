#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <unistd.h>
#include <arpa/inet.h>
#include <sys/types.h>
#include <sys/socket.h>
#include <errno.h>
#include "utility.h"
#include "broadcast.h"

struct sockaddr_in* recv_broadcast(int lsoc, int wait_time)
{
	static char buff[20];
    static struct sockaddr_in padd;

	struct timeval timeout;      
    timeout.tv_sec = wait_time;
    timeout.tv_usec = 0;

    int rt = setsockopt(lsoc, SOL_SOCKET, SO_RCVTIMEO, (char *)&timeout, sizeof(timeout));
    if(rt < 0) exit_on_error("recv_broadcast: setsockopt");

	int sz = sizeof(struct sockaddr_in);
	
	rt = recvfrom(lsoc, buff, 20, 0, aCast(&padd), &sz);
	if(rt < 0){
		if(errno == EWOULDBLOCK) return NULL;
		exit_on_error("recv_broadcast: recvfrom");
	}
	
	printf("Rc: %s\n", buff);
	return &padd;
	
}
