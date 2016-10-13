#include <stdlib.h>
#include <string.h>
#include <arpa/inet.h>
#include <sys/types.h>
#include <sys/socket.h>
#include "utility.h"
#include "broadcast.h"
#include "peerConnect.h"

void goodBye(){
	notify("GoodBye");
}

int main(int argc, char *argv[])
{
	#define SRC_PORT 20000
	#define DST_PORT 20001
	#define MAX_ROUNDS 5
	#define WAIT_TIME 5
	#define QLEN 5

	int rt = atexit(goodBye);
	if(rt!=0) exit_err("could not register atexit event");

	int udp_fd = createSocket(0, AF_INET, SRC_PORT, INADDR_ANY, 0);
	int tcp_fd = createSocket(1, AF_INET, SRC_PORT, INADDR_ANY, QLEN);
	int peer = 0;
	
	while(1){
		static int rounds = 0;
		send_broadcast(udp_fd, DST_PORT, "KnockKnock");
		peer = establishConnection(tcp_fd, WAIT_TIME);
		if(peer > 0) { close(udp_fd); break; }
		++rounds;
		if(rounds > MAX_ROUNDS){
			close(udp_fd);
			close(tcp_fd);
			notify("could not find a receipent");
			exit_err("max rounds crossed");
		} 
	}
	
	notify("made connection");

	#undef SRC_PORT
	#undef DST_PORT
	#undef MAX_ROUNDS
	#undef WAIT_TIME
	#undef QLEN

	return 0;
}