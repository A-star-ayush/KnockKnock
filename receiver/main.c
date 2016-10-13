#include <stdio.h>
#include <stdlib.h>
#include <unistd.h>
#include <math.h>
#include <string.h>
#include <arpa/inet.h>
#include <sys/types.h>
#include <sys/socket.h>
#include "utility.h"
#include "broadcast.h"

void goodBye(){
	notify("Bye");
}

int main(int argc, char const *argv[])
{	
	#define SRC_PORT 20001
	#define WAIT_TIME 5
	#define MAX_ROUNDS 5
	#define QLEN 5

	int rt = atexit(goodBye);
	if(rt!=0) exit_err("could not register atexit event");

	int udp_fd = createSocket(0, AF_INET, SRC_PORT, INADDR_ANY, 0);
	int peer = createSocket(1, AF_INET, SRC_PORT, INADDR_ANY, QLEN);

	struct sockaddr_in x;
	int sz = sizeof(struct sockaddr_in);
	rt = getsockname(udp_fd, aCast(&x), &sz);

	struct sockaddr_in* padd;

	while(1){
		static int rounds = 0;
		padd = recv_broadcast(udp_fd, WAIT_TIME);
		
		if(padd == NULL || !establishConnection(peer, padd)) ++rounds;
		else { close(udp_fd); break; }
		
		if(rounds > MAX_ROUNDS){
			notify("could not find a source");
			exit_err("max rounds crossed");
		}
	}

	notify("made connection");
	
	#undef SRC_PORT
	#undef WAIT_TIME
	#undef MAX_ROUNDS
	#undef QLEN
	
	return 0;
}
