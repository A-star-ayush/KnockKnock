#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <unistd.h>
#include <arpa/inet.h>
#include <sys/types.h>
#include <sys/socket.h>
#include "utility.h"
#include "broadcast.h"

#define BROADCST_ADDR "127.255.255.255"
// change the broadcast address to 127.255.255.255 to do testing locally

void send_broadcast(int lsoc, short destPort, char* msg)
{
	
	int broadcast = 1;
 	int rt = setsockopt(lsoc, SOL_SOCKET, SO_BROADCAST, &broadcast, sizeof(broadcast));
 	if(rt < 0) exit_on_error("setsockopt");

 	struct sockaddr_in to;
 	bzero(&to, sizeof(struct sockaddr_in));

 	to.sin_family = AF_INET;
 	rt = inet_aton(BROADCST_ADDR, &to.sin_addr);
	if(rt == 0) exit_on_error("inet_aton");
	to.sin_port = htons(destPort);

	rt = sendto(lsoc, msg, strlen(msg)+1, 0, aCast(&to), sizeof(struct sockaddr_in));
	if(rt <= 0) exit_on_error("sendto");

	printf("Sn: %s\n", msg);
	
}

#undef BROADCST_ADDR
