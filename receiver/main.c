#include <stdio.h>
#include <stdlib.h>
#include <unistd.h>
#include <math.h>
#include <string.h>
#include <arpa/inet.h>
#include <sys/types.h>
#include <sys/socket.h>
#include <errno.h>
#include <dirent.h>
#include "utility.h"
#include "broadcast.h"
#include "files.h"
#include "peerConnect.h"

int finalstatus = 0;
void goodBye(){
	if(finalstatus) notify("GoodBye");
	else notify("\"Sorry, retry after a minute\"");
}

int main(int argc, char *argv[])
{	
	// these configurations could be read from a config file as well
	if(argc > 1)
		chdir(argv[1]);
	
	#define SRC_PORT 40001
	#define WAIT_TIME 5
	#define MAX_ROUNDS 3
	#define QLEN 5
	#define WAIT_FOR_REPLY 5

	int rt = atexit(goodBye);
	if(rt!=0) exit_err("could not register atexit event");

	int udp_fd = createSocket(0, AF_INET, SRC_PORT, INADDR_ANY, 0);
	int peer = socket(PF_INET, SOCK_STREAM, 0);

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
			close(udp_fd);
			close(peer);
			notify("\"could not find a source\"");
			exit_err("max rounds crossed");
		}
	}

	getfiles(peer, WAIT_FOR_REPLY);

	finalstatus = 1;

	#undef SRC_PORT
	#undef WAIT_TIME
	#undef MAX_ROUNDS
	#undef QLEN
	#undef WAIT_FOR_REPLY

	return 0;
}
