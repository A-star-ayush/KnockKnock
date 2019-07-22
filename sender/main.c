#include <stdlib.h>
#include <string.h>
#include <arpa/inet.h>
#include <sys/types.h>
#include <sys/socket.h>
#include "utility.h"
#include "broadcast.h"
#include "peerConnect.h"
#include "files.h"


int finalstatus = 0;

void goodBye(){
	if(finalstatus) notify("GoodBye");
	else notify("Sorry");
}

int main(int argc, char *argv[])
{
	// we choose the port numbers from the empheral range: 32768 - 61000 when last checked 
	// cat /proc/sys/net/ipv4/ip_local_port_range
	
	#define SRC_PORT 40000
	#define DST_PORT 40001
	#define MAX_ROUNDS 3
	#define WAIT_TIME 5
	#define QLEN 1
	#define WAIT_FOR_REPLY 5

	int rt = atexit(goodBye);
	if(rt!=0) exit_err("could not register atexit event");

	int udp_fd = createSocket(0, AF_INET, SRC_PORT, INADDR_ANY, 0);
	int tcp_fd = createSocket(1, AF_INET, SRC_PORT, INADDR_ANY, QLEN);
	int peer = 0;
	
	while(1){
		static int rounds = 1;
		send_broadcast(udp_fd, DST_PORT, "KnockKnock");
		peer = establishConnection(tcp_fd, WAIT_TIME);
		if(peer > 0) { close(udp_fd); break; }
		++rounds;
		if(rounds > MAX_ROUNDS){
			close(udp_fd);
			close(tcp_fd);
			notify("\"could not find a receipent\"");
			exit_err("max rounds crossed");
		} 
	}
	
	close(tcp_fd);  // we no more want to expect any more clients .. single client model (can multithread for multiple client)
    
	sendfiles(peer, argv+1, WAIT_FOR_REPLY);	
	close(peer);

	finalstatus = 1;
	
	#undef SRC_PORT
	#undef DST_PORT
	#undef MAX_ROUNDS
	#undef WAIT_TIME
	#undef QLEN
	#undef WAIT_FOR_REPLY

	return 0;
}
